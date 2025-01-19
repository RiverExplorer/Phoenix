/**                                                                             
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed un\
der CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC                      
 */


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


