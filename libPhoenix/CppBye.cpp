/**                                                                            
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed uder CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#include "CppBye.hpp"
#include "IO.hpp"

namespace RiverExplorer::Phoenix
{
	std::vector<Register::PluginCapability*>  CppBye::PreAuth;
	std::vector<Register::PluginCapability*>  CppBye::PostAuth;
	
	// Someone said we got a bye packet.
	//
	bool
	CppBye::Callback(int Fd, Command * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		Command * Reply = NewBye(Pkt->Sequence);
		
		XDR WriteXdrs;
		uint64_t BlobSize = sizeof(*Reply) * 2;
		char * Blob = new char[BlobSize];

		xdrmem_create(&WriteXdrs, Blob, (int)BlobSize, XDR_ENCODE);
			
		if (xdr_Command(&WriteXdrs, Reply)) {
			// The data blob is XDR formatted, ready to send.
			// This method copies the data before returning.
			// It is for small amounts of data.
			//
			IO::QOutbound(Fd,	(uint8_t*)Blob, xdr_getpos(&WriteXdrs));

			xdr_free((xdrproc_t)xdr_Command, Reply);
			xdr_destroy(&WriteXdrs);
			delete[] Blob;
			Results = true;
		}
		
		return(Results);
	}

	Command * NewBye(SEQ_t Seq)
	{
		Command	*	Results = new Command();

		Results->Sequence = Seq;
		Results->Payload.Cmd = BYE;

		return(Results);
	}
}
