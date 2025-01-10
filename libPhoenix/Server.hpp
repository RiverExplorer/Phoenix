
#ifndef _RIVEREXPLORER_PHOENIX_POLLIO_HPP_
#define _RIVEREXPLORER_PHOENIX_POLLIO_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "IO.hpp"
#else
#include <RiverExplorer/Phoenix/IO.hpp>
#endif

#ifndef W64
#include <poll.h>
#include <sys/uio.h> // iovec
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
	 */
	class Server
	{
	public:

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
		 * The signleton.
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
		 * Take a single packet and process it, and send it.
		 *
		 * It is possible on a busy network to not be able to send
		 * large packets without blocking. This method regulates the
		 * sending to each endpoint, or waits (yields), so that it other
		 * unrealated operations can proceed without blocking for the
		 * hardware to be ready.
		 *
		 * This method takes the incomming data, sorts by file descriptor,
		 * and adds to its iovec (list) of things to send.
		 *
		 * @param Pkt The packet to send. Which include the file descriptor
		 * and authentication status.
		 *
		 * @return True when the packet was accepted for processing.
		 * Return false if something in the Pkt is not acceptable.
		 * This might mean that the file descriptor is closed, or missing
		 * or invalid.
		 */
		bool	Send(IO::SendPacket * Pkt);

		/**
		 * Peer addreses.
		 */
		struct PeerInfo
		{
			sockaddr  Addr;
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
		
	private:

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
		 * We ues FILE and not (int) so we can use various printf() calls.
		 */
		FILE			*	_LogFp;
		
		/**
		 * This is the file descriptor that listen() waits on.
		 */
		static int	_ListenFd;
		static sockaddr_in6	_ServerAddress; // Works with IPv4 and IPv6
		static int	_Backlog;

		static std::map<int,PeerInfo*> _Peers;
		
		/**
		 * List of data for each file descriptor.
		 * <fd, iovec list>
		 */
		std::map<int, std::deque<iovec*>*> _SendData;
		std::mutex _SendDataMutex;

		/**
		 * Data can come in in partial packets.
		 *
		 * Each packed is proceeded by its total length of what follows.
		 * This data structure keeps in incomming data in one place for
		 * each connected enpoint, until all of the packet arrives.
		 */
		struct _Partial
		{
			int Fd;
			uint64_t TotalSize;
			std::vector<iovec*> Collected;
			
		};

		/**
		 * One entry for each file descriptor that is open.
		 * The entries are created on the first read from the network.
		 */
		static std::map<int, _Partial*> _PartialPackets;
		
		/**
		 * The thread that we use to send out data.
		 */
		static void 	_Work();

	};
}
#endif // _RIVEREXPLORER_PHOENIX_POLLIO_HPP_
