
// Get poll(): POLLRDHUP
//

#include "Register.hpp"
#include "Server.hpp"

#include <stdio.h>
#ifndef W64
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
/**@todo W64*/
#endif

#include <semaphore.h>

namespace RiverExplorer::Phoenix
{

	/**
	 * The singleton object for PollIO.
	 */
	std::map<int,Server::PeerInfo*>		Server::_Peers;
	std::map<int, Server::_Partial*>	Server::_PartialPackets;
	std::set<int>						Server::_RemoveFds;
	std::set<int>						Server::_NewFds;
	Server								*	Server::Instance = nullptr;
	nfds_t									Server::_NumberEntries = 0;
	struct pollfd					*	Server::_Entries = nullptr;
	int											Server::_InterruptPollFd;
	int											Server::_ListenFd;
	struct sockaddr_in6			Server::_ServerAddress;
	int											Server::_Backlog = 10;
	
	std::thread							_WorkThread;
	sem_t										_WorkThreadReady;

	static char							Fifo[PATH_MAX];

	static IO						*		IOInstance = nullptr;
	
	static void Cleanup()
	{
		unlink(Fifo);

		return;
	}
	
	Server::Server()
	{
		if (Instance == nullptr) {
			Instance = this;

			// Create the loopback connection
			// to read from to wakup poll() to re-read its configration data.
			//
			pid_t OurPid = getpid();
			snprintf(Fifo, sizeof(Fifo), "%s/NamedPipe/PollLoopback.%u", INSTALL_DIR, OurPid);

			// We don't use them.
			//
			fclose(stdin);
			fclose(stdout);

			// Where we write logs and messages.
			//
#ifdef DEBUG			
			_LogFp = stderr;
#else
			fclose(stderr);


			_LogFp = fopen("Phoenix.log", "w");
#endif			
			
			if (access(Fifo, R_OK|W_OK) != F_OK) {
				unlink(Fifo); // In case it exsts and has incorrect permissions.
				mkfifo(Fifo, 0600);
				atexit(Cleanup);
			}
				
			_InterruptPollFd = open(Fifo, O_RDWR);
			sem_init(&_WorkThreadReady, 0, 0);
		}
		// else - ignore us!
		//

		return;
	}

	Server::~Server()
	{
		/*@todo - empty for now, nothing to do*/
	}

	// This is run as a thread.
	//
	std::thread *
	Server::Start(uint16_t Port, const char * Device)
	{
		std::thread * Results = nullptr;
		
		char On = 1;

		IOInstance = new IO();
		
		memset(&_ServerAddress, 0, sizeof(_ServerAddress));

		_ListenFd = socket(AF_INET6, SOCK_STREAM, 0);
		setsockopt(_ListenFd, SOL_SOCKET, SO_REUSEADDR, &On, sizeof(On));

		_ServerAddress.sin6_family = AF_INET6;
		_ServerAddress.sin6_addr = in6addr_any;
		_ServerAddress.sin6_port = htons(Port);

		if (Device != nullptr && Device[0] != '\0') {
			struct ifreq ifr;
			strncpy(ifr.ifr_name, Device, IFNAMSIZ - 1 );

			if (setsockopt(_ListenFd,
										 SOL_SOCKET,
										 SO_BINDTODEVICE,
										 &ifr,
										 sizeof(ifr) < 0)) {
				// ERROR.
			}
		}
		bind(_ListenFd, (struct sockaddr*)&_ServerAddress, sizeof(_ServerAddress));
		_NewFds.insert(_InterruptPollFd);
		_NewFds.insert(_ListenFd);
		_ReloadPoll();
		_WorkThread = std::thread(_Work);
		Results = &_WorkThread;
#ifdef DEBUG
		fprintf(_LogFp, "DEBUG:_Work thread started\n");
#endif
		sem_wait(&_WorkThreadReady);

		return(Results);
	}

	bool
	Server::Send(IO::SendPacket * Pkt)
	{
		bool Results = false;

		if (Pkt != nullptr) {
			std::map<int, std::deque<iovec*>*>::iterator It;
			std::deque<iovec*>	*	Q = nullptr;
		
			_SendDataMutex.lock();

			It = _SendData.find(Pkt->Connection->Fd);

			if (It == _SendData.end()) {
				Q = new std::deque<iovec*>();

				// Inturrupt poll(), update the list.
				//
				_NewFds.insert(Pkt->Connection->Fd);
				_ReloadPoll();
				//
			} else {
				Q = It->second;
			}

			iovec * Iov = new iovec();
			Iov->iov_base = &Pkt->WhatToSend->ID;
			Iov->iov_len  = sizeof(Pkt->WhatToSend->ID);
			Q->push_back(Iov);
		
			Iov = new iovec();
			Iov->iov_base = &Pkt->WhatToSend->Cmd;
			Iov->iov_len  = sizeof(Pkt->WhatToSend->Cmd);
			Q->push_back(Iov);
		
			Iov = new iovec();
			Iov->iov_base = &Pkt->WhatToSend->Data.Data;
			Iov->iov_len  = sizeof(Pkt->WhatToSend->Data.Len);
			Q->push_back(Iov);
		
			_SendDataMutex.unlock();
		}
		
		return(Results);
	}
	
	void
	Server::RemoveFd(int Fd)
	{
		_RemoveFds.insert(Fd);
		_ReloadPoll();

		return;
	}

	// This runs as a thread.
	//
	void
	Server::_Work()
	{
		sem_post(&_WorkThreadReady);
		do {

			// Start listening for incomming connections.
			//
			listen(_ListenFd, _Backlog);

			// We always have at least 2 entriese to listen to.
			// Listen() and the interrupt FD.
			//
			_NumberEntries = 2;
			_Entries = new struct pollfd[1];

			// Add _ListenFd to the entries.
			//
			_Entries[0].fd = _ListenFd;
			_Entries[0].events = POLLIN;
			
			// Add _InterruptPollFd to the entries.
			// _InterruptPollFd is used by other threads
			// to break out of poll() and re-read the fd list.
			//
			_Entries[1].fd = _InterruptPollFd;
			_Entries[1].events = POLLIN;
			
			if (_NumberEntries > 0 && _Entries != nullptr) {
#ifdef DEBUG
				fprintf(_LogFp, "DEBUG:Poll() running with %lu entries\n", _NumberEntries);
#endif
				int Count = poll(_Entries, _NumberEntries, -1);

				if (Count > 0) {
					for (nfds_t Offset = 0; Offset < _NumberEntries; Offset++) {

 						int REvents = _Entries[Offset].revents;
						int RFd = _Entries[Offset].fd;
						
						if (REvents > 0) {
							// If the what to poll for list has changed ...
							//
							if (RFd == _InterruptPollFd) {
								char TossMe;

								// We got woke up by our wake-up file descriptor so that _Entries and _NumberEntries
								// could be updated. Then go back to work.
								//
								read(_InterruptPollFd, &TossMe, 1);

								nfds_t NewTotal = _NumberEntries + _NewFds.size() - _RemoveFds.size();
								struct pollfd * NewList = new pollfd[NewTotal];

								int											NewOffset = 0;
								nfds_t									OldOffset;
								std::set<int>::iterator	SetIt;
								std::map<int,PeerInfo*>::iterator	PeerIt;

								// Go through the current list, and remove the ones
								// that are to be removed.
								//
								for (OldOffset = 0; OldOffset < _NumberEntries; OldOffset++) {
									SetIt = _RemoveFds.find(_Entries[OldOffset].fd);
									if (SetIt != _RemoveFds.end()) {
										// Shutdown and close the socket.
										//
										shutdown(_Entries[OldOffset].fd, SHUT_RDWR);
										close(_Entries[OldOffset].fd);

										// Remove it from the remove list.
										//
										_RemoveFds.erase(SetIt);
										PeerIt = _Peers.find(_Entries[OldOffset].fd);
										if (PeerIt != _Peers.end()) {
											
											// Wipe out data (security).
											//
											memset(PeerIt->second, 0, sizeof(struct sockaddr));

											// Remove it from the list of peers.
											//
											delete PeerIt->second;
											PeerIt->second = nullptr; // (Security)
											_Peers.erase(PeerIt);
										}

									} else {
										// It has not been removed, copy it to the new poll() list.
										//
										NewList[NewOffset].fd = _Entries[OldOffset].fd;
										NewList[NewOffset].events = _Entries[OldOffset].events;
										NewList[NewOffset++].revents = _Entries[OldOffset].revents;
									}
								}
								// Add any new ones.
								//
								for (SetIt = _NewFds.begin(); SetIt != _NewFds.end(); SetIt++) {
									NewList[NewOffset].fd = *SetIt;
#ifdef _GNU_SOURCE
									NewList[NewOffset].events = POLLIN | POLLOUT | POLLRDHUP;
#else
									NewList[NewOffset].events = POLLIN | POLLOUT;
#endif
									NewList[NewOffset++].revents = 0;
								}
								_NewFds.clear(); // Empty the set of new file descriptors.
								
								memset(_Entries, 0, _NumberEntries);	// Invalidate the old data (security).
								delete[] _Entries;
								_Entries = NewList;
								_NumberEntries = NewTotal;

								// Go back to work.
								//
								continue;

							} else if (RFd == _ListenFd) {

								// New activity on the listen() file descriptor.
								//
								PeerInfo * Info = new PeerInfo();

								memset(Info, 0, sizeof(*Info));

								// Get the file descriptor for the new connection.
								//
								int NewFd = -1;

								NewFd = accept(_ListenFd, &Info->Addr, &Info->AddrLen);

								// Log the new connection.
								//
								char IP[INET6_ADDRSTRLEN];
								
								inet_ntop(Info->Addr.sa_family, IP, sizeof(IP));
								fprintf(_LogFp, "MSG:New connection, fd: %d/%s",
												NewFd,
												IP);

								// Save the peer info.
								// Add the new file descriptor to the add list.
								//
								_Peers.insert(std::make_pair(NewFd, Info));
								_NewFds.insert(NewFd);

								// Cause poll() to exit and look for any change or data.
								//
								char ToWrite = 'r';
								write(_InterruptPollFd, &ToWrite, 1); // Wake up poll().

								// Go back to work.
								//
								continue;
							}

							// Something to do...
							//
							// If we got here if there is a POLLIN or POLLOUT data.
							//
							for (nfds_t i = 0; i < _NumberEntries; i++) {

								if (_Entries[i].fd > -1) {

									// Is there data for us to read?
									//
									if ((_Entries[i].revents & POLLIN) > 0) {
										int Fd = _Entries[i].fd;
										
										// Get any partial packt data that might exist for
										// this file descriptor.
										//
										std::map<int,_Partial*>::iterator PIt;

										PIt = _PartialPackets.find(Fd);
										_Partial * Partial = nullptr;

										bool NewPacket = false;
										
										if (PIt == _PartialPackets.end()) {
											Partial = new _Partial();
											Partial->Fd = Fd;
											NewPacket = true;
											
										} else {
											Partial = PIt->second;
										}

										// How many octets can we read?
										//
										iovec *		Vec = new iovec();
										int				CanRead;
										
										ioctl(Fd, FIONREAD, &CanRead);

										// The first uint32_t of a new packet
										// is the total packet length.
										//
										if (NewPacket) {
											if (CanRead < (int)sizeof(uint32_t)) {
												// This is a new packet and we need all of
												// the first 4 octets, go back and wait.
												//
												continue;
											}
											CanRead -= (int)sizeof(uint32_t);

											uint32_t	NTotalLen;
											
											read(Fd, &NTotalLen, sizeof(uint32_t));
											Partial->TotalSize = ntohl(NTotalLen);
										}

										// Sill more? (We may have took some above.)
										//
										if (CanRead > 0) {
											// In addition to getting partial packets,
											// we might get multiple packets,
											// check here.
											//
											// Calculate the total collected so far. ...
											//
											size_t	Have = 0;
											for (const auto LenVec : Partial->Collected) {
												Have += LenVec->iov_len;
											}

											if ((Have + CanRead) > Partial->TotalSize) {
												// The rest belong to the next packet.
												//
												CanRead = (int)((size_t)Partial->TotalSize - (int)Have);
											}

											// Allocate what we can or should read.
											//
											Vec->iov_base = new uint8_t[CanRead];
											Vec->iov_len = CanRead;
											read(Fd, Vec->iov_base, CanRead);
											Partial->Collected.push_back(Vec);

											// If done with this packet.
											//
											if ((Have + CanRead) == Partial->TotalSize) {
												// We are done with this packet.
												//
												uint64_t DataLength = Partial->TotalSize
													- sizeof(CommandID)
													- sizeof(Command_e);
													
												uint8_t	*	Data = new uint8_t[DataLength];
												uint8_t	*	DataPtr = Data;
												
												std::vector<iovec*>::iterator vIt;
												iovec * CollectedVec;

												for (vIt = Partial->Collected.begin()
															 ; vIt != Partial->Collected.end()
															 ; vIt++) {
													CollectedVec = *vIt;

													// Move this partial data into the full packet
													// at DataPtr.
													//
													memcpy(DataPtr,
																 CollectedVec->iov_base,
																 CollectedVec->iov_len);
													
													DataPtr += CollectedVec->iov_len;

													// Clean out (security).
													//
													memset(CollectedVec->iov_base, 0, CollectedVec->iov_len);

													// Delete partial data.
													//
													delete CollectedVec;
												}

												// Data contans the incomming packet in XDR format.
												// DataLenght is the lenth of the packet.
												//
												CmdPacket Pkt;

												XDR Xdrs;
												
												xdrmem_create(&Xdrs,
																			(char*)Data,
																			(u_int)Partial->TotalSize,
																			XDR_DECODE);
												
												if (xdr_CmdPacket(&Xdrs, &Pkt)) {
													Register::Dispatch(Partial->Fd, &Pkt);
												}

												// Free internal data.
												//
												xdr_free((xdrproc_t)xdr_CmdPacket, Data);

												// Delete the XDR data.
												//
												delete[] Data;

												// Destroy the XDR object.
												//
												xdr_destroy(&Xdrs);
											}
										}
									}

									// Can we write data?
									//
									if ((_Entries[i].revents & POLLOUT) > 0) {
									}
									if (((_Entries[i].revents & POLLHUP) > 0)
											|| ((_Entries[i].revents & POLLRDHUP) > 0)) {
#ifdef DEBUG
										fprintf(_LogFp, "DEBUG:Closed socket: %d\n", _Entries[i].fd);
#endif										
										RemoveFd(_Entries[i].fd);
									}
								}
							}
						}

					}
				}
			}
			
		} while (1);

		/*NOTREACHED*/
		return;
	}

	void
	Server::_ReloadPoll()
	{
		// Cause poll() to wake up and notice it should update..
		//
		char ToWrite = 'r';
		
		write(_InterruptPollFd, &ToWrite, 1);

		return;
	}

}
