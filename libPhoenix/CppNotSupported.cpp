/**                                                                             
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed un\
der CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC                      
 */


#include "Commands.hpp"
#include "CmdNotSupported.hpp"
#include "PacketBody.hpp"

namespace RiverExplorer::Phoenix
{
	Command *
	NewNotSupported(SEQ_t ID)
	{
		Command * Results = new Command();

		Results->Payload.Cmd = NOT_SUPPORTED;
		Results->Sequence = ID;
		

		return(Results);
	}
}
