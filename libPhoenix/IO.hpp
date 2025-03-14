/**
 * Project: Phoenix
 *
 * @file IO.hpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer.
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

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509v3.h>

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
		 *
		 * @param IsServer When true, we are running on the
		 * server side. When false, we are running on the client side.
		 */
		IO(bool IsServer);

		/**
		 * IO - Destructor
		 */
		~IO();
		
		/**
		 * Close down an incoming connection.
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
			 * ActiveConnection - Default Constructor.
			 */
			ActiveConnection();

			/**
			 * ActiveConnection - Destructor.
			 */
			~ActiveConnection();
			
			/**
			 *  The R/W file descriptor to the other endpoint.
			 */
			int		Fd;

			/**
			 * The XDR Write handle.
			 * @note May be nullptr when not used.
			 */
			XDR	*	WriteXdrs;
			
			/**
			 * The XDR Read handle.
			 * @note May be nullptr when not used.
			 */
			XDR	*	ReadXdrs;
			
			/**
			 * If the connection has been authenticated.
			 */
			bool	Authenticated;

			/**
			 * Data received and not processed.
			 */
			Iov			Received;

			/**
			 * SSL Context.
			 * How the connection is established.
			 * What kind of connection to establish.
			 */
			static SSL_CTX	*	Ctx;

			/**
			 * The SSL handle.
			 */
			SSL		*	Ssl;

			/**
			 * The SSL read from buffer.
			 */
			BIO		*	SslReadBio;
			
			/**
			 * The SSL write to buffer.
			 */
			BIO		*	SslWriteBio;

			/**
			 * Open an XDR SSL connection.
			 * Open a TCP port, initialize SSL/TLS, and
			 * setup for XDR I/O.
			 *
			 * @param HostOrIp The host-name or IP address to connect to.
			 *
			 * @param Port The TCP port to connect to.
			 *
			 * @param PublicCertFile The public PEM certificate file.
			 *
			 * @param PrivteKeyFile The private PEM certificate file.
			 *
			 * @return true when everything worked.
			 * Else returns false.
			 */
			bool Open(const std::string & HostOrIp,
								uint16_t Port,
								const std::string & PublicCertFile,
								const std::string & PrivateKeyFile);

			/**
			 * Get the connected to host name.
			 *
			 * @return The connected to host name.
			 * Or nullptr when there is no connection.
			 */
			const char * HostName() const;

		private:

			/**
			 * Clean up data on delete.
			 */
			void _Cleanup();

			/**
			 * The host we are connected to.
			 */
			const char * _HostOrIp;
			
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
		 * addtogroup ServerInternals
		 *
		 * This thread listens for incoming data.
		 * Started as a thread
		 *
		 * It places what is available in the Received
		 * Iov object once every WaitSeconds, when
		 * data is available to read.
		 *
		 * @note
		 * This is used by the server side, not the client side.
		 */
		static void				_ServerListener();

		/**
		 * addtogroup ClientInternals
		 *
		 * This thread listens for incoming data.
		 * Started as a thread
		 *
		 * It places what is available in the Received
		 * Iov object once every WaitSeconds, when
		 * data is available to read.
		 *
		 * @param WaitSeconds How often in seconds between
		 * checking for incomming data.
		 *
		 * @note
		 * This is used by the client side, not the server side.
		 */
		static void				_ClientListener(uint32_t WaitSeconds);

		/**
		 * addtogroup ClientInternals
		 *
		 * Get whatever we can get. ...
		 *
		 * @param Con The ActiveConnection to read from.
		 *
		 * @param Buffer A pointer that will be allocated and
		 * filled in with the results.
		 *
		 * @note
		 * The old contents of Buffer must be preserved or deleted if needed,
		 * as this method will destroy the pointer and will not
		 * copy or delete the contents.
		 *
		 * @return The number of octets received.
		 * On error Buffer will be set to nullptr.
		 * On zero returned, Buffer will be set to nullptr. Nothing
		 * was readable without blocking.
		 * On (<0) call SSL_get_error() with the
		 * results to determine the error.
		 */
		int	_ClientRead(ActiveConnection * Con, uint8_t *& Buffer);

		/**
		 * addtogroup ClientInternals
		 *
		 * Write to the server.
		 *
		 * @param Con The ActiveConnection to write to.
		 *
		 * @param Buffer What to write.
		 *
		 * @param Length How many octets to write.
		 *
		 * @return > 0, the number of octets written.
		 * <= 0 Caller must call SSL_get_error() with the
		 * results to determine the error.
		 * On zero (0) nothing was written. Could be because
		 * it would block.
		 */
		int _ClientWrite(ActiveConnection * Con, uint8_t * Buffer, uint32_t Length);

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
		 * A mutex lock for _ActiveConnections.
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
