/**
 * Project: Phoenix
 * Time-stamp: <2025-02-26 16:54:15 doug>
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
#include <md5.h>

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
	MakeClientPath()
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
		
		// Path to the directory where the configuration file exists,
		// under the users home.
		//
		// Linux Path:
		//
		//		~/.Phoenix/RiverExplorer/Client
		//
		// Windows Path:
		//
		//		(APP_DATA)/RiverExplorer/Phoenix/Client
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

		return;
	}
	
	Client::Client(const std::string & ClientName)
	{
		static const char * LogDevice_s = "LogDevice";

		_ClientProgramName = ClientName.c_str();
		
		Configuration::ClientInitializeConfiguration(ClientName);
		MakeClientPath();

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
			fprintf(stderr, "ERROR: Can not open LOG device: %s\n", LogDevice.c_str());
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
			char Buf[MD5_DIGEST_STRING_LENGTH];
			char * Md5 = MD5Data((uint8_t*)Pw, strlen(Pw), Buf);
		}
		
		return(Results);
	}
}

