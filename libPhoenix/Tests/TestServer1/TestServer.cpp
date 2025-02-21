/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

/**
 * THIS TEST CODE is a very simple Phoenix server.
 */
 
#include <RiverExplorer/Phoenix/PhoenixServer.hpp>
#include <RiverExplorer/Phoenix/PhoenixEvent.hpp>
#include <RiverExplorer/Phoenix/IPPeer.hpp>
#include <RiverExplorer/Phoenix/Register.hpp>

#include <exception>

#ifndef W64
#include <arpa/inet.h>
#endif

const char * const VendorID = "RiverExplorer:TestServer1";

uint16_t			TestPort = 6112;
const char *	TestDevice = "lo";

std::mutex		HangMain;

using namespace RiverExplorer;
using namespace RiverExplorer::Phoenix;

std::map<int,Phoenix::IPPeer*> Connections;

/**
 * This method is registered in main below to be called
 * when the server is ready and has started performing
 * it operations. And before any incomming connections have
 * happened.
 *
 * @param Fd The Server::Ready() event always passes us
 * a (-1) as no file descriptor is valid for this event and
 * it is ignored here.
 *
 * @param ID The ID of the event, in this case a Server::ReadyID()
 * It is ignored in this test code.
 *
 * @param Data The Server::Ready() event always passes us
 * a nullptr as no data is associated with this event.
 * And it is ignored in this test code.
 *
 * @return The return value for this event is ignored
 * and this method always returns true.
 */
bool
MyReadyCallback(int /*Fd*/, Event::Event_e /*ID*/, void * /*Data*/)
{
	fprintf(stdout, "Got ServerReady event.\n");

	return(true);
}

/*
 * This method is registered in main below to be called
 * when the Phoenix Server library accepts an incomming connceton.
 * And before placing it in service.
 *
 * The Phoenix::Server::NewClientConnection_s PhoenixEvent expects a return
 * of true or false. Where a false return phrohibits the server from
 * allowing a client on this * connection.
 *
 * This could be because of system design, spam, DDOS,
 * or other controls.
 *
 * @param Fd It get the new socket file descriptor.
 *
 * @param ID The event ID, in this case Server::NewClientConnection().
 *
 * @param Peer A (Server::PeerInfo*) sent as a generic (void*).
 *
 * @return true if this method is okay with the peer.
 * This method returns false to block this peer from continuing.
 *
 * In this TEST CODE, this method always returns true.
 */
bool
MyNewClientCallback(int Fd,
										Event::Event_e,
										void * Data)
{
	Phoenix::IPPeer * Peer = static_cast<Phoenix::IPPeer*>(Data);

	char IP[INET6_ADDRSTRLEN];

	// Note the IpV4 and IpV6 sa_family are the same thing,
	// in the same position, so I just pick one  and it works
	// for both.
	//
	inet_ntop(((sockaddr*)&Peer->Addr.IpV4)->sa_family,
						&Peer->Addr, IP,
						sizeof(IP));

	Connections.insert(std::make_pair(Fd, Peer));

	fprintf(stdout, "Client connecting from: %s\n", IP);

	// Now sent it a CAPABILITY_PRE.
	//

	return(true);
}

/**
 * This method is registered in main below to be called
 * when the Phoenix Server library detects that a client
 * has disconnected.
 *
 * @param Fd The associated file descriptor.
 *
 * @param ID The PhoenixEvent::ID.
 *
 * @param Data Will be nullptr.
 *
 * @return Always returns true as ClientDisconnectedID()
 * does not use the return value from the callbacks.
 */
bool
MyClientDisconnected(int Fd, Event::Event_e, void * /*NotUsed*/)
{
	std::map<int,Phoenix::IPPeer*>::iterator PeerIt;

	PeerIt = Connections.find(Fd);
	Phoenix::IPPeer * Peer = nullptr;
	
	if (PeerIt != Connections.end()) {

		Peer = PeerIt->second;
		char IP[INET6_ADDRSTRLEN];

		inet_ntop(((sockaddr*)&Peer->Addr.IpV4)->sa_family,
							&Peer->Addr, IP,
							sizeof(IP));

		fprintf(stdout, "Client disconnected: %s\n", IP);
		
	} else {
		fprintf(stdout,
						"ERROR:Got disconnect from an unknown file descriptor: %d\n",
						Fd);
						
	}
	
	return(true);
}

/**
 * The client is saying goodbye.
 *
 * @param Fd The associated file descriptor.
 *
 * @param ID The PhoenixEvent::ID.
 *
 * @param Data Will be nullptr.
 *
 * @return Always returns true as it can not be aborted.
 */
bool
MyClientBye(int Fd, Event::Event_e, void * /*NotUsed*/)
{
	std::map<int,Phoenix::IPPeer*>::iterator PeerIt;

	PeerIt = Connections.find(Fd);
	Phoenix::IPPeer * Peer = nullptr;
	
	if (PeerIt != Connections.end()) {

		Peer = PeerIt->second;
		char IP[INET6_ADDRSTRLEN];

		inet_ntop(((sockaddr*)&Peer->Addr.IpV4)->sa_family,
							&Peer->Addr, IP,
							sizeof(IP));

		fprintf(stdout, "BYE from Client: %s\n", IP);
		
	} else {
		fprintf(stdout,
						"ERROR:Got BYE from an unknown file descriptor: %d\n",
						Fd);
	}
	
	return(true);
}

int
main(int /*argc*/, char ** /*argv*/)
{
	unlink("Phoenix.log"); // Delete the results of previous runs of thise test.
	
	fprintf(stdout, "TestServer Starting\n");
	
	Phoenix::Server	TheServer;

	// Register the server to send out CAPABILITY_PRE on new connections.
	//
	Event::Register(Event::NewClientConnection_Event,
									MyNewClientCallback);

	//
	//
	Event::Register(Event::ServerReady_Event,
									MyReadyCallback);

	// Called when the server geta a new connection.
	//
	Event::Register(Event::NewClientConnection_Event,
									MyNewClientCallback);

	// Called when the client disconnects.
	//
	Event::Register(Event::ClientDisconnected_Event,
									MyClientDisconnected);

	// Called with the client sends a BYE command.
	//
	Event::Register(Event::Bye_Event,
									MyClientBye);
	
	std::thread * ServerThread = TheServer.Start(TestPort, TestDevice);

	fprintf(stdout, "ServerThread started.\n");
	
	// There is no way with std::thread to keep other threads
	// running when main() exists.
	//
	// So this hack, just hangs the main thread while not really
	// consuming resources.
	//
	//ServerThread->join(); // Forever...
	HangMain.lock();
	HangMain.lock();
	//pthread_exit(nullptr);

	return(0);
}
