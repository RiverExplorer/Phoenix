/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLORER_PHOENIX_IO_HPP_
#define _RIVEREXPLORER_PHOENIX_IO_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "Commands.hpp"
#include "PacketBody.hpp"
#include "CmdNotSupported.hpp"
#include "Iov.hpp"
#else
#include <RiverExplorer/Phoenix/Commands.hpp>
#include <RiverExplorer/Phoenix/PacketBody.hpp>
#include <RiverExplorer/Phoenix/CmdNotSupported.hpp>
#include <RiverExplorer/Phoenix/Iov.hpp>
#endif

#include <map>
#include <mutex>
#include <thread>
#include <list>
#include <deque>

namespace RiverExplorer::Phoenix
{
	/**
	 * @class IO IO.hpp <RiverExplorer/Phoenix/IO.hpp>
	 *
	 * Perform the I/O.
	 */
	class IO
	{
	public:

		/**
		 * IO - Default constructor.
		 */
		IO();
		
		/**
		 * Close down an incomming connection.
		 *
		 * @param Fd The file descriptor to close down.
		 *
		 * @return true when Fd was found and shutdown.
		 */
		static bool RemoveConnection(int Fd);
		
		/**
		 * Each active connection has one of these
		 */
		struct ActiveConnection
		{
			/**
			 *  The R/W file descriptor to the other endpoint.
			 */
			int		Fd;

			/**
			 * If the connection has been authenticated.
			 */
			bool	Authenticated;
		};

		/**                                                                        
		 * Data ready to send.                                                     
		 */
		struct SendPacket
		{
			/**
			 *  Which connection to send it to.
			 */
			ActiveConnection * Connection;

			/**
			 * The data to send.
			 */
			Iov									DataToSend;

			/**
			 * SendPacket - Default Constructor.
			 */
			SendPacket();

			/**
			 * SendPacket - Destructor.
			 */
			~SendPacket();
		};

		/**
		 * Queue packet for outbound transmission.
		 * This version can handle mmap() and non mmap() iovsections.
		 * Each mmap() iovec is munmap(), and each non-mmap() iovec
		 * is deleted after being sent.
		 *
		 * @param Fd The file descriptor to send to.
		 *
		 * @param Blob The XDR encoded blob.
		 *
		 * @param BlobSize The sizeof the XDR encoded Blob.
		 *
		 * @note
		 * Use this one for small packets, it copies the data before
		 * sending.
		 */
		static void QOutbound(int Fd, uint8_t * Blob, uint32_t BlobSize);

		/**
		 * Queue packet for outbound transmission.
		 * This version can handle mmap() and non mmap() iovsections.
		 * Each mmap() iovec is munmap(), and each non-mmap() iovec
		 * is deleted after being sent.
		 *
		 * @param Fd The file descriptor to send to.
		 *
		 * @param Vecs An array of VecCount iovec objects to send.
		 *
		 * @param VecCount The number of iovec objects.
		 *
		 * @note
		 * Use this one for large packets, it does not copy the
		 * data before sending, the caller must clean up.
		 * The data can not be cleaned up until the packet is transmitted.
		 */
		static void QOutbound(int Fd, iovec * Vecs, uint32_t VecCount);

	private:

		/**
		 * This thread listens for incoming data.
		 * Started as a thread
		 *
		 * Pre-processes it, then calls _Dispatch().
		 */
		static void				_Listener();
		
		/**
		 * A list of commands we issued, and do not have a reply yet.
		 * Commands that do not get a reply, are not added.
		 */
		static std::map<SEQ_t, Command*> _CommandsWeIssued;

		/**
		 * Mutex lock for CommandsWeIssued.
		 * Keeps only one hand in the pot at a time.
		 */
		static std::mutex _CommandsWeIssuedMutex;
	
		/**
		 * This is the thread that run listens for active data
		 * and sends it to Dispatch().
		 */
		static std::thread	_ListenerThread;

		/**
		 * List of active connections to listen to.
		 */
		static std::map<int,ActiveConnection*> _ActiveConnections;

		/**
		 * A mutex lock for __ActiveConnections.
		 * Keeps only one hand in the pot at a time.
		 */
		static std::mutex _ActiveConnectionsMutex;

		/**
		 * Cleanup for exit.
		 */
		static void _Cleanup();
		
	};
}

#endif // _RIVEREXPLORER_PHOENIX_IO_HPP_
