/**
 * Project: Phoenix
 * Time-stamp: <2025-03-03 12:17:36 doug>
 * 
 * @file CppCapability.hpp
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

#ifndef _RIVEREXPLORER_PHOENIX_CPPCAPABILITY_HPP_
#define _RIVEREXPLORER_PHOENIX_CPPCAPABILITY_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "Commands.hpp"
#include "Capability.hpp"
#include "Register.hpp"
#include "Vendor.hpp"
#include "VendorID.hpp"
#else
#include <RiverExplorer/Phoenix/Commands.hpp>
#include <RiverExplorer/Phoenix/Capability.hpp>
#include <RiverExplorer/Phoenix/Register.hpp>
#include <RiverExplorer/Phoenix/Vendor.hpp>
#include <RiverExplorer/Phoenix/VendorID.hpp>
#endif

namespace RiverExplorer::Phoenix
{

	
	/**
	 * Base class for all capability entries.
	 */
	class CapabilityEntryBase
		: public CapabilityEntry
	{
	public:
		
		/**
		 * Initialize the PreAuth and PostAuth vectors for Capability.
		 */
		virtual void InitializeKnown() = 0;

		/**
		 * @return The CAPABILITY CMD_e.
		 */
		CMD_e Cmd() const;
	};
	
	/**
	 * @class CapabilityCommand CppCapability.hpp <RiverExplorer/Phoenix/CppCapability.hpp>
	 * A CAPABILITY command that holds zero or more capabilities.
	 */
	class CapabilityCommand
		: public Command
	{
	public:

		/**
		 * Add a new capability entry to the list.
		 *
		 * @param NewOne The new Capability to add.
		 */
		void Add(CapabilityEntryBase * NewOne);

		/**
		 * Get the SEQ_e in this command.
		 */
		SEQ_t	SEQ() const;

		/**
		 * This the capabilty type CAPABILITY_PRE
		 * or CAPABILITY_POST.
		 */
		CMD_e Cmd() const;
		
		/**
		 * Get the CMD_e.
		 *
		 * Get the command for which entry.
		 */
		CapabilityEntryBase * operator[](size_t Offset) const;

		/**
		 * The number of entries in _Entries.
		 */
		size_t size() const;
		
	protected:
		/**
		 * CapabilityCommand - Constructor.
		 *
		 * @param SEQ The sequence number.
		 *
		 * @param Cmd The command.
		 */
		CapabilityCommand(SEQ_t SEQ, CMD_e Cmd);

	private:
		
		/**
		 * All capability entries in this command.
		 */
		std::vector<CapabilityEntryBase*> _Entries;
	};

	/**
	 * @class CapabilityCommandPre CppCapability.hpp <RiverExplorer/Phoenix/CppCapability.hpp>
	 * A CAPABILITY_PRE command.
	 */
	class CapabilityCommandPre
		: public CapabilityCommand
	{
	public:
		
		/**
		 * CapabilityCommandPre - Constructor.
		 *
		 * @param SEQ The command sequence number.
		 */
		CapabilityCommandPre(SEQ_t SEQ);

		/**
		 * CapabilityCommandPre - Destructor.
		 */
		virtual ~CapabilityCommandPre();
		
		/**
		 * When a the Capability command comes in, call this method.
		 *
		 * @param Fd the associated socket file descriptor.
		 *
		 * @param Pkt The incoming packet.
		 *
		 * @param ReadXdrs The XDR input stream for the still encoded
		 * XDR data.
		 */
		static bool Callback(int Fd, Command * Pkt, XDR * ReadXdrs);

		/**
		 * List of pre authentication capabilities supported by
		 * the plugin.
		 */
		static std::vector<Register::PluginCapability*>  PreAuth;

		/**
		 * List of post authentication capabilities supported by
		 * the plugin.
		 */
		static std::vector<Register::PluginCapability*>  PostAuth;

	private:
	};

	class CapabilityCommandPost
		: public CapabilityCommand
	{
	public:
		
		/**
		 * CapabilityCommandPost - Constructor.
		 *
		 * @param SEQ The command sequence number.
		 */
		CapabilityCommandPost(SEQ_t SEQ);

		/**
		 * CapabilityCommandPost - Destructor.
		 */
		virtual ~CapabilityCommandPost();

		/**
		 * When a the Capability command comes in, call this method.
		 *
		 * @param Fd the associated socket file descriptor.
		 *
		 * @param Pkt The incoming packet.
		 *
		 * @param ReadXdrs The XDR input stream for the still encoded
		 * XDR data.
		 */
		static bool Callback(int Fd, Command * Pkt, XDR * ReadXdrs);

		/**
		 * List of pre authentication capabilities supported by
		 * the plugin.
		 */
		static std::vector<Register::PluginCapability*>  PreAuth;

		/**
		 * List of post authentication capabilities supported by
		 * the plugin.
		 */
		static std::vector<Register::PluginCapability*>  PostAuth;

	private:
		CapabilityCommandPost();
	};

	class VendorCapability
		: public CapabilityEntryBase
	{
	public:
		/**
		 * VendorCapability - Constructor.
		 * Construct a capability entry.
		 *
		 * @param ID The value to use for the
		 * vendor capability.
		 *
		 * @param Data The value to use in the vendor capability.
		 *
		 * @param Count The size of Data.
		 *
		 * @note
		 * VendorString will be free()'d when the Command is
		 * deleted. So, duplicate it, if you need to keep the data.
		 */
		VendorCapability(uint32_t ID,
										 uint8_t * Data,
										 Phoenix::Length Count);

		/**
		 * Get the data and length.
		 *
		 * @param[out] Data A reference to a (uint8_t*) that
		 * will be set to point to where the data is.
		 *
		 * @param[out] Count How many octets are in Data.
		 */
		void		GetData(uint8_t *& Data, Phoenix::Length & Count) const;
		
		/**
		 * VendorCapability - Destructor.
		 */
		virtual ~VendorCapability();

		/**
		 * Initialize the PreAuth and PostAuth vectors for Capability.
		 */
		virtual void InitializeKnown();

		/**
		 * We register for the ClientAuthenticated_s event.
		 * When they happen, we Q up the VENDOR_ID.
		 *
		 * @param Fd The associated file descriptor.
		 *
		 * @param ID The PhoenixEvent::ID.
		 *
		 * @param Data A (void*) to (Phoenix::IPPeer*).
		 *
		 * @return Always returns true as this method
		 * never stops the connection.
		 */
		static bool ClientAuthenticated(int Fd,
																		Event::Event_e ID,
																		void * VPeerInfo);
	};
	
	class VendorIDCapability
		: public CapabilityEntryBase
	{
	public:
		/**
		 * VendorIDCapability - Constructor.
		 * Construct a capability entry.
		 *
		 * @param VendorString The vendor ID string.
		 *
		 * @note
		 * VendorString will be free()'d when the Command is
		 * deleted. So, duplicate it, if you need to keep the data.
		 */
		VendorIDCapability(char * VendorString);

		/**
		 * Get the VendorString.
		 *
		 * @return The vendor string.
		 */
		const char * VendorString() const;
		 
		/**
		 * CppCapabilityVendorID - Destructor.
		 */
		virtual ~VendorIDCapability();
		
		/**
		 * Initialize the PreAuth and PostAuth vectors for Capability.
		 */
		virtual void InitializeKnown();

		/**
		 * We register for the ClientAuthenticated_s event.
		 * When they happen, we Q up the VENDOR_ID.
		 *
		 * @param Fd The associated file descriptor.
		 *
		 * @param ID The PhoenixEvent::ID.
		 *
		 * @param Data A (void*) to (Phoenix::IPPeer*).
		 *
		 * @return Always returns true as this method
		 * never stops the connection.
		 */
		static bool ClientAuthenticated(int Fd,
																		Event::Event_e ID,
																		void * VPeerInfo);
	};
}
#endif // _RIVEREXPLORER_PHOENIX_CPPCAPABILITY_HPP_
