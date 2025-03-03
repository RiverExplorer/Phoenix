/**
 * Project: Phoenix
 * Time-stamp: <2025-03-03 01:41:17 doug>
 *
 * @file Capability.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "CppCapability.hpp"

namespace RiverExplorer::Phoenix
{
	bool
	CapabilityCommandPre::Callback(int Fd, Command * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		return(Results);
	}
	
	Command *
	CapabilityCommandPre::New(SEQ_t Sequence)
	{
		Command * Results = new Command();

		Results->Sequence = Sequence;
		Results->Payload.Cmd = Phoenix::CAPABILITY_PRE;
			
		return(Results);
	}

	bool
	CapabilityCommandPost::Callback(int Fd, Command * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		return(Results);
	}

	Command *
	CapabilityCommandPost::New(SEQ_t Sequence)
	{
		Command * Results = new Command();

		Results->Sequence = Sequence;
		Results->Payload.Cmd = Phoenix::CAPABILITY_POST;
			
		return(Results);
	}

	CapabilityEntry *
	CppCapabilityVendorID::New(char * VendorString)
	{
		CapabilityEntry * Results = new CapabilityEntry();

		Results->Capability = VENDOR_ID;
		Results->CapabilityEntry_u.VendorID = VendorString;

		return(Results);
	}
	
	CapabilityEntry *
	CppCapabilityVendor::New(uint32_t VendorCapabilityID,
													 uint8_t * Data,
													 Phoenix::Length Count)
	{
		CapabilityEntry * Results = new CapabilityEntry();

		// Make sure that the VENDOR_BIT is set.
		//
		VendorCapabilityID |= CMD_VENDOR_MASK;
		Results->Capability = (CMD_e)VendorCapabilityID;
		Results->CapabilityEntry_u.VendorCapability.Data.Data =  Data;
		Results->CapabilityEntry_u.VendorCapability.Data.Len = Count;		
		
		return(Results);
	}
}
