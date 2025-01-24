/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */
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
	// An array of _FdMax ClientInfo objects.
	//
	static Server::ClientInfo	**		Clients;

	// List of file descriptors to remove from poll() next time it wakes up.
	//
	std::set<int>						Server::_RemoveFds;

	// List of file descriptors to add to poll() next time it wakes up.
	//
	std::set<int>						Server::_NewFds;

	// Mutext for _RemoveFds and _NewFds.
	//
	std::mutex							Server::_FdMutex;

	// Only one instance of the server.
	//
	Server								*	Server::Instance = nullptr;

	// Number of poll() file descriptors it is working with.
	//
	nfds_t									Server::_FdMax = 0;
	// List of poll() file descriptors it is working with.
	//
	struct pollfd					*	Server::_Entries = nullptr;

	// To wake up poll() write 1 byte to this file descriptor.
	//
	int											Server::_WakeUpPoll;

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

	// List of file descriptors to have POLLIN added to their poll() events.
	//
	std::set<int>					AddPollIn;
	
	// List of file descriptors to have POLLIN removed from their poll() events.
	//
	std::set<int>					RemovePollIn;

	// Mutex for AddPollIn and RemovePollIn
	//
	std::mutex						PollInMutex;
	
	// The name of the FIFO used to wake up poll().
	//
	static char						Fifo[PATH_MAX];

	// ??
	//
	static IO						*	IOInstance = nullptr;

	// These are the names of PhoenixEvent events.
	// The event names are strings. You register with a name and get a random ID back.
	//
	const char * const			Server::ClientBlocked_s = "ServerClientBlocked";
	const char * const			Server::ClientDisconnected_s = "ServerClientDisconnected";
	const char * const			Server::LoggingMessage_s = "ServerLoggingMessage";
	const char * const			Server::NewClientConnection_s = "ServerNewClientConnection";
	const char * const			Server::Ready_s = "ServerReady";
	const char * const			Server::ShuttingDown_s = "ServerShuttingDown";
	const char * const			Server::ErrorOnFd_s = "ServerErrorOnFd";

	// The randomly assigned PhoenixEvent ID's used when calling PhoenixEvent::Invoke().
	//
	PhoenixEvent::EventID		Server::_ClientBlockedID = 0;
	PhoenixEvent::EventID		Server::_ClientDisconnectedID = 0;
	PhoenixEvent::EventID		Server::_LoggingMessageID = 0;
	PhoenixEvent::EventID		Server::_NewClientConnectionID = 0;
	PhoenixEvent::EventID		Server::_ReadyID = 0;
	PhoenixEvent::EventID		Server::_ShuttingDownID = 0;
	PhoenixEvent::EventID		Server::_ErrorOnFdID = 0;

	int32_t
		Server::_NetWrite(int Fd, Iov * IovData)
	{
		int32_t	Results = 0;
		
		// Get a pointer to the next Blob.
		//
		uint32_t	Len = 0;
		
		uint8_t * Data = IovData->Take(Len);

		if (Len > 0 && Data != nullptr) {
			// There was something to write to this file descriptor.
			//
			Results = write(Fd, Data, Len);

			if (Results > 0) {
				if (Results == (int32_t)Len) {
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
					_Event::Invoke(Fd, ErrorOnFdID(),
												 "Server.cpp:_NetWrite:ERROR:EBANF:Closed socket.\n");
					_Event::Invoke(Fd, ClientDisconnectedID(),
												 "Server.cpp:_NetWrite:ERROR:EBANF:Closed socket.\n");
					RemoveFd(Fd);
					Results = -1;

				} else if (errno == EPIPE) {
					// The other end closed.
					//
					RemoveFd(Fd);
					Results = -1;
					_Event::Invoke(Fd, ErrorOnFdID(),
												 "Server.cpp:_NetWrite:ERROR:EPIPE:Closed socket.\n");
					_Event::Invoke(Fd, ClientDisconnectedID(),
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
		
		close(_WakeUpPoll);
		unlink(Fifo);

		return;
	}

	
	Server::Server()
		{
			if (Instance == nullptr) {
				_FdMax = sysconf(_SC_OPEN_MAX);
				
				Clients = new ClientInfo*[_FdMax];
				_Entries = new struct pollfd[_FdMax];

				for (nfds_t i = 0; i < _FdMax; i++) {
					_Entries[i].fd = -1;
					_Entries[i].events = 0;
					_Entries[i].revents = 0;
				}
				
				// Ignore SIGPIPE
				//
				struct sigaction SigPipe;
				SigPipe.sa_handler = SIG_IGN;
				SigPipe.sa_flags = 0;
				sigaction(SIGPIPE, &SigPipe, nullptr);
				
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
				//fclose(stdin);
				//fclose(stdout);

				// Where we write logs and messages.
				//
				//fclose(stderr);
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
										 "Could not parse configuration file: %s: error: %s",
										 ConfigFile.c_str(),
										 SErr.c_str());
						_Event::Invoke(-1, LoggingMessageID(), Buf);
					}
				
				} else {
					_LogFp = stderr;
				}
			
				if (access(Fifo, R_OK|W_OK) != F_OK) {
					unlink(Fifo); // In case it exsts and has incorrect permissions.
					mkfifo(Fifo, 0600);
					atexit(_Cleanup);
				}
				
				_WakeUpPoll = open(Fifo, O_RDWR);
				sem_init(&_WorkThreadReady, 0, 0);

				// Register the server PhoenixEvent names.
				//
				_ReadyID = PhoenixEvent::Register(Ready_s, nullptr);
				_ClientBlockedID = PhoenixEvent::Register(ClientBlocked_s, nullptr);
				_NewClientConnectionID = PhoenixEvent::Register(NewClientConnection_s, nullptr);
				_ClientDisconnectedID = PhoenixEvent::Register(ClientDisconnected_s, nullptr);
				_ShuttingDownID = PhoenixEvent::Register(ShuttingDown_s, nullptr);
				_ErrorOnFdID = PhoenixEvent::Register(ErrorOnFd_s, nullptr);
			
				// Register to listen for _Event::InvokeMessage().
				//
				_LoggingMessageID = PhoenixEvent::Register(LoggingMessage_s, _LogMessage);

				// Register for new client connections and validate their IP.
				//
				PhoenixEvent::Register(NewClientConnection_s, _ValidateIP);
				_Event::Invoke(-1, ReadyID(), (char*)nullptr);
			
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
		if (_ListenFd < 0) {
			fprintf(stderr, "ERROR::Server.cpp:Start():socket() return (-1)\n");
			exit(1);
		}
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
		AddFd(_WakeUpPoll);
		AddFd(_ListenFd);
		_WorkThread = std::thread(_Work);
		_WorkThread.detach();
		Results = &_WorkThread;
#ifdef DEBUG
		_Event::Invoke(-1, LoggingMessageID(), "DEBUG:_Work thread started\n");
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
	
	void
	Server::_Shutdown(int Fd)
	{
		shutdown(Fd, SHUT_RDWR);
		close(Fd);

		return;
	}
	
	// This runs as a thread.
	//
	void
		Server::_Work()
	{
		char On = 1;

		// Start listening for incomming connections.
		//
		listen(_ListenFd, _Backlog);

		SSL_load_error_strings();
		ERR_load_crypto_strings();
		OpenSSL_add_all_algorithms();
		SSL_library_init();

		// Add _ListenFd to the entries.
		//
		_Entries[_ListenFd].fd = _ListenFd;
		_Entries[_ListenFd].events = POLLIN;
			
		// Add _WakeUpPoll to the entries.
		// _WakeUpPoll is used by other threads
		// to break out of poll() and re-read the fd list.
		//
		_Entries[_WakeUpPoll].fd = _WakeUpPoll;
		_Entries[_WakeUpPoll].events = POLLIN;
			
		sem_post(&_WorkThreadReady);
		
		std::set<int>::iterator It;
		uint8_t				Buffer[1024];
		int						ToRead = 0;
		int						DidRead = 0;
		
		do {

			// Check SSL first. Skip stdin, stdout, and stderr.
			//
			for (nfds_t Fd = 3; Fd < _FdMax; Fd++) {

				if (_Entries[Fd].fd == -1) {
					// This FD not currently active.
					//
					continue;
				}

				if ((int)Fd == _WakeUpPoll) {
					// No SSL on _WakUpPoll pipe
					//
					continue;
				}

				if ((int)Fd == _ListenFd) {
					// No SSL on _ListenFd.
					//
					continue;
				}

				// Create or get the SSL info for this fd.
				//
				if (Clients[Fd] == nullptr) {
					// It is a new client connection.
					//
					Clients[Fd] = new ClientInfo();
				}

				// Create or get the Inbound buffer for this fd.
				//
				if (Clients[Fd]->InboundData.TotalLength == 0) {
					// Then Inbound TotalLength will be zero,
					// so we will know it is a new packet when we
					// get any data.
					//
					// New packet, just get the total length for now.
					//
					ToRead = sizeof(uint64_t);

				} else {
					ToRead = sizeof(Buffer);
				}

				// If NOT a new packet.
				//
				if (Clients[Fd]->InboundData.TotalLength != 0) {
					// How much do we need to finish this packet?
					//
					/**
					 * @todo fix for LARGE data sizes.
					 */
					ToRead = (int)Clients[Fd]->InboundData.TotalLength
						- (int)Clients[Fd]->InboundData.ProcessedCount;

					// Cap to size of Buffer.
					//
					if (ToRead > (int)sizeof(Buffer)) {
						ToRead = sizeof(Buffer);
					}
				}

				// Read (a) The total length uint64_t for the new packets.
				// Or (b) The total amount we still need.
				// And (c) But not more than sizeof(Buffer)
				//
				DidRead = Clients[Fd]->OurConnection.Read(Buffer, ToRead);

				if (DidRead <= 0) {
					int Err = Clients[Fd]->OurConnection.GetError(ToRead);

					// Erase any previous poll() results.
					//
					_Entries[Fd].revents = 0;

					if (Err == SSL_ERROR_WANT_READ) {
						// This means check for a POLLIN on this FD.
						//
						_Entries[Fd].events |= POLLIN;
						
					} else {
						// SSL does not want data, so don't
						// check for POLLIN. (??)
						//
						_Entries[Fd].events ^= POLLIN;
					}
					
				} else {

					// We got data in the SSL pipe, store in
					// the FD's input buffer. It may be less
					//  than what we expected.
					//
					if (Clients[Fd]->InboundData.TotalLength == 0) {
						// Is a new packet, so transfer over sizeof(uint32_t)
						// octets and un-xdr them.
						// For a new packet, we set the read amount to
						// sizeof(uint32_t).
						//
						if (DidRead == sizeof(uint32_t)) {
							uint32_t	NTotal;

							memcpy(&NTotal, Buffer, sizeof(uint64_t));
							Clients[Fd]->InboundData.TotalLength = ntohll(NTotal);
							DidRead -= (int)sizeof(uint32_t);

							if (DidRead == 0) {
								// I think it will always be zero at this point.
								//
								continue;
							}

						} else {
							// ERROR - now what?
							// We are a new packet, and did not get the
							// total length.
							//
							/**
							 * @todo Figure out what to do with incomplete total length.
							 */
							continue;
						}
								
					} else {
						// Was not a new packet.
						// The rest goes into the input buffer.
						//
						// We capped the read to be equal to or
						// less than what we need, so
						// transfer over all of it.
						//
						/**
						 * @todo Fix for LARGE data sizes.
						 */
						uint8_t	* Tmp = new uint8_t[DidRead];

						memcpy(Tmp, Buffer, DidRead);

						// Add to the input buffer for this FD.
						//
						Clients[Fd]->InboundData.Add(Tmp, DidRead, false);

						// Update the got count.
						//
						Clients[Fd]->InboundData.ProcessedCount += DidRead;

						// If we got what we need ...
						//
						if (Clients[Fd]->InboundData.ProcessedCount
								== Clients[Fd]->InboundData.TotalLength) {
							
							// We have 100% of this packet.
							//
							// We are done with this packet.
							// We have all of the XDR data for this packet.
							// It is in 1 or more iovec's.
							// And the total size of the RAW XDR data
							// is Partial->TotalSize.
							//
							// Consolidate it into one blob, cleanup the partial data.
							// Then put it on the to be processed Q.
							//
							/**
							 * @todo deal with MASSIVE packets.
							 */
							uint8_t * Data;
							uint8_t	*	DataPtr = Data = new uint8_t[Clients[Fd]->InboundData.TotalLength];
							uint32_t	Got = 0;
							uint8_t * OneBlob = Clients[Fd]->InboundData.Take(Got);

							while (OneBlob != nullptr) {
								memcpy(DataPtr, OneBlob, Got);
								DataPtr += Got;
								OneBlob = Clients[Fd]->InboundData.Take(Got);
							}
							
							// Data contans the incomming packet in XDR format.
							// DataLenght is the length of the packet.
							//
							CmdPacket *	Pkt = new CmdPacket();
							XDR					Xdrs;

							// WHAT ABOUT MASSIVE DATA IN THE PACKETS ???
							//
							xdrmem_create(&Xdrs,
														(char*)Data,
														(u_int)Clients[Fd]->InboundData.TotalLength,
														XDR_DECODE);

							// Decode the XDR data and dispatch the correct code.
							//
							if (xdr_CmdPacket(&Xdrs, Pkt)) {
								Register::Dispatch((int)Fd, Pkt);
							} else {
								PhoenixEvent::InvokeMessage((int)Fd, PhoenixEvent::LogError_s,
																						"Server.cpp:poll():xdr_CmdPacket() failed.");
							}

							// Clear out the inbound data, await more.
							//
							Clients[Fd]->InboundData.Clear();

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
			} // End of for(...) ... doing SSL_read()...

			// At this point, all client connections that
			// need SSL I/O have POLLIN set.
			//
			int Count = poll(_Entries, _FdMax, -1);

			if (Count > 0) {
				// Skip stdin, stdout, and stderr
				//
				for (nfds_t Fd = 3; Fd < _FdMax; Fd++) {

					if (_Entries[Fd].fd != -1 ) {
						int REvents = _Entries[Fd].revents;
						
						if (REvents > 0) {

							// WAKE UP POLL ...
							//
							// If the what to poll for list has changed ...
							//
							if ((int)Fd == _WakeUpPoll) {
								Count--;
								std::set<int>::iterator	SetIt;
								char TossMe;

								// We got woke up by our wake-up file descriptor
								// so that _Entries and _NumberEntries
								// could be updated. Then go back to work.
								//
								read(_WakeUpPoll, &TossMe, 1);

								// Check and update: add POLLIN, and remove POLLIN
								//
								if (TossMe == 'E'|| TossMe == 'B') {
									std::set<int>::iterator	EIt;
									
									// Go throug the existing list and update events
									// to remove POLLIN.
									//
									PollInMutex.lock();
									
									for (EIt = RemovePollIn.begin()
												 ; EIt != RemovePollIn.end()
												 ; EIt++) {

										_Entries[*EIt].events ^= POLLIN;
										_Entries[*EIt].revents = 0;
									}
									RemovePollIn.clear();
									
									// Go throug the existing list and update events
									// to add POLLIN.
									//
									for (EIt = AddPollIn.begin()
												 ; EIt != AddPollIn.end()
												 ; EIt++) {
#ifdef _GNU_SOURCE
										_Entries[*EIt].events = POLLIN | POLLRDHUP | POLLHUP | POLLERR;
#else
										_Entries[*EIt].events = POLLIN | POLLHUP | POLLERR;
#endif
										_Entries[*EIt].revents = 0;

										// We have something to poll.
										//
									}
									AddPollIn.clear();
									
									PollInMutex.unlock();
								}

								// If only update File descriptors
								// Or update Both.
								//
								if (TossMe == 'F' || TossMe == 'B') {
									_FdMutex.lock();

									// Remove any that are to be removed.
									//
									for (SetIt = _RemoveFds.begin()
												 ; SetIt != _RemoveFds.end()
												 ; SetIt++) {
										
										_Entries[*SetIt].fd = -1;
									}
									_RemoveFds.clear(); // Empty the set of new file descriptors.

									// Add any that are to be added.
									//
									for (SetIt = _NewFds.begin()
												 ; SetIt != _NewFds.end()
												 ; SetIt++) {
										
										_Entries[*SetIt].fd = *SetIt;
									}
									_NewFds.clear(); // Empty the set of new file descriptors.
									_FdMutex.unlock();
								}

							} else if ((int)Fd == _ListenFd) {
								Count--;

								// New activity on the listen() file descriptor.
								//

								// Get the file descriptor for the new connection.
								//
								int NewFd = -1;

								// The Addr is a union of ipv4 and ipv6, so show
								// we can handle the larger ipv6 address and both will work.
								//
								IPPeer Tmp;

								Tmp.AddrLen = sizeof(in6_addr);
								NewFd = accept(_ListenFd,
															 (sockaddr*)&Tmp.Addr.IpV6,
															 &Tmp.AddrLen);
								

								if (NewFd < 0) {
									fprintf(stderr, "ERROR::Server.cpp:Start():accept() return (-1)\n");
									continue; // Something went wrong!
								}
								setsockopt(NewFd, SOL_SOCKET, SO_REUSEADDR, &On, sizeof(On));

								if (Clients[NewFd] != nullptr) {
									delete Clients[NewFd];
								}
								if (_Event::Invoke(NewFd, NewClientConnectionID(),
																	 &Tmp)) {

									// Set to non-blocking.
									// Get old flags, add O_NONBLOCK and set.
									//
									int OldFlags = fcntl(NewFd, F_GETFL, 0);
								
									fcntl(NewFd, F_SETFL, OldFlags | O_NONBLOCK);

									// Make room for the new client data.
									//
									Clients[NewFd] = new ClientInfo();

									// Copy over the remote peer address
									// into the new client data.
									//
									Clients[NewFd]->Peer =  Tmp;

									// Tell poll() to add a new FD.
									// 
									AddFd(NewFd);

								} else {
									// When any NewClientConnection PhoenixEvent callback
									// returns false, then this clinet was blocked for some
									// reason.
									//
									Clients[NewFd] = nullptr;
									_Event::Invoke(NewFd, ClientBlockedID(), &Tmp);
									shutdown(NewFd, SHUT_RDWR);
									close(NewFd);
								}

								// Get SSL running on the new port.
								//
								/**@todo GET correct cert or generate a new one */
								Clients[NewFd]->OurConnection.Generate();
								Clients[NewFd]->OurConnection.Accept(NewFd, _LogFp);
								
								// Go back to work.
								//
							}
						}
					}

					// If there is still more to do.
					// Listen and WakeUp may have taken one.
					//
					if (Count > 0) {
						bool DidSomething = false;
						
						// Something to do...
						//
						// If we got here if there is a POLLIN or POLLOUT data.
						//
						// We skip stdin, stdout, and stderr.
						//

						// Is there data for us to read?
						//
						if ((_Entries[Fd].revents & POLLIN) > 0) {
							DidSomething = true;
#ifdef DEBUG
							_Event::Invoke((int)Fd,
														 LoggingMessageID(),
														 "DEBUG:Poll() got POLLIN.");
#endif
							// Let SSL_READ read some.
							//
						}
								
						if ((_Entries[Fd].revents & POLLHUP) > 0) {
							DidSomething = true;
							_Event::Invoke((int)Fd,
														 ErrorOnFdID(),
														 "Server.cpp:poll():Closed socket:POLLHUP.\n");
							RemoveFd(_Entries[Fd].fd);
						}
						if ((_Entries[Fd].revents & POLLRDHUP) > 0) {
							DidSomething = true;
							_Event::Invoke((int)Fd,
														 ErrorOnFdID(),
														 "Server.cpp:poll():Closed socket:POLLRDHUP.\n");
							RemoveFd(_Entries[Fd].fd);
						}
						if ((_Entries[Fd].revents & POLLERR) > 0) {
							DidSomething = true;
							_Event::Invoke((int)Fd,
														 ErrorOnFdID(),
														 "Server.cpp:poll():Closed socket:POLLERR.\n");
							RemoveFd(_Entries[Fd].fd);
						}

						if (DidSomething) {
							Count--;
						}
						_Entries[Fd].revents = 0;
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
		char ToWrite = 'F';

		if (_NewFds.size() > 0) {
			ToWrite = 'B'; // So it does both.
		}
		
		if (_RemoveFds.size() > 0) {
			ToWrite = 'B'; // So it does both.
		}
		
		write(_WakeUpPoll, &ToWrite, 1);

		return;
	}

	void
	Server::_UpdatePollEvents()
	{
		// Cause poll() to wake up and notice it should update..
		//
		char ToWrite = 'E';

		if (_NewFds.size() > 0) {
			ToWrite = 'B'; // So it does both.
		}
		
		if (_RemoveFds.size() > 0) {
			ToWrite = 'B'; // So it does both.
		}
		
		write(_WakeUpPoll, &ToWrite, 1);

		return;
	}

	bool
		Server::_LogMessage(int Fd, PhoenixEvent::EventID /*ID*/, void * Data)
	{
		const char * CData = (char*)Data;

		if (CData != nullptr) {
			std::string SData = CData;

			if (SData.back() != '\n') {
				SData += '\n';
			}
			if (Fd > -1) {
				fprintf(_LogFp, "File Descriptor: %d, %s", Fd, SData.c_str());
			} else {
				fprintf(_LogFp, "%s", SData.c_str());
			}
			fflush(_LogFp);
		}

		return(true);
	}

	bool
	Server::_Event::Invoke(int Fd, PhoenixEvent::EventID ID, void * Data)
	{
		return(DispatchCallbacks(Fd, ID, Data));
	}

	bool
	Server::_Event::Invoke(int Fd, PhoenixEvent::EventID ID, const IPPeer * Peer)
	{
		return(DispatchCallbacks(Fd, ID, (void*) Peer));
	}

	bool
	Server::_Event::Invoke(int Fd, PhoenixEvent::EventID ID, const char * Data)
	{
		return(DispatchCallbacks(Fd, ID, (void*)Data));
	}

	bool
	Server::_ValidateIP(int Fd, PhoenixEvent::EventID ID, void * VPeer)
	{
		IPPeer * Peer = (IPPeer*)VPeer;
		
		/**@todo add IP checking, and return false if should block the IP.*/
		
		return(true);
	}

	PhoenixEvent::EventID
		Server::ReadyID()
	{
		return(_ReadyID);
	}
	
	PhoenixEvent::EventID
		Server::NewClientConnectionID()
	{
		return(_NewClientConnectionID);
	}
		
  PhoenixEvent::EventID
		Server::ClientDisconnectedID()
	{
		return(_ClientDisconnectedID);
	}
	
	PhoenixEvent::EventID
		Server::ClientBlockedID()
	{
		return(_ClientBlockedID);
	}

	PhoenixEvent::EventID
		Server::LoggingMessageID()
	{
		return(_LoggingMessageID);
	}
	
	PhoenixEvent::EventID
		Server::ShuttingDownID()
	{
		return(_ShuttingDownID);
	}
	
	PhoenixEvent::EventID
		Server::ErrorOnFdID()
	{
		return(_ErrorOnFdID);
	}

	Server::ClientInfo::ClientInfo()
	{

		return;
	}

	Server::ClientInfo::~ClientInfo()
	{
					 
		return;
	}
	
} // End RiverExplorer::Phoenix namespace
