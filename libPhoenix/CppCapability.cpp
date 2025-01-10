
#include "CmdCapability.hpp"
#include <CppCapability.hpp>

namespace RiverExplorer::Phoenix
{
	std::vector<Register::PluginCapability*>  CppCapability::PreAuth;
	std::vector<Register::PluginCapability*>  CppCapability::PostAuth;

	bool
	CppCapability::Callback(int Fd, CmdPacket * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		return(Results);
	}
	
}


