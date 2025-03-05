/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0 
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#ifndef _RIVEREXPLORER_PHOENIX_SERVER_HPP_
#define _RIVEREXPLORER_PHOENIX_SERVER_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "Log.hpp"
#include "IO.hpp"
#include "PhoenixEvent.hpp"
#include "Certs.hpp"
#include "IPPeer.hpp"
#else
#include <RiverExplorer/Phoenix/Log.hpp>
#include <RiverExplorer/Phoenix/IO.hpp>
#include <RiverExplorer/Phoenix/PhoenixEvent.hpp>
#include <RiverExplorer/Phoenix/Certs.hpp>
#include <RiverExplorer/Phoenix/IPPeer.hpp>
#endif

#ifndef W64
#include <poll.h>
#else
/**@todo W64 Server */
#endif

#include <map>
#include <deque>
#include <thread>
#include <semaphore>
#include <set>
#include <vector>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

namespace RiverExplorer::Phoenix
{
	/**
	 * @class Server Server.hpp <Phoenix/Server.hpp>
	 * This class takes packets that are to be sent or received
	 * and transmits or reads them as fast as hardware allows.
	 *
	 * The server invokes these PhoenixEvents:                                  
	 *
	 * - Server::ClientBlocked_s as "ServerClientBlocked".
	 * - Server::ClientDisconnected_s as "ServerClientDisconected".
	 * - Server::LoggingMessage_s as "ServerLoggingMessage".
	 * - Server::NewClientConnection_s as "ServerNewClientConnection".
	 * - Server::ServerReady_s as "ServerReady".
	 * - Server::ShuttingDown_s as "ServerShuttingDown".
	 * - Server::ErrorOnFd_s as "ServerErrorOnFd".
	 */
	class Server
	{
	public:

		/**
		 * Server - Default Constructor.
		 *
		 * This is a singleton object.
		 *
		 * @param ServerName The name of the program
		 * that will be used in the logs and for finding
		 * configuration information.
		 */
		Server(const std::string & ServerName);

		/**
		 * Server - Destructor.
		 */
		~Server();

		/**
		 * The singleton.
		 */
		static Server		*	Instance;
		
		/**
		 * We start as a thread.
		 *
		 * @param Port the port to listen to.
		 *
		 * @param Device The device name to listen on.
		 * When nullptr, bind to all.
		 * When not nullptr, bind to the named device (eth0, ...)
		 *
		 * @return The thread ID for the worker thread.
		 */
		static std::thread * Start(uint16_t Port, const char * Device = nullptr);

		/**
		 * Remove a connection.
		 *
		 * @param Fd The file descriptor to remove.
		 */
		static void				RemoveFd(int Fd);

		/**
		 * Add a connection.
		 *
		 * @param Fd The file descriptor to add.
		 */
		static void				AddFd(int Fd);

		/**
		 * Get the (FILE*) where logs are sent.
		 *
		 * @return The (FILE*) where logs are being sent.
		 * May return nullptr if not set.
		 */
		static FILE * GetLogFp();
		
		/**
		 * Take a single packet and process it, and send it.
		 *
		 * It is possible on a busy network to not be able to send
		 * large packets without blocking. This method regulates the
		 * sending to each endpoint, or waits (yields), so that it other
		 * unrelated operations can proceed without blocking for the
		 * hardware to be ready.
		 *
		 * This method takes the incoming data, sorts by file descriptor,
		 * and adds to its Iov (list) of things to send.
		 *
		 * @param Pkt The packet to send. Which include the file descriptor
		 * and authentication status.
		 *
		 * @return True when the packet was accepted for processing.
		 * Return false if something in the Pkt is not acceptable.
		 * This might mean that the file descriptor is closed, or missing
		 * or invalid.
		 */
		static bool	Send(IO::SendPacket * Pkt);

		/**
		 * Client Map.
		 *
		 * Map a file descriptor to the connection information.
		 * This is the data associated with a file descriptor.		 
		 */
		struct ClientInfo
		{
			/**
			 * Our connection details, including SSL/TLS
			 */
			Cert								OurConnection;
			
			/**
			 * Their connection details, including SSL/TLS
			 */
			Cert								TheirConnection;

			/**
			 * The peer IP information.
			 */
			IPPeer							Peer;

			/**
			 * Inbound Data. It arrives in XDR encoded format.
			 */
			Iov									InboundData;
			std::mutex					InboundDataMutex;
			
			/**
			 * Outbound data is XDR encoded, then stored in blobs
			 * in the _SendData map, by outbound socket file descriptor.
			 */
			Iov									OutboundData;
			std::mutex					OutboundDataMutex;

			/**
			 * ClientInfo - Default Constructor.
			 */
			ClientInfo();

			/**
			 * ClientInfo - Destructor.
			 */
			~ClientInfo();
		};

		/**
		 * Get the peer address for a file descriptor.
		 *
		 * @param FdToGet The file descriptor to check.
		 *
		 * @return The peer address, or nullptr when not known.
		 */
		const IPPeer * GetPeer(int FdToGet);

	private:

		/**
		 * Called each time Server::Event::InvokeMessage() is called.
		 *
		 * It is a PhoenixEvent::PhoenixEventCallback method.
		 *
		 * @param Fd Ignored.
		 *
		 * @param ID Will be _LoggingMessageID.
		 *
		 * @param Data A (char*) message.
		 *
		 * @return Always returns true.
		 */
		static bool _LogMessage(int Fd, Event::Event_e ID, void *Data);

		/**
		 * Check for blocked or other IP's
		 *
		 * This is called from the PhoenixEvent "ServerNewClientConnection"
		 * (NewClientConnection_s).
		 *
		 * @param Fd The file descriptor of the new connection.
		 *
		 * @param ID The ID that was returned from the PhoenixEvent::Register()
		 * call.
		 *
		 * @param Peer The PeerInfo of the incoming connection.
		 * Cast to a (PeerInfo*).
		 *
		 * @return true if the IP is allowed.
		 * Returns false if the IP is not allowed.
		 */
		static bool _ValidateIP(int Fd, Event::Event_e ID, void * Peer);
		 
		/**
		 * Check for blocked or other IP's
		 *
		 * This is called from the PhoenixEvent ClientDisconnected_Event.
		 *
		 * @param Fd The file descriptor of the new connection.
		 *
		 * @param ID The ID that was returned from the PhoenixEvent::Register()
		 * call.
		 *
		 * @param Peer The PeerInfo of the disconnected connection.
		 * Cast to a (PeerInfo*).
		 *
		 * @return true if the IP is allowed.
		 * Returns false if the IP is not allowed.
		 */
		static bool _Disconnected(int Fd, Event::Event_e ID, void * Peer);
		 
		/**
		 * Check for blocked or other IP's
		 *
		 * This is called from the PhoenixEvent ErrorOnFd_Event
		 *
		 * @param Fd The file descriptor that had the error.
		 *
		 * @param ID The ID that was returned from the PhoenixEvent::Register()
		 * call.
		 *
		 * @param Peer The PeerInfo of the associated connection.
		 * Cast to a (PeerInfo*).
		 *
		 * @return true if the IP is allowed.
		 * Returns false if the IP is not allowed.
		 */
		static bool _ErrorOnFd(int Fd, Event::Event_e ID, void * Peer);
		 
		/**
		 * @addtogroup PhoenixEvent
		 * Server invokes the following PhoenixEvents:
		 * This object is private because only the Server can invoke
		 * these messages.
		 * Others can register to get the notifications.
		 *
		 * - ServerReady_Event
		 * The server has started is ready to start processing.
		 *
		 * - NewClientConnection
		 * Is sent each time a new connection is made to the server.
		 *
		 * - ClientDisconnected
		 * When the client connection is terminated by the client, or the server.
		 *
		 * - ServerLoggingMessage
		 * A new log message has been saved.
		 *
		 * - ServerShuttingDown
		 * The server is shutting down very soon.
		 *
		 * - ServerErrorOnFd
		 * A socket connected to a client has had some kind of error.
		 * It could be they hung up without logging out, network error.
		 * It is unknown why. Code that handles the connection should
		 * register for this PhoenixEvent to know that the connection is now dead.
		 */			 
		class _Event
			: public Event
		{
		public:

			/**
			 * @addtogroup PhoenixEvent
			 * This method calls DispatchCallbacks in the base class.
			 * Just a convenience wrapper that takes (char*) and not (void*).
			 *
			 * @param Fd The associated file descriptor, or (-1)
			 * if not associated with a file descriptor.
			 *
			 * @param ID The ID of the event being invoked.
			 *
			 * @param Data Any extra data.
			 * Currently the Server class does not send extra data
			 * and this value is always nullptr.
			 *
			 * @return true for most calls.
			 * May return false for NewClientConnection, if the
			 * callback is telling the server to block this connection.
			 */
			static bool InvokeMessage(int Fd, Event::Event_e ID, const char * Msg = nullptr);

			/**
			 * @addtogroup PhoenixEvent
			 * This method calls DispatchCallbacks in the base class.
			 * Just a convenience wrapper that takes (char*) and not (void*).
			 *
			 * @param Fd The associated file descriptor, or (-1)
			 * if not associated with a file descriptor.
			 *
			 * @param ID The ID of the event being invoked.
			 *
			 * @param Peer The associated PeerInfo.
			 *
			 * @return true for most calls.
			 * May return false for NewClientConnection, if the
			 * callback is telling the server to block this connection.
			 */
			static bool InvokePeer(int Fd, Event::Event_e ID, const IPPeer * Peer);

		private:
		
			/**
			 * @addtogroup PhoenixEvent
			 * Server Event - Constructor.
			 */
			_Event();

			/**
			 * @addtogroup PhoenixEvent
			 * Server Event -  Destructor.
			 */
			virtual ~_Event();

			/**
			 * @addtogroup PhoenixEvent
			 * This method calls DispatchCallbacks in the base class.
			 *
			 * @param Fd The associated file descriptor, or (-1)
			 * if not associated with a file descriptor.
			 *
			 * @para ID The ID of the event being invoked.
			 *
			 * @param Data Any extra data.
			 * Currently the Server class does not send extra data
			 * and this value is always nullptr.
			 *
			 * @return true for most calls.
			 * May return false for NewClientConnection, if the
			 * callback is telling the server to block this connection.
			 */
			virtual bool InvokeData(int Fd, Event_e ID, void * Data = nullptr);

		};

		/**
		 * Cleanup on exit.
		 */
		static void _Cleanup();
		
		/**
		 * The number of file descriptors we are currently handling.
		 */
		static nfds_t	_FdMax;

		/**
		 * The array needed by poll(2).
		 */
		static struct pollfd * _Entries;

		/**
		 * Break out of poll(), then it reloads the list of file
		 * descriptors to use.
		 *
		 * It does this by writing a 'F' to the _WakeUpPoll file
		 * descriptor.
		 */
		static void	_ReloadPoll();

		/**
		 * Break out of poll(), because one or more requested
		 * events are being updated. And if there happens
		 * to be FD changes, does a _ReloadPoll().
		 *
		 * This was created to keep poll() from POLLOUT events
		 * releasing poll() when there was no data to be sent.
		 *
		 * It does this by writing a 'E' to the _WakeUpPoll file
		 * descriptor.
		 */
		 static void _UpdatePollEvents();

		/**
		 * Used by _ReloadPoll().
		 * Add these to your list.
		 */
		static std::set<int>	_NewFds;

		/**
		 * Used by _ReloadPoll().
		 * Remove these from your list.
		 */
		static std::set<int>	_RemoveFds;

		/**
		 * The mutex for _RemoveFds and _NewFds.
		 */
		static std::mutex			_FdMutex;

		/**
		 * Close down and free stuff for client on FD
		 *
		 * @param Fd The associated file descriptor.
		 */
		static void			_Shutdown(int Fd);
		
		/**
		 * The way we pause poll() is to write to its '_WakeUpPoll'
		 * The handler for that Fd waits.
		 *
		 * If you write a 'F', it will update the file descriptor list.
		 *
		 * If you write a 'E', it will update its events list and not
		 * go through the rebuild FD list by default. It will look
		 * to see if the add or remove list is empty, if they are not empty
		 * it is the same as 'F'.
		 *
		 * If yo write a 'B', it will do 'E' and 'F'.
		 */
		static int	_WakeUpPoll;

		/**
		 * All logs go to this FILE pointer.
		 * Logs contain messages and errors.
		 *
		 * We use FILE and not (int) so we can use various printf() calls.
		 */
		static FILE			*	_LogFp;
		
		/**
		 * This is the file descriptor that listen() waits on.
		 */
		static int	_ListenFd;
		static sockaddr_in6	_ServerAddress; // Works with IPv4 and IPv6
		static int	_Backlog;

		/**
		 * A wrapper for write().
		 *
		 * @param Fd The file descriptor to write to.
		 *
		 * @param IovData The data to write.
		 *
		 * @return The number of octets written.
		 * Returns (-1) If the socket is invalid for any reason and this
		 * should not be attempted again.
		 */
		static ssize_t		_NetWrite(int Fd, Iov * IovData);
		
		/**
		 * The thread that we use to send out data.
		 */
		static void 	_Work();

		/**
		 * The name of the server program.
		 * This is used in log and configuration file names.
		 */
		static const char * _ServerProgramName;

		/**
		 * When true, do not use SSL/TLS.
		 * Not secure, used for debugging and intra company communications.
		 */
		static bool _NoTls;
	};
}
#endif // _RIVEREXPLORER_PHOENIX_SERVER_HPP_
