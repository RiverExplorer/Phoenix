/**                                                                             
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed un\
der CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer LLC
 */

#include "CppCapability.hpp"

namespace RiverExplorer::Phoenix
{
	std::vector<Register::PluginCapability*>  CppCapabilityPre::PreAuth;
	std::vector<Register::PluginCapability*>  CppCapabilityPre::PostAuth;

	std::vector<Register::PluginCapability*>  CppCapabilityPost::PreAuth;
	std::vector<Register::PluginCapability*>  CppCapabilityPost::PostAuth;

	bool
	CppCapabilityPre::Callback(int Fd, Command * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		return(Results);
	}
	
	bool
	CppCapabilityPost::Callback(int Fd, Command * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		return(Results);
	}
	
}
