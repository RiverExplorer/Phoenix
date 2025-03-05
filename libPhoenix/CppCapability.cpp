/**
 * Project: Phoenix
 * Time-stamp: <2025-03-03 12:17:05 doug>
 * 
 * @file CppCapability.cpp
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

/**
 * Project: Phoenix
 * Time-stamp: <2025-03-03 09:39:26 doug>
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
	std::vector<Register::PluginCapability*>  CapabilityCommandPre::PreAuth;
	std::vector<Register::PluginCapability*>  CapabilityCommandPre::PostAuth;
	
	std::vector<Register::PluginCapability*>  CapabilityCommandPost::PreAuth;
	std::vector<Register::PluginCapability*>  CapabilityCommandPost::PostAuth;

	CapabilityCommand::CapabilityCommand(SEQ_t SEQ, CMD_e Cmd)
	{
		Sequence = SEQ;
		Payload.Cmd = Cmd;

		return;
	}

	CMD_e
	CapabilityEntryBase::Cmd() const
	{
		return(Capability);
	}

	CMD_e
	CapabilityCommand::Cmd() const
	{
		return(Payload.Cmd);
	}

	SEQ_t
	CapabilityCommand::SEQ() const
	{
		return(Sequence);
	}

	void
	CapabilityCommand::Add(CapabilityEntryBase * NewOne)
	{
		_Entries.push_back(NewOne);
		
		return;
	}

	CapabilityEntryBase *
	CapabilityCommand::operator[](size_t Offset) const
	{
		CapabilityEntryBase * Results = nullptr;

		if (_Entries.size() > Offset) {
			Results = _Entries[Offset];
		}
		
		return(Results);
	}

	size_t
	CapabilityCommand::size() const
	{
		return(_Entries.size());
	}
		
	CapabilityCommandPre::CapabilityCommandPre(SEQ_t Seq)
		: CapabilityCommand(Seq, CAPABILITY_PRE)
	{
		/*EMPTY*/
		return;
	}

	CapabilityCommandPre::~CapabilityCommandPre()
	{
		/*EMPTY*/
		return;
	}

	bool
	CapabilityCommandPre::Callback(int Fd, Command * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		return(Results);
	}

	CapabilityCommandPost::CapabilityCommandPost(SEQ_t Seq)
		: CapabilityCommand(Seq, CAPABILITY_POST)
	{
		/*EMPTY*/
		return;
	}

	CapabilityCommandPost::~CapabilityCommandPost()
	{
		/*EMPTY*/
		return;
	}

	bool
	CapabilityCommandPost::Callback(int Fd, Command * Pkt, XDR * ReadXdrs)
	{
		bool Results = false;

		return(Results);
	}

	VendorIDCapability::VendorIDCapability(char * VendorString)
	{
		Capability = VENDOR_ID;
		CapabilityEntry_u.VendorIDCapability.ID = VendorString;

		return;
	}

	const char *
	VendorIDCapability::VendorString() const
	{
		return(CapabilityEntry_u.VendorIDCapability.ID);
	}
	
	VendorIDCapability::~VendorIDCapability()
	{
		/*EMPTY*/
		return;
	}

	void
	VendorIDCapability::InitializeKnown()
	{
		/**@todo*/
	}
	
	VendorCapability::VendorCapability(uint32_t VendorCapabilityID,
																		 uint8_t * VData,
																		 Phoenix::Length Count)
	{
		// Make sure that the VENDOR_BIT is set.
		//
		VendorCapabilityID |= CMD_VENDOR_MASK;
		Capability = (CMD_e)VendorCapabilityID;
		CapabilityEntry_u.VendorCapability.Data.Data = VData;
		CapabilityEntry_u.VendorCapability.Data.Len = Count;
		
		return;
	}

	VendorCapability::~VendorCapability()
	{
		/*EMPTY*/
		return;
	}

	void
	VendorCapability::InitializeKnown()
	{
		/**@todo*/
	}
	
	void
	VendorCapability::GetData(uint8_t *& Data, Length & Len) const
	{
		Data = CapabilityEntry_u.VendorCapability.Data.Data;
		Len = CapabilityEntry_u.VendorCapability.Data.Len;

		return;
	}
	
}
