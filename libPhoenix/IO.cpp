/**                                                                             * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

/**
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
#include "Server.hpp"
#include "Iov.hpp"
#include "Commands.hpp"

#ifndef W64
#include <sys/socket.h>
#include <sys/mman.h>
#else
/** @todo Add W64 includes */
#endif

#include <semaphore.h>

namespace RiverExplorer::Phoenix
{
	
	/**
	 * A list of commands we issued, and do not have a reply yet.
	 * Commands that do not get a reply, are not added.
	 */
	std::map<CommandID, CmdPacket*> IO::_CommandsWeIssued;

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

	IO::IO()
	{
		FILE * Fp = Server::GetLogFp();
		
		// Start these if they are not started.
		//
		if (!ThreadsInited) {
			_ListenerThread = std::thread(IO::_Listener);
			_ListenerThread.detach();
#ifdef DEBUG
			fprintf(Fp, "_ListenerThread thread started\n");
#endif
		}

		atexit(_Cleanup);

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
			shutdown(Fd, SHUT_RDWR);
			close(Fd);

			_ActiveConnections.erase(It);
			Results = true;
		}
		_ActiveConnectionsMutex.unlock();

		return(Results);
		
	}

	void
	IO::_Listener()
	{
		// ...

		return;
	}

	// This version of QOutbound copies the data.
	//
	void
	IO::QOutbound(int Fd,
								CommandID ID,
								Command_e Cmd,
								uint8_t * Blob,
								uint64_t BlobLength)
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
			// length is easy to calculate.
			//
			// sizeof(ID) + sizeof(Cmd) + BlobLength as uint32_t.
			//
			uint64_t HeaderLength = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
			uint64_t TotalLength = HeaderLength + BlobLength;
			uint8_t * Header = new uint8_t[HeaderLength];
			XDR Xdr;

			xdrmem_create(&Xdr, (char*)Header, (u_int)HeaderLength, XDR_ENCODE);

			if (xdr_CommandID(&Xdr, &ID)) {
				if (xdr_Command_e(&Xdr, &Cmd)) {
					// We do not need to XDR encode or copy the opaque data.
					// So we send its length, then the opaque data.
					// The blob is already ready.
					//
					if (xdr_uint64_t(&Xdr, &BlobLength)) {
						Packet->DataToSend.Add(TotalLength);
						Packet->DataToSend.Add(Header, HeaderLength, false);
						Packet->DataToSend.Add(Blob, BlobLength, false);
						
						Server::Send(Packet);
					} else {
						PhoenixEvent::InvokeMessage(Fd,
																				PhoenixEvent::LogError_s,
																				"IO.cpp:QOutbound:Unable to xdr_uint32_t()");
						delete Packet;
					}
				} else {
					PhoenixEvent::InvokeMessage(Fd,
																			PhoenixEvent::LogError_s,
																			"IO.cpp:QOutbound:Unable to xdr_Command_e()");
					delete Packet;
				}
			} else {
				PhoenixEvent::InvokeMessage(Fd,
																		PhoenixEvent::LogError_s,
																		"IO.cpp:QOutbound:Unable to xdr_CommandID()");
				delete Packet;
			}
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
}

