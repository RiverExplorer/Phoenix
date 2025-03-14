/**
 * Project: Phoenix
 * Time-stamp: <2025-03-06 15:44:51 doug>
 * 
 * @file Certs.cpp
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

#include "Certs.hpp"
#include "IPPeer.hpp"
#include "Log.hpp"

#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <unistd.h>

namespace RiverExplorer::Phoenix
{
	std::map<std::string,EVP_PKEY*> Cert::_PrivateKeys;
	std::map<std::string,X509*>			Cert::_PublicKeys;
	std::map<std::string,SSL_CTX*>	Cert::_SniCtx;
	
	Cert::Cert()
	{
		_Ssl = nullptr;
		_Accept = nullptr;
		_Bio = nullptr;
		_FullRootCertPath = nullptr;

		return;
	}

	Cert::~Cert()
	{
		if (_Ssl != nullptr) {
      SSL_shutdown(_Ssl);
      _Ssl = nullptr;
    }

    if (_Bio != nullptr) {
      BIO_free_all(_Bio);
      _Bio = nullptr;
    }

    if (_Accept != nullptr) {
      BIO_free_all(_Accept);
      _Accept = nullptr;
    }

		// It is cached, so don't delete it.
		//
		_CtxToUse = nullptr;

		return;
	}

	
	void
	Cert::_LogSslError()
	{
		char Buf[256];

		ERR_error_string(ERR_get_error(), Buf);
		Log::PrintError("Cert.cpp:%s, Buf");

		return;
	}

	bool
	Cert::Generate(const char * TargetHost)
	{
		bool Results = false;

		if (TargetHost != nullptr) {

			std::map<std::string,EVP_PKEY*>::iterator PrivateIt;
			std::map<std::string,X509*>::iterator PublicIt;
			std::map<std::string,SSL_CTX*>::iterator CtxIt;
			
			// Generate the inital pre context, if it does not exist.
			//
			if (_CtxToUse == nullptr) {
				CtxIt = _SniCtx.find(TargetHost);

				if (CtxIt == _SniCtx.end()) {
					// No CTX for target host yet, create it.
					//
					_CtxToUse = SSL_CTX_new(TLS_server_method());

					if (_CtxToUse == nullptr) {
						Log::PrintError("Certs.cpp:Generate():"
														"Can not generate CTX for host '%s'.",
														TargetHost);
						_LogSslError();
						_CtxToUse = nullptr;
						
					} else {
						_SniCtx[TargetHost] = _CtxToUse;
					}
				}
			}

			if (_CtxToUse != nullptr) {
				PrivateIt = _PrivateKeys.find(TargetHost);
				PublicIt = _PublicKeys.find(TargetHost);
				
				if (PrivateIt == _PrivateKeys.end()
						|| PublicIt == _PublicKeys.end()) {
					
					// One or both certs not found.
					//
					/**@todo USE USER SUPPLIED CERTS!*/
				
					PrivateIt = _GeneratePrivateKey(TargetHost);
					PublicIt = _GenerateCertificate(PrivateIt);
				}

				if (PrivateIt != _PrivateKeys.end()
						&& PublicIt != _PublicKeys.end()) {
					
					_PrivateKeys[TargetHost] = PrivateIt->second;
					_PublicKeys[TargetHost] = PublicIt->second;

				} else {
					Log::PrintError("Certs.cpp:Generate():Count find or"
													" generate keys for host: '%s'.",
													TargetHost);
					_CtxToUse = nullptr;
				}

				if (_CtxToUse != nullptr) { // If no error.
					SSL_CTX_use_certificate(_CtxToUse, PublicIt->second);
					SSL_CTX_use_PrivateKey(_CtxToUse, PrivateIt->second);
					SSL_CTX_set_default_passwd_cb(_CtxToUse, _PasswordCb);

					RSA * Rsa = RSA_generate_key(4096, RSA_F4, nullptr, nullptr);

					SSL_CTX_set_tmp_rsa(_CtxTouse, Rsa);
					RSA_free(Rsa);
					SSL_CTX_set_verify(_CtxToUse, SSL_VERIFY_NONE, 0);
					_Ssl = SSL_new(_CtxToUse);
					Results = true;
				}
			}
		}
		 
		return(Results);
	}

	int
	Cert::_ClientWantsHostCallback(SSL * Ssl, int * Ad, void * Arg)
	{
		// SSL_TLSEXT_ERR_OK
		// This is used to indicate that the servername requested by the
		// client has been accepted. Typically a server will call
		// SSL_set_SSL_CTX() in the callback to set up a different
		// configuration for the selected servername in this case.
		//
		// SSL_TLSEXT_ERR_ALERT_FATAL
		// In this case the servername requested by the client is not accepted
		// and the handshake will be aborted. The value of the alert to be
		// used should be stored in the location pointed to by the al
		// parameter to the callback. By default this value is initialised to
		// SSL_AD_UNRECOGNIZED_NAME.
		//
		// SSL_TLSEXT_ERR_ALERT_WARNING
		// If this value is returned then the servername is not accepted by
		// the server. However, the handshake will continue and send a
		// warning alert instead. The value of the alert should be stored in
		// the location pointed to by the al parameter as for
		// SSL_TLSEXT_ERR_ALERT_FATAL above. Note that TLSv1.3 does not
		// support warning alerts, so if TLSv1.3 has been negotiated then this
		// return value is treated the same way as SSL_TLSEXT_ERR_NOACK.
		// 
		// SSL_TLSEXT_ERR_NOACK
		// This return value indicates that the servername is not accepted by
		// the server. No alerts are sent and the server will not acknowledge
		// the requested servername.
		//

		int		Results = 0;

		const char *RequestedHost = SSL_get_servername(_Ssl,
																									 TLSEXT_NAMETYPE_host_name);

		if (RequestedHost != nullptr) {

			/** @todo Check  against configured hosts ...*/

			// This looks for, and generates if needed ...
			//
			std::map<std::string,EVP_PKEY*>::iterator PrivateIt;

			PrivateIt = _GeneratePrivateKey(RequestedHost);

			if (PrivateIt != _PrivateKeys.end())  {
				std::map<std::string,X509*>::iterator PublicIt;
				
				PublicIt = _GenerateCertificate(PrivateIt);

				if (PublicIt != _PublicKeys.end()) {

					std::map<std::string, SSL_CTX*>::iterator CtxIt;

					CtxIt = _SniCtx.find(RequestedHost);

					if (CtxIt != _SniCtx.end()) {
						_CtxToUse = CtxIt->second;
						SSL_set_SSL_CTX(_Ssl, _CtxToUse);
						Results = SSL_TLSEXT_ERR_OK;
					}
				}
			}
		}
		
		return(Results);
	}
	
	bool
	Cert::UseCerts(const char * TargetHost,
								 const char * PrivateFile,
								 const char * PublicFile,
								 const char * /*ChainPath*/) // Not used yet.
	{
		bool		Results = false;

		/**@todo UseCerts(), ChainPath - add code. */
		
		if (TargetHost != nullptr
				&& PrivateFile != nullptr
				&& PublicFile != nullptr) {

			struct stat SBuf;
			
			// Private KEY must be  mode 0600.
			//
			memset(&SBuf, 0, sizeof(SBuf));
			stat(PrivateFile, &SBuf);

			if (!(S_ISREG(SBuf.st_mode))) {
				Log::PrintError("Certs.cpp:UseCerts():Private File must"
												" be regular file.");
				
			} else {
				uid_t Uid = getuid();
				gid_t Gid = getgid();
				
				if (Uid != SBuf.st_uid
						|| Gid != SBuf.st_gid
						|| SBuf.st_mode & S_IRGRP
						|| SBuf.st_mode & S_IWGRP
						|| SBuf.st_mode & S_IXGRP
						|| SBuf.st_mode & S_IROTH
						|| SBuf.st_mode & S_IWOTH
						|| SBuf.st_mode & S_IXOTH
						|| SBuf.st_mode & S_ISUID
						|| SBuf.st_mode & S_ISGID
						|| SBuf.st_mode & S_ISVTX
						|| SBuf.st_mode & S_IXUSR
						|| !(SBuf.st_mode & S_IRUSR)) {

					Log::PrintError("Certs.cpp:UserCerts():Private key"
													" has wrong permissions."
													" Must be readable by this this progam,"
													" and not accessable by others users."
													" The user and group must be the same"
													"  user id and groupd id as this program is"
													" runing as. Should be mode:0600 or 0400."
													" Execution or set-uid must not be set.");
					
				} else {
						
					// Public key must not be writeable to
					// group or public.
					//
					memset(&SBuf, 0, sizeof(SBuf));
					stat(PublicFile, &SBuf);

					if (!(S_ISREG(SBuf.st_mode))) {
						Log::PrintError("Certs.cpp:UseCerts():Public File must"
														" be regular file.");
						
					} else {
				
						if (Uid != SBuf.st_uid
								|| Gid != SBuf.st_gid
								|| SBuf.st_mode & S_IWGRP
								|| SBuf.st_mode & S_IXGRP
								|| SBuf.st_mode & S_IWOTH
								|| SBuf.st_mode & S_IXOTH
								|| SBuf.st_mode & S_ISUID
								|| SBuf.st_mode & S_ISGID
								|| SBuf.st_mode & S_ISVTX
								|| SBuf.st_mode & S_IXUSR
								|| !(SBuf.st_mode & S_IRUSR)) {

							Log::PrintError("Certs.cpp:UserCerts():Public key"
															" has wrong permissions."
															" Must be readable by this program,"
															" and perhaps readable by others users."
															" Can not be writable by other users."
															" The user and group must be the same"
															"  user id and groupd id as this program is"
															" runing as."
															" Execution or set-uid must not be set.");
							
						} else {

							std::map<std::string,SSL_CTX*>::iterator CtxIt;

							CtxIt = _SniCtx.find(TargetHost);

							if (CtxIt == _SniCtx.end()) {
								Log::PrintError("Certs.cpp:UseCerts:Do not have SSL_CTX"
																" for host: '%s'.",
																TargetHost);
				
							} else {
								SSL_CTX * Ctx = CtxIt->second;
			
								int Res = SSL_CTX_use_certificate_file(Ctx,
																											PublicFile,
																											SSL_FILETYPE_PEM);

								if (Res != 1) {
									Log::PrintError("Certs.cpp:UseCerts():"
																	" Use public Cert failed.");
									_LogSslError();

								} else {
									Res = SSL_CTX_use_PrivateKey_file(_CtxToUse,
																										PrivateFile,
																										SSL_FILETYPE_PEM);
									if (Res != 1) {
										Log::PrintError("Certs.cpp:UseCerts():"
																		"Use private Cert failed.");
										_LogSslError();

									} else {
										_CtxToUse = Ctx;
										SSL_set_SSL_CTX(_Ssl, Ctx);
										Results = true;
									}
								}
							}
						}
					}
				}
			}
		}
		
		return(Results);
	}

	// Server Side
	//
	int
	Cert::Accept(int Fd, FILE * ErrorsTo)
	{
		 _Accept = BIO_new_socket(Fd, BIO_CLOSE);
		 SSL_set_bio(_Ssl, _Accept, _Accept);
		 
		 int Results = SSL_accept(_Ssl);
		 
		 ERR_print_errors_fp(ErrorsTo);
		 _Bio = BIO_pop(_Accept);

		 return(Results);
	}
	
	std::map<std::string,EVP_PKEY*>::iterator
	Cert::_GeneratePrivateKey(const char * TargetHost)
	{
		std::map<std::string,EVP_PKEY*>::iterator Results = _PrivateKeys.end();
		
    EVP_PKEY_CTX	* PCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
		EVP_PKEY * Key = nullptr;
		
    EVP_PKEY_keygen_init(PCtx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(PCtx, 4096);
    EVP_PKEY_keygen(PCtx, &Key);

		_PrivateKeys[TargetHost] = Key;
		Results = _PrivateKeys.find(TargetHost);
		
		EVP_PKEY_CTX_free(PCtx);

		return(Results);
	}

	std::map<std::string,X509*>::iterator
	Cert::_GenerateCertificate(std::map<std::string,EVP_PKEY*>::iterator It)
	{
		std::map<std::string,X509*>::iterator Results = _PublicKeys.end();
		
		if (It != _PrivateKeys.end()) {
			const char * TargetHost = It->first.c_str();

			X509 * NewCert = X509_new();

			X509_set_version(NewCert, 2);
			ASN1_INTEGER_set(X509_get_serialNumber(NewCert), 0);
			X509_gmtime_adj(X509_get_notBefore(NewCert), 0);
			X509_gmtime_adj(X509_get_notAfter(NewCert), (long)60*60*24 * 365);
			X509_set_pubkey(NewCert, It->second);

			X509_NAME * Name = X509_get_subject_name(NewCert);

			X509_NAME_add_entry_by_txt(Name,
																 "C",
																 MBSTRING_ASC,
																 (const unsigned char *) "US",
																 -1,
																 -1,
																 0);

			std::string Cn = "RiverExplorer";

			X509_NAME_add_entry_by_txt(Name,
																 "O",
																 MBSTRING_ASC,
																 (const unsigned char*)Cn.c_str(),
																 -1,
																 -1,
																 0);
			Cn += ":Phoenix:";
			Cn +=  TargetHost;
			
			X509_NAME_add_entry_by_txt(Name,
																 "CN",
																 MBSTRING_ASC,
																 (const unsigned char*)Cn.c_str(),
																 -1,
																 -1,
																 0);
			
			X509_set_issuer_name(NewCert, Name);
			X509_sign(NewCert, It->second, EVP_md5());

			_PublicKeys[TargetHost] = NewCert;
			Results = _PublicKeys.find(TargetHost);
		}
		
		return(Results);
	}

	int
	Cert::CanRead()
	{
		return(SSL_pending(_Ssl));
	}

	int
	Cert::Read(void * Buf, size_t Number)
	{
		return(SSL_read(_Ssl, Buf, Number));
	}

	int
	Cert::GetError(int Res)
	{
		return(SSL_get_error(_Ssl, Res));
	}
	
	int
	Cert::_PasswordCb(char * Buf, int Size, int /*RwFlag*/, void * Password)
  {
    strncpy(Buf, (char*)Password, Size);
    Buf[Size - 1] = '\0';

    return((int)strlen(Buf));
  }

}
