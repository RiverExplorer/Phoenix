/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0 
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#ifndef _RIVEREXPLORER_PHOENIX_POLLIO_HPP_
#define _RIVEREXPLORER_PHOENIX_POLLIO_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "IO.hpp"
#include "PhoenixEvent.hpp"
#else
#include <RiverExplorer/Phoenix/IO.hpp>
#include <RiverExplorer/Phoenix/PhoenixEvent.hpp>
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

namespace RiverExplorer::Phoenix
{
	/**
	 * @class Server Server.hpp <Phoenix/Server.hpp>
	 * This class takes packets that are to be sent or received
	 * and transmits or reads them as fast as hardware allows.
	 *
	 * The server invokes these PhoenixEvents:                                  
	 *
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
		 * @addtogroup PhoenixEvent
		 * Contains the string "ServerReady" PhoenixEvent.
		 *
		 * All callbacks registered for this PhoenixEvent
		 * will be called with the server is ready to start
		 * processing clients.
		 *
		 * This is the name of an PhoenixEvent that callers
		 * can register for with the Server.
		 *
		 * No optional data is passed with this PhoenixEvent.
		 */
		static const char * const Ready_s;
			
		/**
		 * @addtogroup PhoenixEvent
		 * Contains the string "ServerNewClientConnection" PhoenixEvent.
		 *
		 * All callbacks registered for this PhoenixEvent
		 * will be called when a new client connection has happened.
		 * The callback will be called before the server starts
		 * to accept I/O on this connection.
		 *
		 * This is the name of an PhoenixEvent that callers
		 * can register for with the Server.
		 *
		 * The callback is supplied with the socket file descriptor
		 * of the incoming connection.
		 *
		 * The callback is supplied a PeerInfo object set to
		 * the peer's IP address as the 'Data'.
		 *
		 * The user supplied callback must return false when
		 * it wishes the connection to fail.
		 *
		 * @note
		 * If an implementation desires to do IP filtering,
		 * they would register for this PhoenixEvent
		 * and have the callback return true when okay,
		 * or false if the IP should be blocked.
		 */
		static const char * const NewClientConnection_s;

		/**
		 * @addtogroup PhoenixEvent
		 * Contains the string "ServerClientDisconnected" PhoenixEvent.
		 *
		 * All callbacks registered for this PhoenixEvent will be
		 * called when a client disconnects, or when the server
		 * disconnects a client. The callback is called after
		 * the socket connection has been terminated.
		 * 
		 * This is the name of an PhoenixEvent that callers
		 * can register for with the Server.
		 *
		 * The callback is supplied with the socket file descriptor
		 * of the incoming connection.
		 */
		static const char * const ClientDisconnected_s;

		/**
		 * @addtogroup PhoenixEvent
		 * Contains the string "ServerLogMessage" PhoenixEvent.
		 *
		 * All callbacks registered for this PhoenixEvent will be
		 * called when the server code logs a message.
		 *
		 * This is the name of an PhoenixEvent that callers
		 * can register for with the Server.
		 *
		 * The callback is supplied with:
		 *
		 * -The socket file descriptor of the incoming connection.
		 * When the file descriptor is (-1) then no file descriptor
		 * is associated with the message.
		 *
		 * -A text message that is being logged.
		 */
		static const char * const LoggingMessage_s;
		 
		/**
		 * @addtogroup PhoenixEvent
		 * Contains the string "ServerShuttingDown" PhoenixEvent.
		 *
		 * All callbacks registered for this PhoenixEvent will
		 * be called prior to the shutdown. After all callbacks
		 * have been called, the shutdown will continue.
		 *
		 * This is the name of an PhoenixEvent that callers
		 * can register for with the Server.
		 *
		 * No optional data is passed with this PhoenixEvent.
		 */
		static const char * const ShuttingDown_s;

		/**
		 * @addtogroup PhoenixEvent
		 * Contains the string "ServerErrorOnFd" PhoenixEvent.
		 *
		 * All callbacks registered for this PhoenixEvent will
		 * be called when there is some kind of error on a
		 * socket file descriptor. This means the whatever
		 * is associated with Fd, will no longer function.
		 *
		 * Watch for this event to detect that connections have been
		 * unexpectedly shut down.
		 *
		 * This is the name of an PhoenixEvent that callers
		 * can register for with the Server.
		 *
		 * No optional data is passed with this PhoenixEvent.
		 *
		 * @note
		 * ClientDisconnected_s is also sent with this event when the
		 * error is on a client connection.
		 */
		static const char * const ErrorOnFd_s;

		/**
		 * Server - Default Constructor.
		 *
		 * This is a singleton object.
		 */
		Server();

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
		 * Peer address.
		 */
		struct PeerInfo
		{
			union {
				in_addr	ipv4;
				in6_addr ipv6;
			} Addr;
			//sockaddr  Addr;
			socklen_t AddrLen;
		};
			
		/**
		 * Get the peer address for a file descriptor.
		 *
		 * @param FdToGet The file descriptor to check.
		 *
		 * @return The peer address, or nullptr when not known.
		 */
		const PeerInfo * GetPeer(int FdToGet);
		
			
			/**
			 * @addtogroup PhoenixEvent
			 * The ID for the Server "Ready" PhoenixEvent.
			 */
			static PhoenixEvent::EventID	ReadyID;

			/**
			 * @addtogroup PhoenixEvent
			 * The ID for the Server "NewClientConnection" PhoenixEvent.
			 */
			static PhoenixEvent::EventID	NewClientConnectionID;

			/**
			 * @addtogroup PhoenixEvent
			 * The ID for the Server "ClientDisconnected" PhoenixEvent.
			 */
			static PhoenixEvent::EventID	ClientDisconnectedID;

			/**
			 * @addtogroup PhoenixEvent
			 * The ID for the Server "LoggingMessage" PhoenixEvent.
			 */
			static PhoenixEvent::EventID	LoggingMessageID;

			/**
			 * @addtogroup PhoenixEvent
			 * The ID for the Server "ShuttingDown" PhoenixEvent.
			 */
			static PhoenixEvent::EventID	ShuttingDownID;
			
			/**
			 * @addtogroup PhoenixEvent
			 * The ID for the Server "ErrorOnFd" PhoenixEvent.
			 */
			static PhoenixEvent::EventID	ErrorOnFdID;

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
		static bool _LogMessage(int Fd, PhoenixEvent::EventID ID, void *Data);

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
		static bool _ValidateIP(int Fd, PhoenixEvent::EventID ID, void * Peer);
		 
		/**
		 * @addtogroup PhoenixEvent
		 * Server invokes the following PhoenixEvents:
		 * This object is private because only the Server can invoke
		 * these messages.
		 * Others can register to get the notifications.
		 *
		 * - "ServerReady"
		 * The server has started is ready to start processing.
		 *
		 * - "ServerNewClientConnection"
		 * Is sent each time a new connection is made to the server.
		 *
		 * - "ServerClientDisconnected:
		 * When the client connection is terminated by the client, or the server.
		 *
		 * - "ServerLoggingMessage"
		 * A new log message has been saved.
		 *
		 * - "ServerShuttingDown"
		 * The server is shutting down very soon.
		 *
		 * - "ServerErrorOnFd"
		 * A socket connected to a client has had some kind of error.
		 * It could be they hung up without logging out, network error.
		 * It is unknown why. Code that handles the connection should
		 * register for this PhoenixEvent to know that the connection is now dead.
		 */			 
		class _Event
			: public PhoenixEvent
		{
		public:

			/**
			 * @addtogroup PhoenixEvent
			 * This method calls DispatchCallbaks in the base class.
			 * Just a convenience wrapper that takes (char*) and not (void*).
			 *
			 * @param Fd The associated file descriptor, or (-1)
			 * if not associated with a file descriptor.
			 *
			 * @param EventID The ID of the event being invoked.
			 *
			 * @param Data Any extra data.
			 * Currently the Server class does not send extra data
			 * and this value is always nullptr.
			 *
			 * @return true for most calls.
			 * May return false for NewClientConnection, if the
			 * callback is telling the server to block this connection.
			 */
			static bool Invoke(int Fd, PhoenixEvent::EventID ID, const char * Msg = nullptr);

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
			 * This method calls DispatchCallbaks in the base class.
			 *
			 * @param Fd The associated file descriptor, or (-1)
			 * if not associated with a file descriptor.
			 *
			 * @param EventID The ID of the event being invoked.
			 *
			 * @param Data Any extra data.
			 * Currently the Server class does not send extra data
			 * and this value is always nullptr.
			 *
			 * @return true for most calls.
			 * May return false for NewClientConnection, if the
			 * callback is telling the server to block this connection.
			 */
			virtual bool Invoke(int Fd, EventID ID, void * Data = nullptr);

		};

		/**
		 * Cleanup on exit.
		 */
		static void _Cleanup();
		
		/**
		 * The number of file descriptors we are currently handling.
		 */
		static nfds_t	_NumberEntries;

		/**
		 * The array needed by poll(2).
		 */
		static struct pollfd * _Entries;

		/**
		 * Break out of poll(), then it reloads the list of file
		 * descriptors to use.
		 */
		static void	_ReloadPoll();

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
		 * The way we pause poll() is to write to its '_InterruptPollFd'
		 * The handler for that Fd waits.
		 */
		static int	_InterruptPollFd;

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

		static std::map<int,PeerInfo*> _Peers;
		
		/**
		 * Outbound data is XDR encoded, then stored in blobs
		 * in the _SendData map, by outbound socket file descriptor.
		 */
		static std::map<int, Iov*> _OutboundData;
		static std::mutex _OutboundDataMutex;
		static std::map<int, Iov*> _InboundData;
		static std::mutex _InboundDataMutex;

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
		static int64_t		_NetWrite(int Fd, Iov * IovData);
		
		/**
		 * The thread that we use to send out data.
		 */
		static void 	_Work();

	};
}
#endif // _RIVEREXPLORER_PHOENIX_POLLIO_HPP_
