/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0 
 */
//
// Get poll(): POLLRDHUP
//

#include "Server.hpp"
#include "Register.hpp"

#include <stdio.h>
#ifndef W64
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#else
/**@todo W64*/
#endif

#include <semaphore.h>
#include <iostream>
#include <toml++/toml.hpp>
#include <dirent.h>
#include <endian.h>

#ifndef W64
#include <byteswap.h>
#else
#ifdef _MSC_VER
#include <stdlib.h>
#define bswap_16(x) _byteswap_ushort(x)
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)
#endif
#endif

namespace RiverExplorer::Phoenix
{

	/**
	 * We use TOML (toml++) for our configuration file.
	 */
	toml::table TomlConfigFile;
	static const char * const DefaultConfig =
		"[Administration]\n"
		"LogFile = \"Phoenix.log\"\n"
		"ListenBacklog = 10\n";

	// List of peer IP, by socket file descriptor.
	//
	std::map<int,Server::PeerInfo*>		Server::_Peers;

	// List of file descriptors to remove from poll() next time it wakes up.
	//
	std::set<int>						Server::_RemoveFds;

	// List of file descriptors to add to poll() next time it wakes up.
	//
	std::set<int>						Server::_NewFds;

	// Only one instance of the server.
	//
	Server								*	Server::Instance = nullptr;

	// Number of poll() file descriptors it is working with.
	//
	nfds_t									Server::_NumberEntries = 0;

	// List of poll() file descriptors it is working with.
	//
	struct pollfd					*	Server::_Entries = nullptr;

	// To wake up poll() write 1 byte to this file descriptor.
	//
	int											Server::_InterruptPollFd;

	// This is the socket file descriptor that accepts incomming connections.
	// Set by listen();
	//
	int											Server::_ListenFd;

	// The Server address, holds IPv4 and IPv6 addresses.
	//
	struct sockaddr_in6			Server::_ServerAddress;

	// The listen() backlog value.
	//
	int											Server::_Backlog = 10;

	// Where to log messsages to.
	//
	FILE									*	Server::_LogFp;

	// The thread that runs the server.
	//
	std::thread							_WorkThread;

	// Used to signal that the _WorkThread has started working.
	// Only use once.
	//
	sem_t										_WorkThreadReady;

	// List of data to send out, by outbound socket file descriptor.
	//
	std::map<int,Iov*>		Server::_OutboundData;

	// Mutex lock for _SendData.
	//
	std::mutex						Server::_OutboundDataMutex;

	// List of data to being received, by outbound socket file descriptor.
	//
	std::map<int,Iov*>		Server::_InboundData;

	// Mutex lock for _ReceiveData.
	//
	std::mutex						Server::_InboundDataMutex;

	// The name of the FIFO used to wake up poll().
	//
	static char						Fifo[PATH_MAX];

	// ??
	//
	static IO						*	IOInstance = nullptr;

	// These are the names of PhoenixEvent events.
	// The event names are strings. You register with a name and get a random ID back.
	//
	const char * const			Server::ClientDisconnected_s = "ServerClientDisconnected";
	const char * const			Server::LoggingMessage_s = "ServerLoggingMessage";
	const char * const			Server::NewClientConnection_s = "ServerNewClientConnection";
	const char * const			Server::Ready_s = "ServerReady";
	const char * const			Server::ShuttingDown_s = "ServerShuttingDown";
	const char * const			Server::ErrorOnFd_s = "ServerErrorOnFd";

	// The randomly assigned PhoenixEvent ID's used when calling PhoenixEvent::Invoke().
	//
	PhoenixEvent::EventID		Server::ClientDisconnectedID = 0;
	PhoenixEvent::EventID		Server::LoggingMessageID = 0;
	PhoenixEvent::EventID		Server::NewClientConnectionID = 0;
	PhoenixEvent::EventID		Server::ReadyID = 0;
	PhoenixEvent::EventID		Server::ShuttingDownID = 0;
	PhoenixEvent::EventID		Server::ErrorOnFdID = 0;

	int64_t
	Server::_NetWrite(int Fd, Iov * IovData)
	{
		int64_t	Results = 0;
		
		// Get a pointer to the next Blob.
		//
		uint64_t	Len = 0;
		
		uint8_t * Data = IovData->Take(Len);

		if (Len > 0 && Data != nullptr) {
			// There was something to write to this file descriptor.
			//
			Results = write(Fd, Data, Len);

			if (Results > 0) {
				if (Results == (int64_t)Len) {
					// We are done with this Blob.
					//
					IovData->WeUsed(Len); // We took it all.
					
				} else {
					// It write less than we asked.
					// Return the unwritten back to the Q for next time.
					//
					IovData->WeUsed(Len - Results);
				}

			} else {
				// error?
				//
				if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
					// Just Retry later.
					//
					Results = 0;

				} else if (errno == EBADF) {
					// Bad file descriptor.
					//
					_Event::Invoke(Fd, ErrorOnFdID,
												 "Server.cpp:_NetWrite:ERROR:EBANF:Closed socket.\n");
					_Event::Invoke(Fd, ClientDisconnectedID,
												 "Server.cpp:_NetWrite:ERROR:EBANF:Closed socket.\n");
					RemoveFd(Fd);
					Results = -1;

				} else if (errno == EPIPE) {
					// The other end closed.
					//
					RemoveFd(Fd);
					Results = -1;
					_Event::Invoke(Fd, ErrorOnFdID,
												 "Server.cpp:_NetWrite:ERROR:EPIPE:Closed socket.\n");
					_Event::Invoke(Fd, ClientDisconnectedID,
												 "Server.cpp:_NetWrite:ERROR:EPIPE:Closed socket.\n");
				}
			}
		}

		return(Results);
	}
	 
	// This is called by atexit()
	// to push out the log and remove the fifo.
	//
	void Server::_Cleanup()
	{
		fflush(Server::_LogFp);
		
		close(_InterruptPollFd);
		unlink(Fifo);

		return;
	}
	
	Server::Server()
		{
			if (Instance == nullptr) {
				std::string	HomeDirectory;
			
				Instance = this;

				// Remove ALL preexisting FIFO's from the FIFO dir.
				//
				DIR * OpenDir;
				struct dirent * Entry;

				std::string RmPath = INSTALL_DIR;
				std::string File;
			
				RmPath += "/NamedPipe";

				OpenDir = opendir(RmPath.c_str());

				if (OpenDir != nullptr) {
					while ((Entry = readdir(OpenDir)) != nullptr) {
						if (strncmp(Entry->d_name, "PollLoopback.", 13) == 0) {
							File = RmPath;
							File += "/";
							File += Entry->d_name;
							unlink(File.c_str());
						}
					}
					closedir(OpenDir);
				}
			
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
				fclose(stderr);
#ifndef W64
				// User ~/.config/RiverExplorer/Phoenix.conf
				//
				struct passwd		Pw;
				struct passwd * PwResult;
				char					* IPBuf;
				long						IPBufSize;
				int							Found = -1;
				std::string			ConfigFile;
			
				IPBufSize = sysconf(_SC_GETPW_R_SIZE_MAX);
				if (IPBufSize == -1) {
					IPBufSize = 16384;
				}
				IPBuf = new char[IPBufSize];

				Found = getpwuid_r(getuid(), &Pw, IPBuf, IPBufSize, &PwResult);

				if (Found == 0) {
					if (PwResult != nullptr) {
						HomeDirectory = Pw.pw_dir;
						ConfigFile = Pw.pw_dir;
						ConfigFile += "/.config/RiverExplorer/Phoenix/Phoenix.conf";
					}
				}
#else
				/**@todo*/
#endif			

				// Create an empty config file, if it does not exist.
				//
				if (access(ConfigFile.c_str(), R_OK) != 0) {
					std::string Dir = HomeDirectory;

					Dir += "/.config";;
					mkdir(Dir.c_str(), 0700);
					Dir += "/RiverExplorer";
					mkdir(Dir.c_str(), 0700);
					Dir += "/Phoenix";
					mkdir(Dir.c_str(), 0700);
							
					FILE * Tmp = fopen(ConfigFile.c_str(), "w");

					if (Tmp != nullptr) {
						fprintf(Tmp, DefaultConfig);
						fclose(Tmp);
					}
				}
			
				if (access(ConfigFile.c_str(), R_OK) == 0) {
					try {
						toml::table Config = toml::parse_file(ConfigFile);

						std::optional<std::string> LogFile = Config["Administration"]["LogFile"].value<std::string>();

						if (LogFile) {
							std::string Path = *LogFile;
						
							if ((*LogFile)[0] == '~') {
								Path = HomeDirectory;
							
								Path += "/";
								Path += LogFile->substr(1);
							}
					
							_LogFp = fopen(Path.c_str(), "a");
						}

					} catch (const toml::parse_error & Err) {
						_LogFp = stderr;
					
						char Buf[1024];

						std::string SErr = Err.description().data();
					
						snprintf(Buf,
										 sizeof(Buf),
										 "Could not parse configuration file: %s: error: %s\n",
										 ConfigFile.c_str(),
										 SErr.c_str());
						_Event::Invoke(-1, LoggingMessageID, Buf);
					}
				
				} else {
					_LogFp = stderr;
				}
			
				if (access(Fifo, R_OK|W_OK) != F_OK) {
					unlink(Fifo); // In case it exsts and has incorrect permissions.
					mkfifo(Fifo, 0600);
					atexit(_Cleanup);
				}
				
				_InterruptPollFd = open(Fifo, O_RDWR);
				sem_init(&_WorkThreadReady, 0, 0);

				// Register the server PhoenixEvent names.
				//
				ReadyID = PhoenixEvent::Register(Ready_s, nullptr);
				NewClientConnectionID = PhoenixEvent::Register(NewClientConnection_s, nullptr);
				ClientDisconnectedID = PhoenixEvent::Register(ClientDisconnected_s, nullptr);
				ShuttingDownID = PhoenixEvent::Register(ShuttingDown_s, nullptr);
				ErrorOnFdID = PhoenixEvent::Register(ErrorOnFd_s, nullptr);
			
				// Register to listen for _Event::InvokeMessage().
				//
				PhoenixEvent::Register(LoggingMessage_s, _LogMessage);

				// Register for new client connections and validate their IP.
				//
				PhoenixEvent::Register(NewClientConnection_s, _ValidateIP);
				_Event::Invoke(-1, ReadyID, (char*)nullptr);
			
			}
			// else - ignore us!
			//

			return;
		}

	Server::~Server()
		{
			/*@todo - empty for now, nothing to do*/
		}

	FILE *
		Server::GetLogFp()
	{
		return(_LogFp);
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
			AddFd(_InterruptPollFd);
			AddFd(_ListenFd);
			_WorkThread = std::thread(_Work);
			Results = &_WorkThread;
#ifdef DEBUG
			_Event::Invoke(-1, LoggingMessageID, "DEBUG:_Work thread started\n");
#endif
			sem_wait(&_WorkThreadReady);

			return(Results);
		}

	bool
		Server::Send(IO::SendPacket * Pkt)
	{
		bool Results = false;

		if (Pkt != nullptr) {
			// The data is already in XDR format.
			// Send it.
			//

		}
		
		return(Results);
	}

	void
		Server::AddFd(int Fd)
	{
		std::set<int>::iterator Found = _NewFds.find(Fd);

		if (Found == _NewFds.end()) {
			_NewFds.insert(Fd);
			_ReloadPoll();
		}

		return;
	}
	
	void
		Server::RemoveFd(int Fd)
	{
		std::set<int>::iterator Found = _RemoveFds.find(Fd);

		if (Found == _RemoveFds.end()) {
			_RemoveFds.insert(Fd);
			_ReloadPoll();
		}

		return;
	}

	// This runs as a thread.
	//
	void
		Server::_Work()
	{

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
			
		sem_post(&_WorkThreadReady);
		
		do {

			if (_NumberEntries > 0 && _Entries != nullptr) {
				int Count = poll(_Entries, _NumberEntries, -1);

				if (Count > 0) {
					for (nfds_t Offset = 0; Offset < _NumberEntries; Offset++) {

 						int REvents = _Entries[Offset].revents;
						int RFd = _Entries[Offset].fd;
						
						if (REvents > 0) {
							// If the what to poll for list has changed ...
							//
							if (RFd == _InterruptPollFd) {
								std::set<int>::iterator	SetIt;
								char TossMe;

								// We got woke up by our wake-up file descriptor
								// so that _Entries and _NumberEntries
								// could be updated. Then go back to work.
								//
								read(_InterruptPollFd, &TossMe, 1);

								nfds_t NewTotal = _NumberEntries + _NewFds.size() - _RemoveFds.size();

								SetIt = _NewFds.find(_ListenFd);
								if (SetIt != _NewFds.end()) {
									NewTotal--;
									_NewFds.erase(SetIt);
								}
								
								SetIt = _NewFds.find(_InterruptPollFd);
								if (SetIt != _NewFds.end()) {
									NewTotal--;
									_NewFds.erase(SetIt);
								}
								
								struct pollfd * NewList = new pollfd[NewTotal];

								int											NewOffset = 0;
								nfds_t									OldOffset;
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

										// Remove inbound or outbound data for this file descriptor.
										//
										std::map<int,Iov*>::iterator PIt;

										_InboundDataMutex.lock();
										PIt = _InboundData.find(_Entries[OldOffset].fd);
										Iov * IData = nullptr;

										if (PIt != _InboundData.end()) {
											IData = PIt->second;
											delete IData;
											_InboundData.erase(PIt);
										}
										_InboundDataMutex.unlock();

										_OutboundDataMutex.lock();
										PIt = _OutboundData.find(_Entries[OldOffset].fd);
										if (PIt != _OutboundData.end()) {
											IData = PIt->second;
											delete IData;
											_OutboundData.erase(PIt);
										}
										_OutboundDataMutex.unlock();
										
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

								// Add any new file descriptors to the poll() list.
								//
								for (SetIt = _NewFds.begin(); SetIt != _NewFds.end(); SetIt++) {

									if (*SetIt == -1) {
										continue;
									}

									NewList[NewOffset].fd = *SetIt;
#ifdef _GNU_SOURCE
									NewList[NewOffset].events = POLLIN | POLLOUT | POLLRDHUP | POLLHUP | POLLERR;
#else
									NewList[NewOffset].events = POLLIN | POLLOUT | POLLHUP | POLLERR;
#endif
									NewList[NewOffset++].revents = 0;
								}
								_NewFds.clear(); // Empty the set of new file descriptors.

								// We use (-1) so that any Fd's found, are invalid.
								// Zero (0) would be stdin.
								//
								memset(_Entries, (char)-1, _NumberEntries);	// Invalidate the old data (security).
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

								// The Addr is a union of ipv4 and ipv6, so show
								// we can handle the larger ipv6 address and both will work.
								//
								Info->AddrLen = sizeof(in6_addr);
								NewFd = accept(_ListenFd, (sockaddr*)&Info->Addr.ipv6, &Info->AddrLen);
								
								// Log the new connection.
								//
								char IP[INET6_ADDRSTRLEN];

								// The first object in in6_addr and in4_addr is sa_family.
								// I just need to family.
								//
								inet_ntop(((sockaddr*)&Info->Addr.ipv4)->sa_family, &Info->Addr, IP, sizeof(IP));

								// Set to non-blocking.
								//
								int OldFlags = fcntl(NewFd, F_GETFL, 0);
								
								fcntl(NewFd, F_SETFL, OldFlags | O_NONBLOCK);
								
								_Event::InvokeNewClientConnection(NewFd, Info);

								// Save the peer info.
								// Add the new file descriptor to the add list.
								//
								_Peers.insert(std::make_pair(NewFd, Info));
								AddFd(NewFd);

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

#ifdef DEBUG
										_Event::InvokeMessage(Fd, "DEBUG:Poll() got POLLIN.");
#endif										
										
										// How many octets can we read?
										//
										int				CanRead = 0;
										
										ioctl(Fd, FIONREAD, &CanRead);

										if (CanRead == 0) {
											// Remote end has disconnected.
											//
											Server::_Event::Invoke(Fd, Server::ErrorOnFdID,
																						 "Serve.cpp::poll():Zero read, closing FD");
											Server::_Event::InvokeClientDisconnected(Fd);
											RemoveFd(Fd);

											// No more processing for this Fd.
											//
											continue; // Go back to poll()
										}

										//
										// Looks like we have data to read.
										//

										// Get any partial packet data that might exist for
										// this file descriptor.
										//
										Iov * IOData = nullptr;
										std::map<int,Iov*>::iterator VIt;
										VIt = _InboundData.find(Fd);

										bool NewPacket = false;
										
										// Look for existing partial data for the file descriptor.
										//
										if (VIt == _InboundData.end()) {
											// None, start collecting a new packet.
											//
											IOData = new Iov();
											NewPacket = true;
											_InboundData.insert(std::make_pair(Fd,IOData));
											
										} else {
											// Yep, continue collecting for this file descriptor.
											//
											IOData = VIt->second;
										}

										// The first uint64_t of a new packet
										// is the total packet length in network byte order.
										//
										if (NewPacket) {
											if (CanRead < (int)sizeof(uint64_t)) {
												// This is a new packet and we need all of
												// the first uint64_t octets, go back and wait.
												//
												continue; // As in go back to poll().
											}

											// We are taking sizeof(uint64_t).
											//
											CanRead -= (int)sizeof(uint64_t);

											// The data arrives in network byte order.
											//
											uint64_t	NTotalLen;
											
											ssize_t Res = read(Fd, &NTotalLen, sizeof(uint64_t));

											if (Res == -1) {
												if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
													continue;	// Go back to poll().

												} else if (errno == EBADF) {
													RemoveFd(Fd);
													continue; // Go back to poll().
												}
											}
											
											// Swap if needed network byte order to
											// host byte order.
											//
											IOData->TotalLength = ntohll(NTotalLen);
										}

										// Sill more? (We may have took some above.)
										//
										if (CanRead > 0) {
											// In addition to getting partial packets,
											// we might get multiple packets, check here.
											//
											// Calculate the total collected so far. ...
											//
											size_t	Have = IOData->ProcessedCount;

											if ((Have + CanRead) > IOData->TotalLength) {
												// The rest belong to the next packet.
												//
												CanRead = (int)((size_t)IOData->TotalLength - (int)Have);
											}

											// Allocate what we can or should read.
											//
											uint8_t * ReadBuf = new uint8_t[CanRead];

											ssize_t Res = read(Fd, ReadBuf, CanRead);
											
											if (Res == -1) {
												if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
													continue;	// Go back to poll().

												} else if (errno == EBADF) {
													RemoveFd(Fd);
													continue; // Go back to poll().
												}
											}

											if (Res > 0) {
												IOData->Add(ReadBuf, Res, false);
												IOData->ProcessedCount += Res;


												// If done with this packet.
												//
												if (IOData->ProcessedCount == IOData->TotalLength) {
													// We are done with this packet.
													// We have all of the XDR data for this packet.
													// It is in 1 or more iovec's.
													// And the total size of the RAW XDR data is Partial->TotalSize.
													//
													// Consolidate it into one blob, cleanup the partial data.
													// Then put it on the to be processed Q.
													//
													uint8_t * Data;
													uint8_t	*	DataPtr = Data = new uint8_t[IOData->TotalLength];
													//uint64_t	DataLength = IOData->TotalLength;
													uint64_t	Got = 0;
													uint8_t * OneBlob = IOData->Take(Got);

													while (OneBlob != nullptr) {
														memcpy(DataPtr, OneBlob, Got);
														DataPtr += Got;
														OneBlob = IOData->Take(Got);
													}
													IOData->Clear();

													// Data contans the incomming packet in XDR format.
													// DataLenght is the length of the packet.
													//
													CmdPacket Pkt;
													XDR Xdrs;
												
													xdrmem_create(&Xdrs,
																				(char*)Data,
																				(u_int)IOData->TotalLength,
																				XDR_DECODE);

													// Decode the XDR data and dispatch the correct code.
													//
													if (xdr_CmdPacket(&Xdrs, &Pkt)) {
														Register::Dispatch(Fd, &Pkt);
													} else {
														PhoenixEvent::InvokeMessage(Fd, PhoenixEvent::LogError_s,
																												"Server.cpp:poll():xdr_CmdPacket() failed.\n");
													}

													// Free internal data.
													//
													xdr_free((xdrproc_t)xdr_CmdPacket, &Pkt);

													// Delete the XDR data.
													//
													delete[] Data;

													// Destroy the XDR object.
													//
													xdr_destroy(&Xdrs);
												}
											}
										}
									}

									// Can we write data?
									//
									if ((_Entries[i].revents & POLLOUT) > 0) {

										// The data we get, is already XDR encoded and ready to be sent.
										//
										int Fd = _Entries[i].fd;
										ssize_t OctetsWrote = 0;
										std::map<int,Iov*>::iterator VIt;
										Iov * IOData = nullptr;

										VIt = _OutboundData.find(Fd);

										// If we reach the end, then we have nothing to write - done.
										//
										if (VIt != _OutboundData.end()) {
											IOData = VIt->second;
											OctetsWrote = _NetWrite(Fd, IOData);

											if (OctetsWrote == -1) {
												// Something went wrong, _NetWrite() logged it.
												// Remove this Fd from the list.
												//
												RemoveFd(Fd);
												continue; // To poll().
											}
										}
									}
								}

								if ((_Entries[i].revents & POLLHUP) > 0) {
									_Event::Invoke(_Entries[i].fd,
																 ErrorOnFdID,
																 "Server.cpp:poll():Closed socket:POLLHUP.\n");
									RemoveFd(_Entries[i].fd);
									continue; // To poll().
								}
								if ((_Entries[i].revents & POLLRDHUP) > 0) {
									_Event::Invoke(_Entries[i].fd,
																 ErrorOnFdID,
																 "Server.cpp:poll():Closed socket:POLLRDHUP.\n");
									RemoveFd(_Entries[i].fd);
									continue; // To poll().
								}
								if ((_Entries[i].revents & POLLERR) > 0) {
									_Event::Invoke(_Entries[i].fd,
																 ErrorOnFdID,
																 "Server.cpp:poll():Closed socket:POLLERR.\n");
									RemoveFd(_Entries[i].fd);
									continue; // To poll().
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

	bool
		Server::_LogMessage(int Fd, PhoenixEvent::EventID /*ID*/, void * Data)
	{
		const char * CData = (char*)Data;

		if (Fd > -1) {
			fprintf(_LogFp, "File Descriptor: %d, %s", Fd, CData);
		} else {
			fprintf(_LogFp, "%s", CData);
		}
		fflush(_LogFp);

		return(true);
	}

	bool
	Server::_Event::Invoke(int Fd, PhoenixEvent::EventID ID, void * Data)
	{
		return(DispatchCallbacks(Fd, ID, Data));
	}

	bool
	Server::_Event::Invoke(int Fd, PhoenixEvent::EventID ID, const char * Data)
	{
		return(DispatchCallbacks(Fd, ID, (void*)Data));
	}

	bool
	Server::_ValidateIP(int Fd, PhoenixEvent::EventID ID, void * VPeer)
	{
		PeerInfo * Peer = (PeerInfo*)VPeer;
		
		/**@todo add IP checking, and return false if should block the IP.*/
		
		return(true);
	}

}
