/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

/**
 * THIS TEST CODE is a very simple Phoenix client.
 * It runs one thread for I/O and another for user control.
 */

#include <RiverExplorer/Phoenix/Client.hpp>
#include <RiverExplorer/Phoenix/PhoenixServer.hpp>
#include <RiverExplorer/Phoenix/PhoenixEvent.hpp>
#include <RiverExplorer/Phoenix/IPPeer.hpp>
#include <RiverExplorer/Phoenix/Capability.hpp>
#include <RiverExplorer/Phoenix/CppCapability.hpp>
#include <RiverExplorer/Phoenix/HostName.hpp>

#include <exception>

#ifndef W64
#include <arpa/inet.h>
#include <netdb.h>
#endif

uint16_t						TestPort = 6112;

/**
 * The VENDOR_ID value will be used.
 * @note
 * There is NO defined format for this value.
 * This is just what I used.
 */
const char * const	RiverExplorerVendor = "RiverExplorer:1.0";

/**
 * Just random text that is used to test a made up Vendor Command.
 */
const char * const	VendorCommandBlob = "The brown fox jumps over the lazy dog";

/**
 * We are only going to be using the local interfaces for testing.
 * This help make sure any bugs found during testing do not
 * effect the local or remote networks.
 */
const char * const	TestDevice = "lo";

/**
 * The host where the Phoenix server we are test is already running on.
 */
const char * const	TestHost = "localhost";

/**
 * A bogus test account name.
 */
const char * const	TestAccount = "noreply@RiverExplorer.games";

/**
 * A bogus test account password.
 */
const char * const	TestPassword = "My2Bad3Password";

using namespace RiverExplorer;

const char * const ClientName = "PhoenixTestClient1";

/**
 * Commands we issued.
 * They get removed when they are complete.
 */
std::map<Phoenix::SEQ_t,Phoenix::Command*> PendingCommands;
std::mutex PendingCommandsMutex;
Phoenix::SEQ_t	NextSeqToUse = 0;

bool
GotCapabilityPre(int Fd, Phoenix::Command * Pkt, XDR * ReadXdrs)
{
	Phoenix::Log::PrintInformation("Got CAPABILITY_PRE from Server on FD:%d", Fd);

	return(true);
}

int
main(int /*argc*/, char ** /*argv*/)
{
	unlink("Phoenix.log"); // Delete the results of previous runs of this test.
	
	Phoenix::Client * OurServer = new Phoenix::Client(ClientName);

	Phoenix::Log::PrintInformation("TestClient Starting");

	Phoenix::Event::Register(Phoenix::CAPABILITY_PRE,	GotCapabilityPre);
																			
	std::vector<Phoenix::Command*>	Capabilities;

	Phoenix::Command * Cap = Phoenix::CppCapabilityPre::New(NextSeqToUse++);

	// When CapVendorID is deleted, it will delete the
	// 2nd argument.
	//
	Phoenix::Command * CapVendorID
		= Phoenix::CppCapabilityVendorID::New(NextSeqToUse++,
																					strdup(RiverExplorerVendor));

	// When CapVendor is deleted, it will delete the
	// 2nd argument. So we duplicate it first.
	//
	uint32_t  BlobLength = strlen(VendorCommandBlob);
	uint8_t * Blob = new uint8_t[BlobLength];

	memcpy(Blob, VendorCommandBlob, BlobLength);

	Phoenix::Command * CapVendor
		= Phoenix::CppCapabilityVendor::New(NextSeqToUse++,
																				(1| 0x80000000),
																				Blob,
																				BlobLength);

	// Lock the pending reply list.
	//
	PendingCommandsMutex.lock();
	
	// We push this first to show that if a client sends
	// vendor specific capabilities, that the vendor-id MUST go first.
	// so that the server can determine if it should process or
	// ignore them. After then, the order does not matter to
	// Phoenix compliant servers. See any vendor extensions
	// for their requirements.
	//
	PendingCommands.insert(std::make_pair(CapVendorID->Sequence, CapVendorID));
	Capabilities.push_back(CapVendorID);

	// Push the CAPABILITY_PRE
	//
	PendingCommands.insert(std::make_pair(Cap->Sequence, Cap));
	Capabilities.push_back(Cap);
	
	// Push the vendor-specific capability.
	//
	PendingCommands.insert(std::make_pair(Cap->Sequence, CapVendor));
	Capabilities.push_back(CapVendor);

	// Unlock the pending reply list.
	//
	PendingCommandsMutex.unlock();

	// This connects to the server, and send the pre authentication
	// Capabilities followed by the authentication to the server.
	//
	// This call sends the pre authentication capabilities
	// upon connection, then returns. When the server pre authentication
	// capabilities returns, then if it includes AUTHMD5, then it
	// sends the AUTHMD5.
	//
	// NOTE:
	// The protocol supports sending authentication before
	// the capabilities, when the client has had a successful authentication
	// using the same method to the same server.
	// That is not being tested here.
	//
	struct addrinfo * ServerAddress = nullptr;
	
	int ServerFd = Phoenix::HostName::ConnectTcp(TestHost,
																							 TestPort,
																							 ServerAddress);

	if (ServerFd == -1) {
		Phoenix::Log::PrintError("ConnectTcp() failed - Unable to connect"
														 " to server '%s' at port '%u'",
														 TestHost,
														 TestPort);
		
	} else {
		
		
	}

	Phoenix::Log::PrintInformation("Exiting");
	return(0);
}
