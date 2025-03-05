/**
 * Project: Phoenix
 *
 * @file IO.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 *
 * -This code takes received packets and dispatches them
 * The any registred function for the command.
 *
 * -Processes any CmdNotSupported packets.
 *
 * -Listens for incomming data.
 *
 * -Sends out outgoing data.
 */
#include "Register.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Iov.hpp"
#include "Commands.hpp"
#include "HostName.hpp"

#ifndef W64
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#else
/** @todo Add W64 includes */
#endif

#include <semaphore.h>

namespace RiverExplorer::Phoenix
{

	SSL_CTX	*	IO::ActiveConnection::Ctx = nullptr;
	
	/**
	 * A list of commands we issued, and do not have a reply yet.
	 * Commands that do not get a reply, are not added.
	 */
	std::map<SEQ_t, Command*> IO::_CommandsWeIssued;

	/**
	 * Mutex lock for CommandsWeIssued.
	 * Keeps only one hand in the pot at a time.
	 */
	std::mutex IO::_CommandsWeIssuedMutex;

	/**
	 * Set true when the dispatch and listen threads have been started.
	 */
	static bool ThreadsInited = false;
	
	/**
	 * This is the thread that run listens for active data
	 * and sends it to Dispatch().
	 */
	std::thread IO::_ListenerThread;

	/**
	 * List of active connections to listen to.
	 */
	std::map<int,IO::ActiveConnection*> IO::_ActiveConnections;

	/**
	 * A mutex lock for __ActiveConnections.
	 * Keeps only one hand in the pot at a time.
	 */
	std::mutex IO::_ActiveConnectionsMutex;

	IO::IO(bool IsServer)
	{
		// Start these if they are not started.
		//
		if (!ThreadsInited) {
			/**
			 * @todo Change '5' to configurable variable.
			 */
			if (IsServer) {
				_ListenerThread = std::thread(IO::_ServerListener);
			} else {
				_ListenerThread = std::thread(IO::_ClientListener, 5);
			}
			_ListenerThread.detach();
#ifdef DEBUG
			Log::PrintInformation("_ListenerThread thread started\n");
#endif
			atexit(_Cleanup);
		}

		return;
	}

	IO::~IO()
	{
		_Cleanup();

		return;
	}
	
	void
	IO::_Cleanup()
	{

	}
	
	bool
	IO::RemoveConnection(int Fd)
	{
		bool Results = false;
		
		std::map<int,ActiveConnection*>::iterator  It;

		_ActiveConnectionsMutex.lock();
		It = _ActiveConnections.find(Fd);

		if (It != _ActiveConnections.end()) {
			_Cleanup();

			_ActiveConnections.erase(It);
			Results = true;
		}
		_ActiveConnectionsMutex.unlock();

		return(Results);
		
	}

	// This is run in its own thread on the server.
	//
	void
	IO::_ServerListener()
	{
		/**@todo _ServerListener*/

		return;
	}
	
	// This is run in its own thread on the client.
	//
	// I don't use poll() so that I don't have to use any method to
	// break out of poll() in order to update the list.
	// As this is the client side code, Checking at LookInterval
	// will suffice.
	//
	void
	IO::_ClientListener(uint32_t LookInterval)
	{
		// Listen for input, then add it to the incomming queue.
		//
		std::map<int,ActiveConnection*>::iterator  AIt;

		ActiveConnection * Connection;
		
		int CanRead;
		uint8_t * Buf;
		
		do {
			sleep(LookInterval);

			_ActiveConnectionsMutex.lock();
			for (AIt = _ActiveConnections.begin()
						 ; AIt != _ActiveConnections.end()
						 ; AIt++) {

				Connection = AIt->second;
				
				if (ioctl(Connection->Fd, FIONREAD, &CanRead) == 0) {
					if (CanRead > 0) {
						Buf = new uint8_t[CanRead];

						read(Connection->Fd, Buf, CanRead);
						Connection->Received.Add(Buf, CanRead, false);
					}
				}
			}
			_ActiveConnectionsMutex.unlock();
			
		} while (1);

		return;
	}

	// This version of QOutbound copies the data.
	// The data is already in XDR format, ready to send.
	//
	void
	IO::QOutbound(int Fd,
								uint8_t * Blob,
								uint32_t BlobLength)
	{
		std::map<int,ActiveConnection*>::iterator  AIt;

		AIt = _ActiveConnections.find(Fd);
		if (AIt != _ActiveConnections.end()) {

			SendPacket  * Packet = new SendPacket();

			Packet->Connection = AIt->second;

			// The first Iov blob, is the total length
			// of the XDR data.
			//
			// With this version of QOutbound(), the resut
			// length is easy to calculate. It is BlobLength.
			//
			uint32_t Len = sizeof(uint32_t) + BlobLength;
			uint8_t * RawData = new uint8_t[Len];

			Packet->DataToSend.Add(htonl(sizeof(uint64_t)));

			// This version copies data.
			//
			Packet->DataToSend.Add(Blob, BlobLength, false);
						
			Server::Send(Packet);
		}

		return;
	}

	IO::SendPacket::SendPacket()
	{
		Connection = nullptr;

		return;
	}

	IO::SendPacket::~SendPacket()
	{
		if (Connection != nullptr) {
			delete Connection;
			Connection = nullptr;
		}

		return;
	}

	IO::ActiveConnection::ActiveConnection()
	{
		Fd = -1;
		WriteXdrs = nullptr;
		ReadXdrs = nullptr;
		Authenticated = false;
		Ssl = nullptr;
		SslReadBio = nullptr;
		SslWriteBio = nullptr;

		return;
	}
	
	IO::ActiveConnection::~ActiveConnection()
	{
		_Cleanup();

		return;
	}

	void
	IO::ActiveConnection::_Cleanup()
	{
		if (WriteXdrs != nullptr) {
			xdr_destroy(WriteXdrs);
			WriteXdrs = nullptr;
		}
		
		if (ReadXdrs != nullptr) {
			xdr_destroy(ReadXdrs);
			ReadXdrs = nullptr;
		}
		
		if (Ssl != nullptr) {
			SSL_free(Ssl); // Also frees the associated BIO.
			Ssl = nullptr;
		}
		SslReadBio = nullptr;
		SslWriteBio = nullptr;

		if (Fd > -1) {
			shutdown(Fd, SHUT_RDWR);
			close(Fd);
			Fd = -1;
		}

		Authenticated = false;

		return;
	}
	
	bool
	IO::ActiveConnection::Open(const std::string & HostOrIp,
														 uint16_t Port,
														 const std::string & PublicCertFile,
														 const std::string & PrivateKeyFile)
	{
		bool Results = false;

		int Okay = 1;
		
		if (Fd > -1) {
			_Cleanup();
		}

		struct addrinfo * Address = nullptr;
		
		Fd = HostName::ConnectTcp(HostOrIp, Port, Address);

		if (Fd > -1) {
			fcntl(Fd, F_SETFL, SOCK_NONBLOCK);
			SSL_set_fd(Ssl, Fd);
			
			if (Ctx == nullptr) {
				SSL_library_init();
				SSLeay_add_ssl_algorithms();
				SSL_load_error_strings();
				
				Ctx = SSL_CTX_new(TLS_method());
				Ssl = SSL_new(Ctx);
				
				Okay = SSL_CTX_use_certificate_file(Ctx,
																						PublicCertFile.c_str(),
																						SSL_FILETYPE_PEM);

				if (Okay == 1) {
					Okay = SSL_CTX_use_PrivateKey_file(Ctx,
																					 PrivateKeyFile.c_str(),
																					 SSL_FILETYPE_PEM);

					if (Okay == 1) {
						Okay = SSL_CTX_check_private_key(Ctx);

						if (Okay == 1) {
							SSL_CTX_set_options(Ctx,
																	SSL_OP_ALL|SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3);

							// Attempt to connect.
							// 'while 1' because it is a non-blocking socket.
							//
							fd_set Fds;
							
							FD_ZERO(&Fds);
							FD_SET(Fd, &Fds);
							
							while (1) {

								Okay = SSL_connect(Ssl);
								if (Okay == 1) {
									//
									// Connected.
									//
									break;
								}

								int Error = SSL_get_error(Ssl, Okay);

								if (Error == SSL_ERROR_WANT_READ) {
									Error = select(Fd + 1, &Fds, nullptr, nullptr, nullptr);

									if (Error == -1) {
										// Failed.
										//
										Okay = 0;
										break;
									}

								} else if (Error == SSL_ERROR_WANT_WRITE) {
									Error = select(Fd + 1, &Fds, nullptr, nullptr, nullptr);

									if (Error == -1) {
										// Failed.
										//
										Okay = 0;
										break;
									}
										
								} else {
									// Failed.
									//
									Okay = 0;
									break;
								}
							}
						}
					}
				}
			}
		}

		if (Okay == 1) {
			Log::PrintInformation("SSL connection using %s", SSL_get_cipher (Ssl));
		}
		
		return(Results);
	}

	int
	IO::_ClientRead(ActiveConnection * Con, uint8_t *& Buffer)
	{
		int	Results = -1;

		int CanRead = SSL_pending(Con->Ssl);

		if (CanRead == 0) {
			Results = 0;
			Buffer = nullptr;

		} else {
			Buffer = new uint8_t[CanRead];

			Results = SSL_read(Con->Ssl, Buffer, CanRead);

			if (Results < 0) {
				int Error = SSL_get_error(Con->Ssl, Results);

				if (Error == SSL_ERROR_WANT_READ) {
					delete Buffer;
					Buffer = nullptr;
					Results = 0;
				
				} else if (Error == SSL_ERROR_WANT_WRITE) {
					delete Buffer;
					Buffer = nullptr;
					Results = 0;
				}
			}
		}
		
		return(Results);
	}

	int
	IO::_ClientWrite(ActiveConnection * Con,
									 uint8_t * Buffer,
									 uint32_t Length)
	{
		int Results = SSL_write(Con->Ssl, Buffer, Length);

		if (Results < 0) {

			int Error = SSL_get_error(Con->Ssl, Results);

			if (Error == SSL_ERROR_WANT_WRITE) {
				Results = 0;

			} else if (Error == SSL_ERROR_WANT_READ) {
				Results = 0;
			}
		}

		return(Results);
	}
	
}
