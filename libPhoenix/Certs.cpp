/**
 * Project: Phoenix
 * Time-stamp: <2025-03-04 16:08:59 doug>
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

namespace RiverExplorer::Phoenix
{
	Cert::Cert()
	{
		_Ctx = nullptr;
		_Ssl = nullptr;
		_Private = nullptr;
		_Public = nullptr;
		_Accept = nullptr;
		_Bio = nullptr;

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

    if (_Ctx != nullptr) {
      SSL_CTX_free(_Ctx);
      _Ctx = nullptr;
    }

    if (_Private != nullptr) {
      EVP_PKEY_free(_Private);
      _Private = nullptr;
    }

    if (_Public != nullptr) {
      X509_free(_Public);
      _Public = nullptr;
    }

		return;
	}

	void
	Cert::Generate()
	{
		_GeneratePrivateKey();
		_GenerateCertificate();
		_Ctx = SSL_CTX_new(SSLv23_server_method());

		 SSL_CTX_use_certificate(_Ctx, _Public);
		 SSL_CTX_set_default_passwd_cb(_Ctx, _PasswordCb);
		 SSL_CTX_use_PrivateKey(_Ctx, _Private);

		 RSA * Rsa = RSA_generate_key(4096, RSA_F4, nullptr, nullptr);

		 SSL_CTX_set_tmp_rsa(_Ctx, Rsa);
		 RSA_free(Rsa);
		 SSL_CTX_set_verify(_Ctx, SSL_VERIFY_NONE, 0);
		 _Ssl = SSL_new(_Ctx);
		 
		return;
	}

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
	
	void
	Cert::_GeneratePrivateKey()
	{
		_Private = nullptr;

    EVP_PKEY_CTX	* PCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

    EVP_PKEY_keygen_init(PCtx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(PCtx, 4096);
    EVP_PKEY_keygen(PCtx, &_Private);

		return;
	}


	void
	Cert::_GenerateCertificate()
	{
		if (_Private != nullptr) {
			_Public = X509_new();

			X509_set_version(_Public, 2);
			ASN1_INTEGER_set(X509_get_serialNumber(_Public), 0);
			X509_gmtime_adj(X509_get_notBefore(_Public), 0);
			X509_gmtime_adj(X509_get_notAfter(_Public), (long)60*60*24 * 365);
			X509_set_pubkey(_Public, _Private);

			X509_NAME * Name = X509_get_subject_name(_Public);

			X509_NAME_add_entry_by_txt(Name,
																 "C",
																 MBSTRING_ASC,
																 (const unsigned char *) "US",
																 -1,
																 -1,
																 0);
			
			X509_NAME_add_entry_by_txt(Name,
																 "CN",
																 MBSTRING_ASC,
																 (const unsigned char*)"RiverExplorer::Phoenix",
																 -1,
																 -1,
																 0);
			
			X509_set_issuer_name(_Public, Name);
			X509_sign(_Public, _Private, EVP_md5());
		}
		
		return;
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
