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
	NewNotSupported(CommandSequence ID)
	{
		CmdPacket * Results = new CmdPacket();

		Results->Sequence = ID;
		Results->Data.Cmd = NotSupported_Cmd;

		return(Results);
	}
}
