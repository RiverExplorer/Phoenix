/**
 * Project: Phoenix
 *
 * @file Server.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * Time-stamp: "2025-03-06 15:42:38 doug"
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */
#include "Configuration.hpp"
#include "Log.hpp"
#include "Server.hpp"
#include "Register.hpp"
#include "ThreadName.hpp"

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
#include <dirent.h>
#include <endian.h>
#include <csignal>

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
	// SIGHUP
	//
	static volatile sig_atomic_t SigHup = 0;
	
	// List of peer IP, by socket file descriptor.
	// An array of _FdMax ClientInfo objects.
	//
	static Server::ClientInfo	**		Clients;

#ifdef DEBUG	
	bool														Server::_NoTls = true;
#else
	bool														Server::_NoTls = false;
#endif
	
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

	// The name of the server program.
	//
	const char						*	Server::_ServerProgramName = nullptr;
	static std::string			ServerPath;
	
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

	ssize_t
	Server::_NetWrite(int Fd, Iov * IovData)
	{
		ssize_t	Results = 0;
		
		// Get a pointer to the next Blob.
		//
		ssize_t	Len = 0;
		
		uint8_t * Data = IovData->Take((uint32_t&)Len);

		if (Len > 0 && Data != nullptr) {
			// There was something to write to this file descriptor.
			//
			Results = write(Fd, Data, Len);

			if (Results > 0) {
				if (Results == (int32_t)Len) {
					// We are done with this Blob.
					//
					IovData->WeUsed((uint32_t)Len); // We took it all.
					
				} else {
					// It write less than we asked.
					// Return the unwritten back to the Q for next time.
					//
					IovData->WeUsed((uint32_t)(Len - Results));
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
					//Server.cpp:_NetWrite:ERROR:EBANF:Closed socket.
					//
					_Event::InvokePeer(Fd,
														 Event::ErrorOnFd_Event,
														 &Clients[Fd]->Peer);
					_Event::InvokePeer(Fd,
														 Event::ClientDisconnected_Event,
														 &Clients[Fd]->Peer);
					RemoveFd(Fd);
					Results = -1;

				} else if (errno == EPIPE) {
					// The other end closed.
					//
					RemoveFd(Fd);
					Results = -1;

					// Server.cpp:_NetWrite:ERROR:EPIPE:Closed socket.
					//
					_Event::InvokePeer(Fd,
														 Event::ErrorOnFd_Event,
														 &Clients[Fd]->Peer);
					_Event::InvokePeer(Fd,
														 Event::ClientDisconnected_Event,
														 &Clients[Fd]->Peer);
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
	

	void
	MakeServerPath(const char * ServerProgramName)
	{
#ifndef W64
		struct passwd	*	Pwd;
		
		Pwd = getpwuid(getuid());

		if (Pwd != nullptr) {
			ServerPath = Pwd->pw_dir;
		}
#else
		ERROR IMPLEMENT W64;
#endif
		
		// Path to the directory where the log file exists,
		// under the users home.
		//
		// Linux Path:
		//
		//		~/.Phoenix/RiverExplorer/Server/<ServerName>
		//
		// Windows Path:
		//
		//		(APP_DATA)/RiverExplorer/Phoenix/Server/<ServerName>
		//
		const char * Parts[] = {
#ifndef W64			
			".Phoenix",
#endif
			"RiverExplorer",
			"Server",
			nullptr
		};

		int						POffset = 0;
		const char	*	Part = Parts[POffset];

		while (Part != nullptr) {
			ServerPath += "/";
			ServerPath += Part;
				
			if (access(ServerPath.c_str(), R_OK|W_OK|X_OK) != F_OK) {
				if (mkdir(ServerPath.c_str(), 0700) != 0) {
					// ERROR
					break;
				}
			}
			Part = Parts[++POffset];
		}
		ServerPath += "/";
		ServerPath += ServerProgramName;
		mkdir(ServerPath.c_str(), 0700);

		return;
	}

	static void
	SigHupHandler(int Signal)
	{
		SigHup = Signal;
		Log::PrintInformation("Got SIGHUP, exiting.");
		exit(0);
	}
	
	Server::Server(const std::string & ServerName)
	{
		static const char * LogDevice_s = "LogDevice";

		// Use TLS
		_NoTls = false;
		
		signal(SIGHUP, SigHupHandler);
		
		_ServerProgramName = strdup(ServerName.c_str());
		
		Configuration::ServerInitializeConfiguration(ServerName);
		MakeServerPath(_ServerProgramName);

		std::string LogDevice;
		
		if (_LogFp == nullptr) {
			if (Configuration::ServerHasConfig(LogDevice_s)) {
				LogDevice = Configuration::ServerGetConfig(LogDevice_s);
			}
		}
		if (LogDevice.length() < 2) {
			// It was not set, add the default configuration
			// path.
			//
			LogDevice = ServerPath;
			LogDevice += "/";
			LogDevice += ServerName;
			LogDevice += ".log";
			Configuration::ServerSetConfig(LogDevice_s, LogDevice);
		}
		
		_LogFp = fopen(LogDevice.c_str(), "a");

		if (_LogFp == nullptr) {
			//  Log::PrintError() will not work here.
			//
			fprintf(stderr, "ERROR: Can not open LOG device: %s\n",
							LogDevice.c_str());
			exit(1);
		}
		Log * TheLog = Log::NewLog(_LogFp, ServerName.c_str());

		atexit(_Cleanup);
		
		std::string PidFile = ServerPath;

		PidFile += "/";
		PidFile += ServerName;
		PidFile += ".pid";

		FILE * PidFp = fopen(PidFile.c_str(), "w");

		fprintf(PidFp, "%u\n", getpid());
		fclose(PidFp);
		chmod(PidFile.c_str(), 0600);

		ThreadName::NameOurThread("Server");
		Log::PrintInformation("Server thread starting. PID=%u",
													getpid());

		const std::string NoTls = Configuration::ServerGetConfig("NoTls");

		if (NoTls == "true") {
			_NoTls = true;
		} else {
			_NoTls = false;
		}
		
		if (Instance == nullptr) {
			_FdMax = sysconf(_SC_OPEN_MAX);
				
			Clients = new ClientInfo*[_FdMax];
			memset(Clients, 0, sizeof(ClientInfo*) * _FdMax);
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
					ConfigFile += "/.config/RiverExplorer/Phoenix/Server/";
					ConfigFile += _ServerProgramName;
					ConfigFile += ".xml";
				}
			}
#else
			/**@todo*/
#endif			

			Configuration::ServerInitializeConfiguration(_ServerProgramName);
			Configuration::ServerLoadConfig();
			
			if (access(Fifo, R_OK|W_OK) != F_OK) {
				unlink(Fifo); // In case it exsts and has incorrect permissions.
				mkfifo(Fifo, 0600);
				atexit(_Cleanup);
			}
				
			_WakeUpPoll = open(Fifo, O_RDWR);
			sem_init(&_WorkThreadReady, 0, 0);

			// Register to listen for _Event::InvokeMessage().
			//
			Event::Register(Event::LoggingMessage_Event, _LogMessage);

			// Register for new client connections and validate their IP.
			//
			Event::Register(Event::NewClientConnection_Event, _ValidateIP);

			// Register for ClientDisconnected
			//
			Event::Register(Event::ClientDisconnected_Event, _Disconnected);
			
			// Register for POLLERR
			//
			Event::Register(Event::ErrorOnFd_Event, _ErrorOnFd);
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

		ThreadName::NameOurThread("Server::Start");

		char On = 1;

		IOInstance = new IO(true);
		
		memset(&_ServerAddress, 0, sizeof(_ServerAddress));

		_ListenFd = socket(AF_INET6, SOCK_STREAM, 0);
		if (_ListenFd < 0) {
			Log::PrintError("Server.cpp:Start():socket() return (-1).");
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

#ifdef DEBUG
	static void
	PrintFds(struct pollfd * Entry)
	{
		std::string Str = "FD=";

		Str += std::to_string(Entry->fd);

		Str += " EVENTS  ";

		if (Entry->events & POLLIN) {
			Str += "  POLLIN ";
		}	else {
			Str += " !POLLIN ";
		}
		
		if (Entry->events & POLLPRI) {
			Str += "  POLLPRI ";
		}	else {
			Str += " !POLLPRI ";
		}
		
		if (Entry->events & POLLOUT) {
			Str += "  POLLOUT ";
		}	else {
			Str += " !POLLOUT ";
		}
		
		if (Entry->events & POLLRDHUP) {
			Str += "  POLLRDHUP ";
		}	else {
			Str += " !POLLRDHUP ";
		}
		
		if (Entry->events & POLLERR) {
			Str += "  POLLERR ";
		}	else {
			Str += " !POLLERR ";
		}
		
		if (Entry->events & POLLHUP) {
			Str += "  POLLHUP ";
		}	else {
			Str += " !POLLHUP ";
		}
		
		if (Entry->events & POLLNVAL) {
			Str += "  POLLNVAL ";
		}	else {
			Str += " !POLLNVAL ";
		}
		
		if (Entry->events & POLLRDBAND) {
			Str += "  POLLRDBAND ";
		}	else {
			Str += " !POLLRDBAND ";
		}
		
		if (Entry->events & POLLMSG) {
			Str += "  POLLMSG ";
		}	else {
			Str += " !POLLMSG ";
		}
		
		Str += "\n     REVENTS ";

		if (Entry->revents & POLLIN) {
			Str += " *POLLIN ";
		}	else {
			Str += " !POLLIN ";
		}
		
		if (Entry->revents & POLLPRI) {
			Str += " *POLLPRI ";
		}	else {
			Str += " !POLLPRI ";
		}
		
		if (Entry->revents & POLLOUT) {
			Str += " *POLLOUT ";
		}	else {
			Str += " !POLLOUT ";
		}
		
		if (Entry->revents & POLLRDHUP) {
			Str += " *POLLRDHUP ";
		}	else {
			Str += " !POLLRDHUP ";
		}
		
		if (Entry->revents & POLLERR) {
			Str += " *POLLERR ";
		}	else {
			Str += " !POLLERR ";
		}
		
		if (Entry->revents & POLLHUP) {
			Str += " *POLLHUP ";
		}	else {
			Str += " !POLLHUP ";
		}
		
		if (Entry->revents & POLLNVAL) {
			Str += " *POLLNVAL ";
		}	else {
			Str += " !POLLNVAL ";
		}
		
		if (Entry->revents & POLLRDBAND) {
			Str += " *POLLRDBAND ";
		}	else {
			Str += " !POLLRDBAND ";
		}
		
		if (Entry->revents & POLLMSG) {
			Str += " *POLLMSG ";
		}	else {
			Str += " !POLLMSG ";
		}
		
		Str += "\n";
		printf("%s", Str.c_str());
		return;
	}
#endif	
	// This runs as a thread.
	//
	void
	Server::_Work()
	{
		ThreadName::NameOurThread("Server._Work");

		std::string OurHostName
			= Configuration::ServerGetConfig(Configuration::OurHostName_s);
		
		char On = 1;

		short int PollFlags;
		
		PollFlags = POLLIN
			| POLLPRI
			| POLLERR
			| POLLHUP
			| POLLNVAL
			| POLLRDBAND
			| POLLMSG;
		
#ifdef _GNU_SOURCE
			PollFlags |= POLLRDHUP;
#endif

			// Start listening for incomming connections.
		//
		listen(_ListenFd, _Backlog);

		if (!_NoTls) {
			SSL_load_error_strings();
			ERR_load_crypto_strings();
			OpenSSL_add_all_algorithms();
			SSL_library_init();
		}

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

				// Create client data set, per connection.
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
						_Entries[Fd].events = PollFlags;
						
					} else {
						// SSL does not want data, so don't
						// check for POLLIN. (??)
						//
						_Entries[Fd].events ^= POLLIN;
					}
					
				} else {

					// We got data in the SSL pipe, store in
					// the FD's input buffer. It may be less
					// than what we expected.
					//
					if (Clients[Fd]->InboundData.TotalLength == 0) {
						// Is a new packet, so transfer over sizeof(uint32_t)
						// octets and un-xdr them.
						// For a new packet, we set the read amount to
						// sizeof(uint32_t).
						//
						if (DidRead == sizeof(uint32_t)) {
							uint32_t	NTotal;

							memcpy(&NTotal, Buffer, sizeof(uint32_t));
							Clients[Fd]->InboundData.TotalLength = ntohl(NTotal);
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
						// less than what we need, so transfer over all of it.
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
							PacketBody *	Pkt = new PacketBody();
							XDR					Xdrs;

							// WHAT ABOUT MASSIVE DATA IN THE PACKETS ???
							//
							xdrmem_create(&Xdrs,
														(char*)Data,
														(u_int)Clients[Fd]->InboundData.TotalLength,
														XDR_DECODE);

							// Decode the XDR data and dispatch the correct code.
							//
							if (xdr_PacketBody(&Xdrs, Pkt)) {
 								Register::Dispatch((int)Fd, Pkt);
							} else {
								Event::InvokeMessage((int)Fd, Event::LogError_Event,
																		 "Server.cpp:poll():xdr_CmdPacket() failed.");
							}

							// Clear out the inbound data, await more.
							//
							Clients[Fd]->InboundData.Clear();

							// Free internal data.
							//
							xdr_free((xdrproc_t)xdr_PacketBody, &Pkt);

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

#ifdef DEBUG
					if (_Entries[Fd].revents != 0) {
						PrintFds(&_Entries[Fd]);
					}
#endif				
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
								if (TossMe == 'P') {
									std::set<int>::iterator	EIt;
									
									// Go through the remove list and update events
									// to remove POLL...
									//
									PollInMutex.lock();
									
									for (EIt = RemovePollIn.begin()
												 ; EIt != RemovePollIn.end()
												 ; EIt++) {
										_Entries[*EIt].fd = -1;
										_Entries[*EIt].events = 0;
										_Entries[*EIt].revents = 0;
									}
									RemovePollIn.clear();
									
									// Go through the add list and update events
									// to add POLLIN.
									//
									for (EIt = AddPollIn.begin()
												 ; EIt != AddPollIn.end()
												 ; EIt++) {
										_Entries[*EIt].fd = *EIt;
										_Entries[*EIt].events = PollFlags;
										_Entries[*EIt].revents = 0;
									}
									AddPollIn.clear();
									
									PollInMutex.unlock();
								}

								// Done with this _WakeUpPoll
								//
								continue;
								
							} else if ((int)Fd == _ListenFd) {

								// Clear the POLLIN for this FD.
								//
								_Entries[Fd].revents = 0;
								Count--;

								// New activity on the listen() file descriptor.
								//
								// Get the file descriptor for the new connection.
								//
								int NewFd = -1;

								// The Addr is a union of ipv4 and ipv6, so show
								// we can handle the larger ipv6 address and both
								// will work.
								//
								IPPeer Tmp;

								Tmp.AddrLen = sizeof(in6_addr);
								NewFd = accept(_ListenFd,
															 (sockaddr*)&Tmp.Addr.IpV6,
															 &Tmp.AddrLen);
								
								if (NewFd < 0) {
									Log::PrintError("Server.cpp:Start():accept() return (-1).");
									continue; // Something went wrong!
								}
								setsockopt(NewFd, SOL_SOCKET, SO_REUSEADDR, &On, sizeof(On));
								setsockopt(NewFd, SOL_SOCKET, SO_KEEPALIVE, &On, sizeof(On));

								if (Clients[NewFd] != nullptr) {
									delete Clients[NewFd];
								}
								if (_Event::InvokePeer(NewFd,
																			 Event::NewClientConnection_Event,
																			 &Tmp)) {

									// Set to non-blocking.
									// Get old flags, add O_NONBLOCK and set.
									//
									int Flags = fcntl(NewFd, F_GETFL, 0);
								
									fcntl(NewFd, F_SETFL, Flags | O_NONBLOCK);

									// Make room for the new client data.
									//
									Clients[NewFd] = new ClientInfo();

									// Copy over the remote peer address
									// into the new client data.
									//
									Clients[NewFd]->Peer =  Tmp;

									// We add the new FD to the poll list.
									//
									_Entries[NewFd].fd = NewFd;
									_Entries[NewFd].events = PollFlags;
									_Entries[NewFd].revents = 0;

								} else {
									// The application said to block this IP.
									// 
									// When any NewClientConnection Event callback
									// returns false, then this clinet was blocked for some
									// reason.
									//
									if (Clients[NewFd] != nullptr) {
										delete Clients[NewFd];
										Clients[NewFd] = nullptr;
									}
									_Entries[NewFd].fd = -1;
									_Entries[NewFd].events = 0;
									_Entries[NewFd].revents = 0;
									_Event::InvokePeer(NewFd,
																		 Event::ClientBlocked_Event,
																		 &Tmp);
									shutdown(NewFd, SHUT_RDWR);
									close(NewFd);
								}

								// Get SSL running on the new port.
								//
								/**@todo GET correct cert or generate a new one */
								Clients[NewFd]->OurConnection.Generate(OurHostName.c_str());
								Clients[NewFd]->OurConnection.Accept(NewFd, _LogFp);
							}
						}

						// If still more to do
						//
						if (_Entries[Fd].revents > 0) {
							
							// Was not a new conncetion, or a wake up.
							//
							bool DidSomething = false;
							bool Clear = false;

							// POLLHUP and POLLRDHUP are odd.
							// One can happen, or both can happen.
							// We do not want 2 notices for the same thing.
							//
							bool NotifiedPollHup = false;
							
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
								// Let SSL_READ read some.
								//
							}

							if ((_Entries[Fd].revents & POLLHUP) > 0) {
								DidSomething = true;
								Clear = true;
								NotifiedPollHup = true;
								_Event::InvokePeer((int)Fd,
																	 Event::ClientDisconnected_Event,
																	 &Clients[Fd]->Peer);
							}
							if ((_Entries[Fd].revents & POLLRDHUP) > 0) {
								DidSomething = true;
								Clear = true;
								if (!NotifiedPollHup) {
									_Event::InvokePeer((int)Fd,
																		 Event::ClientDisconnected_Event,
																		 &Clients[Fd]->Peer);
								}
							}
							if ((_Entries[Fd].revents & POLLERR) > 0) {
								DidSomething = true;
								Clear = true;
								_Event::InvokePeer((int)Fd,
																	 Event::ErrorOnFd_Event,
																	 &Clients[Fd]->Peer);
							}

							if (Clear) {
								_Entries[Fd].fd = -1;
								_Entries[Fd].events = 0;
								_Shutdown((int)Fd);
							}
							if (DidSomething) {
								Count--;
							}
							// Clear the revents for this FD.
							//
							_Entries[Fd].revents = 0;
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
		char ToWrite = 'P';
		
		write(_WakeUpPoll, &ToWrite, 1);

		return;
	}

	void
	Server::_UpdatePollEvents()
	{
		// Cause poll() to wake up and notice it should update..
		//
		char ToWrite = 'P';
		
		write(_WakeUpPoll, &ToWrite, 1);

		return;
	}

	bool
	Server::_LogMessage(int Fd, Event::Event_e /*ID*/, void * Data)
	{
		const char * CData = (char*)Data;

		if (CData != nullptr) {
			std::string SData = CData;

			if (SData.back() != '\n') {
				SData += '\n';
			}
			if (Fd > -1) {
				Log::PrintInformation("File Descriptor: %d, %s", Fd, SData.c_str());
			} else {
				Log::PrintInformation("%s", SData.c_str());
			}
			fflush(_LogFp);
		}

		return(true);
	}

	bool
	Server::_Event::InvokeData(int Fd, Event::Event_e ID, void * Data)
	{
		return(DispatchCallbacks(Fd, ID, Data));
	}

	bool
	Server::_Event::InvokePeer(int Fd, Event::Event_e ID, const IPPeer * Peer)
	{
		return(DispatchCallbacks(Fd, ID, (void*)Peer));
	}

	bool
	Server::_Event::InvokeMessage(int Fd, Event::Event_e ID, const char * Data)
	{
		return(DispatchCallbacks(Fd, ID, (void*)Data));
	}

	bool
	Server::_ValidateIP(int Fd, Event::Event_e ID, void * VPeer)
	{
		bool Results = true;
		
		IPPeer * Peer = static_cast<Phoenix::IPPeer*>(VPeer);
		
		char IP[INET6_ADDRSTRLEN];

		inet_ntop(((sockaddr*)&Peer->Addr.IpV4)->sa_family,
							&Peer->Addr, IP,
							sizeof(IP));

		Log::PrintInformation("Validaing connectopm: %s-FD:%d", IP, Fd);

		/**
		 * @todo add IP checking, and return false if should block the IP.
		 * Check certs? ...
		 */
		
		if (!Results) {
			Log::PrintInformation("IP Blocked: %s-FD:%d", IP, Fd);
		} else {
			Log::PrintInformation("IP Allowed: %s-FD:%d", IP, Fd);
		}
		
		return(true);
	}

	bool
	Server::_Disconnected(int Fd, Event::Event_e ID, void * VPeer)
	{
		// This event can not be canceled, it happened.
		//
		bool Results = true;

		IPPeer * Peer = static_cast<Phoenix::IPPeer*>(VPeer);
		
		char IP[INET6_ADDRSTRLEN];

		inet_ntop(((sockaddr*)&Peer->Addr.IpV4)->sa_family,
							&Peer->Addr, IP,
							sizeof(IP));
		
		Log::PrintInformation("IP Disconnected: %s-FD:%d", IP, Fd);

		return(Results);
	}

	bool
	Server::_ErrorOnFd(int Fd, Event::Event_e ID, void * VPeer)
	{
		// This event can not be canceled, it happened.
		//
		bool Results = true;

		IPPeer * Peer = static_cast<Phoenix::IPPeer*>(VPeer);
		
		char IP[INET6_ADDRSTRLEN];

		inet_ntop(((sockaddr*)&Peer->Addr.IpV4)->sa_family,
							&Peer->Addr, IP,
							sizeof(IP));
		
		Log::PrintInformation("Got network error: %s-FD:", IP, Fd);

		return(Results);
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
