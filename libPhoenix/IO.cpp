
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
#include <Register.hpp>

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
	 * A semaphore to signal something was put into _PacketsToSend.
	 */
	static sem_t	_PacketToSendSem;
	
	/**
	 * A queue of packets that are ready to be sent.
	 */
	std::deque<IO::SendPacket*> IO::_PacketsToSend;

	/**
	 * A mutex lock for PacketsToSend.
	 * Keeps only one hand in the pot at a time.
	 */
	std::mutex IO::_PacketsToSendMutex;

	/**
	 * Set true when the dispatch and listen threads have been started.
	 */
	static bool ThreadsInited = false;
	
	/**
	 * This is the thread that run to dispatch received CmdPacket objects.
	 * At this time, it is a singleton.
	 */
	std::thread	IO::_DispatchThread;
	
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
		// Start these if they are not started.
		//
		if (!ThreadsInited) {
			_DispatchThread = std::thread(IO::_Dispatcher);
#ifdef DEBUG
		fprintf(stdout, "_DispatchThread thread started\n");
#endif
		
			_ListenerThread = std::thread(IO::_Listener);
#ifdef DEBUG
		fprintf(stdout, "_ListenerThread thread started\n");
#endif
		}

		return;
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

	// This method runs in its own thread.
	// 
	void
	IO::_Dispatcher()
	{
		// Take packets out of _PacketsToSend, and send them.
		//
		SendPacket	* Pkt = nullptr;

		do {
			sem_wait(&_PacketToSendSem);
			
			_PacketsToSendMutex.lock();
			if (_PacketsToSend.size() > 0) {
				Pkt = _PacketsToSend.front();
				_PacketsToSend.pop_front();

				
			}
			_PacketsToSend.push_back(Pkt);
			_PacketsToSendMutex.unlock();
		} while (1);
		
		return;		
	}

	void
	IO::_Listener()
	{
		// ...

		return;
	}

	// This version ov QOutbound copies the data.
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
			SendPacket * Pkt = new SendPacket();

			Pkt->Connection = AIt->second;

			SendPacket  * Packet = new SendPacket();
			Packet->WhatToSend = new CmdPacket();
			Packet->WhatToSend->Cmd = Cmd;
			Packet->WhatToSend->ID = ID;
			Packet->WhatToSend->Data.Data = new char[BlobLength];
			memcpy(Packet->WhatToSend->Data.Data, Blob, BlobLength);
			Packet->WhatToSend->Data.Len = (uint32_t)BlobLength;
				
			_PacketsToSendMutex.lock();
			_PacketsToSend.push_back(Packet);
			sem_post(&_PacketToSendSem);
			_PacketsToSendMutex.unlock();
		}

		return;
	}

	IO::SendPacket::SendPacket()
	{
		Connection = nullptr;
		WhatToSend = nullptr;
		Vecs = nullptr;
		IsMmap = nullptr;
		VecCount = 0;

		return;
	}

	IO::SendPacket::~SendPacket()
	{
		if (Connection != nullptr) {
			delete Connection;
			Connection = nullptr;
		}

		if (WhatToSend != nullptr) {
			delete WhatToSend;
			WhatToSend = nullptr;
		}

		if (Vecs != nullptr) {
			for (uint64_t i = 0; i < VecCount; i++) {
				if (IsMmap[i]) {
					munmap(Vecs[i].iov_base, Vecs[i].iov_len);
				} else {
					delete[] (uint8_t*)Vecs[i].iov_base;
				}
				Vecs[i].iov_base = nullptr;
				Vecs[i].iov_len = 0;
			}
			delete[] Vecs;
			Vecs = nullptr;
			delete[] IsMmap;
			IsMmap = nullptr;
		}
		VecCount = 0;

		return;
	}
}

