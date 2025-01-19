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
		Ping	APing;
		
		if (xdr_Ping(ReadXdrs, &APing)) {
			if (APing.WithReply) {
				// Send a ping with the same CommandID back.
				// With 'WithReply' set to false.
				//
				APing.WithReply = false;
				XDR Mem;
				uint64_t Size = sizeof(Ping) * 2;
				char * Chunk = new char[Size];

				xdrmem_create(&Mem, Chunk, (int)Size, XDR_ENCODE);
				
				// This signature of QOutbound() copies what it needs.
				//
				IO::QOutbound(Fd,	Pkt->ID, Ping_Cmd, (uint8_t*)Chunk, Size);
				xdr_free((xdrproc_t)xdr_Ping, Chunk);
				delete[] Chunk;
				xdr_destroy(&Mem);
				Results = true;
			}
		}
		
		return(Results);
	}
	
}
