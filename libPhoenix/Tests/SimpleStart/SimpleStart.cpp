
/**
 * This test code is a very simple Phoenix server.
 */
 
#include <RiverExplorer/Phoenix/PhoenixServer.hpp>

uint16_t			TestPort = 6112;
const char *	TestDevice = "lo";

using namespace RiverExplorer;

int
main(int argc, char ** argv)
{
	Phoenix::Server	TheServer;

	std::thread * ServerThread = TheServer.Start(TestPort, TestDevice);

	fprintf(stdout, "ServerThread started.\n");
	
	void * NotUsed = nullptr;
	
	//pthread_join(ServerThread, &NotUsed);
	
	pthread_exit(nullptr);
}
