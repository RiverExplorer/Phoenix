/**                                                                             
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed un\
der CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC                      
 */


#include <Commands.hpp>
#include <CmdNotSupported.hpp>

namespace RiverExplorer::Phoenix
{
	CmdPacket *
	NewNotSupported(CommandID ID)
	{
		CmdPacket * Results = new CmdPacket();

		Results->ID = ID;
		Results->Cmd = NotSupported_Cmd;
		Results->Data.Len = 0;
		Results->Data.Data = nullptr;

		return(Results);
	}
}
