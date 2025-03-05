/**
 * Project: Phoenix
 * Time-stamp: <2025-03-03 11:50:11 doug>
 *
 * @file TestClient.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

/**
 * THIS TEST CODE is a very simple Phoenix client.
 * It runs one thread for I/O and another for user control.
 *
 * @note
 * This is not an example of how a client would be written.
 * This test code tests the lower level API that is available
 * and not generally used directly by clients.
 */
#include "TestClient.hpp"

#include <RiverExplorer/Phoenix/Client.hpp>
#include <RiverExplorer/Phoenix/PhoenixServer.hpp>
#include <RiverExplorer/Phoenix/PhoenixEvent.hpp>
#include <RiverExplorer/Phoenix/IPPeer.hpp>
#include <RiverExplorer/Phoenix/Capability.hpp>
#include <RiverExplorer/Phoenix/CppCapability.hpp>
#include <RiverExplorer/Phoenix/HostName.hpp>
#include <RiverExplorer/Phoenix/Configuration.hpp>

#include <exception>
#include <fstream>
#include <iostream>

#ifndef W64
#include <arpa/inet.h>
#include <netdb.h>
#endif

using namespace RiverExplorer::Phoenix;

/**
 * The VENDOR_ID value will be used.
 * @note
 * There is NO defined format for this value.
 * This is just what I used.
 */
const char * const	RiverExplorerVendor = "RiverExplorer.Phoenix.20250215";

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
const char * const	TestPassword = "ABadPassword123!@#";

/**
 * Commands we issued.
 * They get removed when they are complete.
 */
const char							* ProgramName = nullptr;
std::map<SEQ_t,Command*>	PendingCommands;
std::mutex								PendingCommandsMutex;
SEQ_t											NextSeqToUse = 0;
Client									*	OurClient = nullptr;
Configuration::Server		* OurServer = nullptr;
Command									* CapPre = nullptr;

VendorIDCapability			* VendorIDCap = nullptr;
VendorCapability				*	VendorExtension1Cap = nullptr;

CapabilityCommandPre		* PreAuthCapability = nullptr;
CapabilityCommandPost		* PostAuthCapability = nullptr;

std::vector<Command*>			Capabilities;
std::vector<Configuration::Server*> ServerList;

void
ClientTest::setUp()
{
	TestHost = "localhost";
	TestPort = 6112;

	return;
}

void
ClientTest::tearDown()
{
}

void
ClientTest::GetServerListTest()
{
	// Get the list of all configured servers this client knows about.
	//
	ServerList = Configuration::GetServers();
	CPPUNIT_ASSERT(ServerList.size() == 0);

	return;
}

void
ClientTest::DeleteServerTest1()
{
	DeleteServerTest(0);
}

void
ClientTest::DeleteServerTest2()
{
	DeleteServerTest(1);
}

void
ClientTest::DeleteServerTest(size_t Initial)
{
	// Check for a 'localhost' configuration.
	//
	std::vector<Configuration::Server*>::const_iterator ServerIt;

	
	ServerList = Configuration::GetServers();

	CPPUNIT_ASSERT(ServerList.size() == Initial);
	
	Configuration::Server * Host;

	// Look for 'localhost' so we can delete (testing) , then add it (testing)
	//
	for (ServerIt = ServerList.cbegin()
				 ; ServerIt != ServerList.cend()
				 ; ServerIt++) {

		Host = *ServerIt;

		if (Host->ServerName() == "Local System") {
			// Delete any old entry.
			//
			Configuration::DeleteServer(Host);

			// And remove it from our list.
			//
			ServerList.erase(ServerIt);
			break;
		}
	}

	ServerList = Configuration::GetServers();
	CPPUNIT_ASSERT(ServerList.size() == 0);

	return;
}

void
ClientTest::AddServerTest()
{
	OurServer = Configuration::NewServer("localhost",
																			 "Local System",
																			 6112,
																			 AUTHMD5,
																			 "BadAccount",
																			 "BadPassword");

	CPPUNIT_ASSERT(OurServer != nullptr);

	CMD_e Method = (CMD_e)0xff;
	const char * Account = nullptr;
	const char * Pw = nullptr;
	
	OurServer->GetAuth(Method, Account, Pw);
	
	CPPUNIT_ASSERT(Method == AUTHMD5);
	CPPUNIT_ASSERT(Account != nullptr);
	CPPUNIT_ASSERT(strcmp(Account, "BadAccount") == 0);
	CPPUNIT_ASSERT(Pw != nullptr);
	CPPUNIT_ASSERT(strcmp(Pw, "BadPassword") == 0);
	CPPUNIT_ASSERT(OurServer->Position() == 0);
	CPPUNIT_ASSERT(OurServer->HostOrIp() == "localhost");
	CPPUNIT_ASSERT(OurServer->Port() == 6112);
								
	ServerList = Configuration::GetServers();
	CPPUNIT_ASSERT(ServerList.size() == 1);

	return;
}

void
ClientTest::ClientConstructorTest()
{
	OurClient = new Client(ProgramName);
	CPPUNIT_ASSERT(OurClient != nullptr);
	CPPUNIT_ASSERT(OurClient->ProgramName() == ProgramName);
	CPPUNIT_ASSERT(OurClient->GetLogFp() != nullptr);

	Log::PrintInformation("% Starting.", ProgramName);

	return;
}

void
ClientTest::CreateCapabilityPreConstructorTest()
{
	PreAuthCapability = new CapabilityCommandPre(NextSeqToUse);

	CPPUNIT_ASSERT(PreAuthCapability != nullptr);
	CPPUNIT_ASSERT(PreAuthCapability->SEQ() == NextSeqToUse);
	CPPUNIT_ASSERT(PreAuthCapability->Cmd() == CAPABILITY_PRE);
	
	NextSeqToUse += 2;

	return;
};

void
ClientTest::CreateCapabilityPostConstructorTest()
{
	PostAuthCapability = new CapabilityCommandPost(NextSeqToUse);

	CPPUNIT_ASSERT(PostAuthCapability != nullptr);
	CPPUNIT_ASSERT(PostAuthCapability->SEQ() == NextSeqToUse);

	NextSeqToUse += 2;

	return;
};

void
ClientTest::CreateCapabilityVendorIDTest()
{
	VendorIDCap = new VendorIDCapability(strdup(RiverExplorerVendor));

	CPPUNIT_ASSERT(VendorIDCap != nullptr);
	CPPUNIT_ASSERT(VendorIDCap->Cmd() == VENDOR_ID);

	CPPUNIT_ASSERT(strcmp(VendorIDCap->VendorString(), RiverExplorerVendor) == 0);

	// We push this first to show that if a client sends
	// vendor specific capabilities, that the vendor-id MUST go first.
	// so that the server can determine if it should process or
	// ignore them. After then, the order does not matter to
	// Phoenix compliant servers. See any vendor extensions
	// for their requirements.
	//
	CPPUNIT_ASSERT(PreAuthCapability != nullptr);
	PreAuthCapability->Add(VendorIDCap);

	CPPUNIT_ASSERT(PreAuthCapability->size() == 1);

	VendorIDCapability * Saved = dynamic_cast<VendorIDCapability*>((*PreAuthCapability)[0]);

	CPPUNIT_ASSERT(Saved == VendorIDCap);

	return;
}

void
ClientTest::CreateCapabilityPreTest()
{
	CapPre = new CapabilityCommandPre(NextSeqToUse);

	CPPUNIT_ASSERT(CapPre != nullptr);
	CPPUNIT_ASSERT(CapPre->Payload.Cmd == CAPABILITY_PRE);
	CPPUNIT_ASSERT(CapPre->Sequence == NextSeqToUse);
	
	// Push the CAPABILITY_PRE
	//
	PendingCommandsMutex.lock();
	PendingCommands.insert(std::make_pair(CapPre->Sequence, CapPre));
	Capabilities.push_back(CapPre);
	CPPUNIT_ASSERT(Capabilities.size() == 2);
	PendingCommandsMutex.unlock();
	
	NextSeqToUse += 2;

	return;
}

void
ClientTest::CreateCapabilityVendorExtension1Test()
{
	// When the vendor extension capability object is deleted,
	// the data will be deleted. So we give it a copy.
	//
	uint32_t  BlobLength = strlen(VendorCommandBlob);
	uint8_t * Blob = new uint8_t[BlobLength];

	memcpy(Blob, VendorCommandBlob, BlobLength);

	// Add the vendor extension BIT (high bit).
	// Or in the made up vendor extension (1).
	//
	uint32_t Ext1Cmd = (1| 0x80000000);

	VendorExtension1Cap	= new VendorCapability(Ext1Cmd,
																						 Blob,
																						 BlobLength);

	uint8_t	* Data = nullptr;
	Length		Count;

	CPPUNIT_ASSERT(VendorExtension1Cap != nullptr);
	CPPUNIT_ASSERT(VendorExtension1Cap->Cmd() == Ext1Cmd);
	VendorExtension1Cap->GetData(Data, Count);
	CPPUNIT_ASSERT(Data == Blob);
	CPPUNIT_ASSERT(Count == BlobLength);
	CPPUNIT_ASSERT(PreAuthCapability != nullptr);
	PreAuthCapability->Add(VendorExtension1Cap);
	CPPUNIT_ASSERT(PreAuthCapability->size() == 2);

	VendorCapability * Saved = dynamic_cast<VendorCapability*>((*PreAuthCapability)[1]);

	CPPUNIT_ASSERT(Saved == VendorExtension1Cap);
	
	return;
}

void
ClientTest::ConnectToServerTest()
{

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

	int ServerFd = HostName::ConnectTcp(TestHost,
																			TestPort,
																			ServerAddress);

	CPPUNIT_ASSERT(ServerFd > -1);
	if (ServerFd == -1) {
		Log::PrintError("ConnectTcp() failed - Unable to connect"
										" to server '%s' at port '%u'",
										TestHost,
										TestPort);
		
	} else {
		
		
	}

	return;
}

//
// END OF CPP METHODS, START OF CLIENT CODE>
//
bool
GotCapabilityPre(int Fd, Command * Pkt, XDR * ReadXdrs)
{
	Log::PrintInformation("Got CAPABILITY_PRE from Server on FD:%d", Fd);

	return(true);
}

int
main(int /*argc*/, char ** argv)
{
	ProgramName = basename(argv[0]);
	
	// Register to be called with a CAPABILITY_PRE
	// comes in from the server.
	//
	Event::Register(CAPABILITY_PRE,	GotCapabilityPre);

	std::ofstream XmlTestResultFile;
	
	unlink("XMLResults.xml");
	XmlTestResultFile.open("XMLResults.xml", std::ios::out);
	
	CppUnit::TestResultCollector Results;
	CppUnit::TestResult Controller;

	Controller.addListener(&Results);

	CppUnit::BriefTestProgressListener Progress;
	Controller.addListener(&Progress);
	
	CppUnit::TextUi::TestRunner Runner;
	CppUnit::TestFactoryRegistry& Registry
		= CppUnit::TestFactoryRegistry::getRegistry();
	
	Runner.addTest(Registry.makeTest());

	Runner.run(Controller);
	
	CppUnit::TextOutputter Outputter(&Results, std::cerr);
	Outputter.write();

	CppUnit::XmlOutputter XmlOut(&Results, XmlTestResultFile);
	XmlOut.write();
	
	return Results.wasSuccessful() ? 0 : 1;

	Log::PrintInformation("Exiting");
	return(0);
}
