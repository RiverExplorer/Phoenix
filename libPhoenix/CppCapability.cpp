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
	
	Command *
	CppCapabilityPre::New(SEQ_t Sequence)
	{
		Command * Results = new Command();

		Results->Sequence = Sequence;
		Results->Payload.Cmd = Phoenix::CAPABILITY_PRE;
			
		return(Results);
	}

	Command *
	CppCapabilityPost::New(SEQ_t Sequence)
	{
		Command * Results = new Command();

		Results->Sequence = Sequence;
		Results->Payload.Cmd = Phoenix::CAPABILITY_POST;
			
		return(Results);
	}

	Command *
	CppCapabilityVendorID::New(SEQ_t Sequence, char * VendorString)
	{
		Command * Results = new Command();

		Results->Payload.Cmd = Phoenix::VENDOR_ID;
		Results->Sequence = Sequence;
		Results->Payload.OneCommand_u.VendorIDCmd = VendorString;

		return(Results);
	}

	Command *
	CppCapabilityVendor::New(SEQ_t Sequence,
													 uint32_t VendorCommandId,
													 uint8_t * Data,
													 Phoenix::Length Count)
	{
		Command * Results = new Command();

		// Make sure that the VENDOR_BIT is set.
		//
		VendorCommandId |= CMD_VENDOR_MASK;
		Results->Payload.Cmd = (Phoenix::CMD_e)VendorCommandId;
		Results->Sequence = Sequence;
		Results->Payload.OneCommand_u.Vendor.Data.Data = (char*)Data;
		Results->Payload.OneCommand_u.Vendor.Data.Len = Count;
		
		return(Results);
	}
}
