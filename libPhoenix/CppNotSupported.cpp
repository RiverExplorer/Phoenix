
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
