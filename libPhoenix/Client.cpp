/**
 * Project: Phoenix
 * Time-stamp: <2025-03-06 15:29:08 doug>
 *
 * @file Client.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 03-JAN-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "Configuration.hpp"
#include "Client.hpp"
#include "Register.hpp"
#include "Iov.hpp"
#include "Commands.hpp"
#include "HostName.hpp"
#include "MD5.hpp"
#include "Log.hpp"

#ifndef W64
#include <sys/socket.h>
#include <sys/mman.h>
#include <pwd.h>
#include <sys/stat.h>
#else
/** @todo Add W64 includes */
#endif

#include <semaphore.h>
#include <sys/types.h>

namespace RiverExplorer::Phoenix
{
	/**
	 * Set true when the dispatch and listen threads have been started.
	 */
	static bool			ThreadsInited = false;
	static					std::string ClientPath;

	FILE					* Client::_LogFp = nullptr;
	const char		*	Client::_ClientProgramName;

	void
	MakeClientPath(const char * ClientProgramName)
	{
#ifndef W64
		struct passwd	*	Pwd;
		
		Pwd = getpwuid(getuid());

		if (Pwd != nullptr) {
			ClientPath = Pwd->pw_dir;
		}
#else
		ERROR IMPLEMENT W64;
#endif
		
		// Path to the directory where the log file exists,
		// under the users home.
		//
		// Linux Path:
		//
		//		~/.Phoenix/RiverExplorer/Client/<ClientName>
		//
		// Windows Path:
		//
		//		(APP_DATA)/RiverExplorer/Phoenix/Client/<ClientName>
		//
		const char * Parts[] = {
#ifndef W64			
			".Phoenix",
#endif
			"RiverExplorer",
			"Client",
			nullptr
		};

		int						POffset = 0;
		const char	*	Part = Parts[POffset];

		while (Part != nullptr) {
			ClientPath += "/";
			ClientPath += Part;
				
			if (access(ClientPath.c_str(), R_OK|W_OK|X_OK) != F_OK) {
				if (mkdir(ClientPath.c_str(), 0700) != 0) {
					// ERROR
					break;
				}
			}
			Part = Parts[++POffset];
		}
		ClientPath += "/";
		ClientPath += ClientProgramName;
		mkdir(ClientPath.c_str(), 0700);

		return;
	}
	
	Client::Client(const std::string & ClientName)
	{
		static const char * LogDevice_s = "LogDevice";

		_ClientProgramName = ClientName.c_str();

		Configuration::ClientInitializeConfiguration(ClientName);
		MakeClientPath(_ClientProgramName);

		std::string LogDevice;
		
		if (_LogFp == nullptr) {
			if (Configuration::ClientHasConfig(LogDevice_s)) {
				LogDevice = Configuration::ClientGetConfig(LogDevice_s);
			}
		}
		if (LogDevice.length() < 2) {
			// It was not set, add the default configuration
			// path.
			//
			LogDevice = ClientPath;
			LogDevice += "/";
			LogDevice += ClientName;
			LogDevice += ".log";
			Configuration::ClientSetConfig(LogDevice_s, LogDevice);
		}

		_LogFp = fopen(LogDevice.c_str(), "a");

		if (_LogFp == nullptr) {
			fprintf(stderr, "ERROR: Can not open LOG device: %s\n",
							LogDevice.c_str());
		} else {
			Log * TheLog = Log::NewLog(_LogFp, ClientName.c_str());

			_IO = new IO(false);
			atexit(_Cleanup);
		}

		return;
	}

	Client::~Client()
	{
		/**@todo ~Client */

		return;
	}
	
	void
	Client::_Cleanup()
	{
		/**@todo ~Cleanup */

		
		return;
	}

	FILE *
	Client::GetLogFp()
	{
		return(_LogFp);
	}
	
	bool
	Client::CloseConnection(Connection * Active)
	{
		bool Results = Active->Server->RemoveConnection(Active->Io->Fd);

		return(Results);
	}

	Client::Connection *
	Client::ConnectTcp(const std::string & HostOrIp,
										 uint16_t Port,
										 const std::vector<Command*> & Capabilities)
	{	
		Connection * Results = new Connection();

		
		Results->Server = new IO(false);
		Results->Io = new IO::ActiveConnection();
		Results->Io->Authenticated = false;

		struct addrinfo * Address = nullptr;
		
		Results->Io->Fd = HostName::ConnectTcp(HostOrIp, Port, Address);

		if (Results->Io->Fd == -1) {
			delete Results;
			Results = nullptr;
		} else {

		}

		return(Results);
	}

	Client::Connection *
	Client::ConnectMD5(const char * const HostOrIP,
										 uint16_t Port,
										 const char * const Account,
										 const char * const Pw,
										 const std::vector<Command*> & Capabilities)
	{
		Connection * Results = ConnectTcp(HostOrIP, Port, Capabilities);

		if (Results != nullptr) {
			// Prepare for and send an AUTHMD5
			//
			std::string MD5Pw = MD5(Pw);
		}
		
		return(Results);
	}

	const std::string
	Client::ProgramName()
	{
		return(_ClientProgramName);
	}
	
	static void
	LogSslError()
	{
		char Buf[256];

		ERR_error_string(ERR_get_error(), Buf);
		Log::PrintError("Cert.cpp:%s, Buf");

		return;
	}

	bool
	Client::StartTls(Connection * Server)
	{
		static bool Initialized = false;

		bool Results = false;
		
		if (Server != nullptr) {
			if (Server->Io != nullptr) {
				if (Server->Io->Fd > -1) {
			
					if (!Initialized) {
						SSL_library_init();
						OpenSSL_add_all_algorithms();
						SSL_load_error_strings();
					}

					const SSL_METHOD * Method = TLS_client_method();

					Server->Io->Ctx = SSL_CTX_new(Method);

					if (Server->Io->Ctx == nullptr) {
						Log::PrintError("Client.cpp:StartTls():"
														"Count not create new method.");
						LogSslError();

					} else {

						int Res = SSL_CTX_set_default_verify_paths(Server->Io->Ctx);

						if (Res != 1) {
							Log::PrintError("Client.cpp:StartTls():"
															"Could not set default verify paths.");
							LogSslError();
							SSL_CTX_free(Server->Io->Ctx);
							Server->Io->Ctx = nullptr;
					
						} else {
							SSL_CTX_set_verify(Server->Io->Ctx,
																 SSL_VERIFY_PEER,
																 _ValidateCert);

							Server->Io->Ssl = SSL_new(Server->Io->Ctx);

							if (Server->Io->Ssl == nullptr) {
								Log::PrintError("Client.cpp:StartTls():"
																"Cound not create SSL object.");
								LogSslError();
								SSL_free(Server->Io->Ssl);
								Server->Io->Ssl = nullptr;
								SSL_CTX_free(Server->Io->Ctx);
								Server->Io->Ctx = nullptr;
								
							} else {
								const char * Host = Server->Io->HostName();

								if (Host == nullptr) {
									Log::PrintError("Client.cpp:StartTls(), no hostname set.");
								} else {
									Res = SSL_set_tlsext_host_name(Server->Io->Ssl,
																								 Host);

									if (Res != 1) {
										Log::PrintError("Client.cpp:StartTls():"
																		"Could not set target hostname");
										LogSslError();
										SSL_free(Server->Io->Ssl);
										Server->Io->Ssl = nullptr;
										SSL_CTX_free(Server->Io->Ctx);
										Server->Io->Ctx = nullptr;

									} else {

										Res = SSL_connect(Server->Io->Ssl);
										if (Res != 1) {
											Log::PrintError("Client.cpp:StartTls():"
																			"Could not connect() to SSL.");
											LogSslError();
											SSL_free(Server->Io->Ssl);
											Server->Io->Ssl = nullptr;
											SSL_CTX_free(Server->Io->Ctx);
											Server->Io->Ctx = nullptr;

										} else {
											long VerifyResult = SSL_get_verify_result(Server->Io->Ssl);

											if (VerifyResult != X509_V_OK) {
												Log::PrintError("Client.cpp:StartTls():"
																				"Could not verify SSL result.");
												LogSslError();
												SSL_free(Server->Io->Ssl);
												Server->Io->Ssl = nullptr;
												SSL_CTX_free(Server->Io->Ctx);
												Server->Io->Ctx = nullptr;
												
											} else {
												X509 * Cert = SSL_get_peer_certificate(Server->Io->Ssl);

												if (Cert == nullptr) {
													SSL_free(Server->Io->Ssl);
													Server->Io->Ssl = nullptr;
													SSL_CTX_free(Server->Io->Ctx);
													Server->Io->Ctx = nullptr;
													Log::PrintError("Client.cpp::StartTls:No peer certificate returned.");
												} else {
													Res = X509_check_host(Cert,
																								Host,
																								strlen(Host),
																								0,
																								nullptr);

													if (Res != 1) {
														Log::PrintError("Client.cpp::StartTls:"
																						"Remote cert does not match"
																						" our target host name: %s",
																						Host);
														SSL_free(Server->Io->Ssl);
														Server->Io->Ssl = nullptr;
														SSL_CTX_free(Server->Io->Ctx);
														Server->Io->Ctx = nullptr;
													} else {

														Results = true;
													}
												}
											}
										}
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

	int
	Client::_ValidateCert(int PreVerifyOkay, X509_STORE_CTX * Ctx)
	{
		if (!PreVerifyOkay) {
			char Buf[256];
			
			X509 * Cert = X509_STORE_CTX_get_current_cert(Ctx);
			X509_NAME_oneline(X509_get_subject_name(Cert), Buf, sizeof(Buf));

			/**@todo Compare expected with Buf.  */
		}

		return(PreVerifyOkay);
	}
	
}
