
#ifndef _RIVEREXPLORER_PHOENIX_IO_HPP_
#define _RIVEREXPLORER_PHOENIX_IO_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "Commands.hpp"
#include "CmdNotSupported.hpp"
#else
#include <RiverExplorer/Phoenix/Commands.hpp>
#include <RiverExplorer/Phoenix/CmdNotSupported.hpp>
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
			 * What to send.
			 * When Vecs is nullptr or VecCount is zero, this is everyting to send.
			 * When Vecs is not nullptr, only send the data from what is in Vecs.
			 */
			CmdPacket		*	WhatToSend;

			/**
			 * List of iovec sutable for writev().
			 */
			iovec				*	Vecs;

			/**
			 * List of iovec that are mmap() entries.
			 * One for each entry in Vecs.
			 */
			bool				*	IsMmap;
			
			/**
			 * Number of entries in Vecs.
			 */
			uint64_t			VecCount;

			/**
			 * SendPacket - Default Constructor.
			 */
			SendPacket();

			/**
			 * SendPacket - Destructor.
			 * When Vecs is non-nullptr, this destructor deletes each
			 * iovec entry contents, then deletes the array.
			 * For each Vecs entry that has an associated IsMmap entry
			 * that is true, the data is unmap().
			 */
			~SendPacket();
		};

		/**
		 * Queue packet for outbound transmission.
		 * The version coped Blob, so the caller
		 * must delete their own data.
		 *
		 * @param Fd The file descriptor to send to.
		 *
		 * @param ID The command ID to use.
		 *
		 * @param Cmd The command to send.
		 *
		 * @param Blob The already XDR encoded blob to send.
		 *
		 * @param BlobLenght The number of octets in Blob.
		 */
		static void QOutbound(int Fd,
													CommandID ID,
													Command_e Cmd,
													uint8_t * Blob,
													uint64_t BlobLength);

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
		 * @param IsMmapped An array of booleans, one for each entry
		 * in Vecs. When true, the iovec entry is mmap()'ed. When false
		 * it is allocaed.
		 *
		 * @param VecCount The number of iovec objects.
		 */
		static void QOutbound(int Fd, iovec * Vecs, uint64_t VecCount);

		/**
		 * Queue packet for outbound transmission.
		 * This version can handle mmap() and non mmap() iovsections.
		 * Each mmap() iovec is munmap(), and each non-mmap() iovec
		 * is deleted after being sent.
		 *
		 * @param Fd The file descriptor to send to.
		 *
		 * @param ID The command ID to use.
		 *
		 * @param Cmd The command to send.
		 *
		 * @param Vecs An array of VecCount iovec objects to send.
		 *
		 * @param IsMmapped An array of booleans, one for each entry
		 * in Vecs. When true, the iovec entry is mmap()'ed. When false
		 * it is allocaed.
		 *
		 * @param VecCount The number of iovec objects.
		 */
		static void QOutbound(int Fd,
													CommandID ID,
													Command_e Cmd,
													iovec * Vecs,
													uint64_t VecCount);

	private:

		/**
		 * This thread listens for incoming data.
		 * Started as a thread
		 *
		 * Pre-processes it, then calls _Dispatch().
		 */
		static void				_Listener();

		/**
		 * This thread takes prepared packets and sends them out.
		 * Started as a thread
		 */
		static void				_Dispatcher();
		
		/**
		 * A list of commands we issued, and do not have a reply yet.
		 * Commands that do not get a reply, are not added.
		 */
		static std::map<CommandID, CmdPacket*> _CommandsWeIssued;

		/**
		 * Mutex lock for CommandsWeIssued.
		 * Keeps only one hand in the pot at a time.
		 */
		static std::mutex _CommandsWeIssuedMutex;

		/**
		 * A queue of packets that are ready to be sent.
		 */
		static std::deque<SendPacket*> _PacketsToSend;

		/**
		 * A mutex lock for PacketsToSend.
		 * Keeps only one hand in the pot at a time.
		 */
		static std::mutex _PacketsToSendMutex;

		/**
		 * This is the thread that run to dispatch received CmdPacket objects.
		 * At this time, it is a singleton.
		 */
		static std::thread 	_DispatchThread;
	
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
	
	};
}

#endif // _RIVEREXPLORER_PHOENIX_IO_HPP_
