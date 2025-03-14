/**
 * @file Client.hpp
 * @copyright 2025 Douglas Mark Royer (A.K.A. RiverExplorer)
 * licensed under CC BY 4.0.
 * @author Doug Royer
 *
 * An application includes this file to connect to
 * a Phoenix Server.
 *
 * RiverExplorer is a trademark of RiverExplorer LLC.
 */
#ifndef _RIVEREXPLORER_PHOENIX_CLIENT_HPP_
#define _RIVEREXPLORER_PHOENIX_CLIENT_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "Log.hpp"
#include "Commands.hpp"
#include "PacketBody.hpp"
#include "CmdNotSupported.hpp"
#include "Iov.hpp"
#include "IO.hpp"
#include "IPPeer.hpp"
#else
#include <RiverExplorer/Phoenix/Log.hpp>
#include <RiverExplorer/Phoenix/Commands.hpp>
#include <RiverExplorer/Phoenix/PacketBody.hpp>
#include <RiverExplorer/Phoenix/CmdNotSupported.hpp>
#include <RiverExplorer/Phoenix/Iov.hpp>
#include <RiverExplorer/Phoenix/IO.hpp>
#include <RiverExplorer/Phoenix/IPPeer.hpp>
#endif

#include <map>
#include <mutex>
#include <thread>
#include <list>
#include <deque>
#include <netdb.h>      // For gethostbyname
#include <arpa/inet.h>  // For inet_addr and inet_pton

namespace RiverExplorer::Phoenix
{
	/**
	 * @class Client Client.hpp <RiverExplorer/Phoenix/Client.hpp>
	 *
	 * Perform the I/O.
	 */
	class Client
	{
	public:

		/**
		 * Client - Default constructor.
		 *
		 * @param ClientName The name of the client.
		 * This is used in logs.
		 */
		Client(const std::string & ClientName);

		/**
		 * Client - Destructor.
		 */
		~Client();
		
		/**
		 * Get the (FILE*) where logs are sent.
		 *
		 * @return The (FILE*) where logs are being sent.
		 * May return nullptr if not set.
		 */
		static FILE * GetLogFp();
		
		/**
		 * One for each connection to a server.
		 */
		struct Connection
		{
			IPPeer Peer;
			IO::ActiveConnection * Io;
			IO * Server; 
			struct ::addrinfo * Address;
		};
		
		/**
		 * Connect to host.
		 *
		 * @param HostName The host name to connect to.
		 *
		 * @param Port The port number to connect to.
		 *
		 * @param Address A reference to a struct addrinfo.
		 * The pointer will be set to nullptr, then tested.
		 * If you are re-using Address, call freeaddrinfo(Address)
		 * before calling Connect(), or you will have a memory leak.
		 *
		 * @param Capabilities A list of CAPABILITY_PRE to send.
		 * If none, then provide an empty list.
		 *
		 * @return The Connection object, or nullptr on fail.
		 */
		static Connection * ConnectTcp(const std::string & HostName,
																	 uint16_t Port,
																	 const std::vector<Command*> & Capabilities);
		
		/**
		 * Open a connection and perform AuthMD5 authentication.
		 * This is called when there has been a history of
		 * MD5 authentication already working with this same host
		 * for this same account.
		 *
		 * The MD5 authentication is started before a CAPABILITY_PRE
		 * is sent, and before caring if a CAPABILITY_PRE has been
		 * received.
		 *
		 * @param HostOrIP The hostname or IP address of the server.
		 *
		 * @param Port The port number to connect to.
		 *
		 * @param Account The account name to authenticate with.
		 *
		 * @param Pw The password to use.
		 *
		 * @param Capabilities A list of CAPABILITY_... to send.
		 * If none, then provide an empty list.
		 *
		 * @return An ActiveConnection object if the connection
		 * was established and MD5 authentication was successful.
		 *
		 * If successful, the server sent a CAPABILITY_POST.
		 * If unsuccessful, the server sent a CAPABILITY_PRE.
		 * Both of these are processed by callbacks registered
		 * by the client.
		 */
		Connection * ConnectMD5(const char * HostOrIP,
														uint16_t Port,
														const char * Account,
														const char * Pw,
														const std::vector<Command*> & Capabilities);
		
		/**
		 * Open a connection and perform AuthMD5 authentication.
		 *
		 * @param Account The account name to authenticate with.
		 *
		 * @param Pw The password to use.
		 *
		 * @return An ActiveConnection object.
		 */
		Connection * AuthenticateMD5(Client::Connection * Server,
																 const char * Account,
																 const char * Pw);
		
		/**
		 * Close down an incoming connection.
		 *
		 * @param Connection The active connection.
		 *
		 * @return true when shutdown.
		 */
		static bool CloseConnection(Connection * Connection);

		/**
		 * Get the client program name.
		 *
		 * @return The client program name that was passed
		 * into the constructor.
		 */
		static const std::string ProgramName();

		/**
		 * Start the TLS process on the open connection.
		 *
		 * @param Server The Connection to the server.
		 * The TCP connection must have alreay been made.
 		 *
		 * @return true if connected and verified.
		 * Returns false when there is no TCP connection.
		 * Returns false when certificate verification failed.
		 */
		static bool StartTls(Connection * Server);
		
	private:

		/**
		 * Called when A CAPABILITY_PRE arrives.
		 */
		void _HandleCapabilityPre(IO::ActiveConnection *);
		
		/**
		 * Called when A CAPABILITY_POST arrives.
		 */
		void _HandleCapabilityPost(IO::ActiveConnection *);

		/**
		 * I/O
		 */
		IO	*	_IO;

		/**
		 * Cleanup for exit.
		 */
		static void _Cleanup();
		 
		/**
		 * All logs go to this FILE pointer.
		 * Logs contain messages and errors.
		 *
		 * We use FILE and not (int) so we can use various printf() calls.
		 */
		static FILE			*	_LogFp;

		/**
		 * The name of the client program.
		 * This is used in log and configuration file names.
		 */
		static const char * _ClientProgramName;
		
		/**
		 * Verify the certificate.
		 *
		 * @param PreVerifyOkay
		 *
		 * @param Ctx
		 */
		static int	_ValidateCert(int PreVerifyOkay, X509_STORE_CTX * Ctx);
		
	};
}

#endif // _RIVEREXPLORER_PHOENIX_IO_HPP_
