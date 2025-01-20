/**                                                                            
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed uder CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#include "CppPing.hpp"
#include "IO.hpp"

namespace RiverExplorer::Phoenix
{
	std::vector<Register::PluginCapability*>  CppPing::PreAuth;
	std::vector<Register::PluginCapability*>  CppPing::PostAuth;
	
	// Someone said we got a ping packet.
	//
	bool
	CppPing::Callback(int Fd, CmdPacket * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		CmdPacket * Reply = NewPing(Pkt->Sequence);
		
		XDR WriteXdrs;
		uint64_t BlobSize = sizeof(*Reply) * 2;
		char * Blob = new char[BlobSize];

		xdrmem_create(&WriteXdrs, Blob, (int)BlobSize, XDR_ENCODE);
			
		if (xdr_CmdPacket(&WriteXdrs, Reply)) {
			// The data blob is XDR formatted, ready to send.
			// This method copies the data before returning.
			// It is for small amounts of data.
			//
			IO::QOutbound(Fd,	(uint8_t*)Blob, xdr_getpos(&WriteXdrs));

			xdr_free((xdrproc_t)xdr_CmdPacket, Reply);
			xdr_destroy(&WriteXdrs);
			delete[] Blob;
			Results = true;
		}
		
		return(Results);
	}

	CmdPacket * NewPing(CommandSequence Seq)
	{
		CmdPacket	*	Results = new CmdPacket();

		Results->Sequence = Seq;
		Results->Data.Cmd = Ping_Cmd;

		return(Results);
	}
}
