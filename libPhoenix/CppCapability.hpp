/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
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
	 * @class CapabilityCommand CppCapability.hpp <RiverExplorer/Phoenix/CppCapability.hpp>
	 * A CAPABILITY command that holds zero or more capabilities.
	 */
	class CapabilityCommand
		: public OneCommand
	{
	public:

		/**
		 * Add a new capability entry to the list.
		 *
		 * @param NewOne The new Capability to add.
		 */
		virtual void Add(CapabilityEntry * NewOne) = 0;

		/**
		 * Get the SEQ_e in this command.
		 */
		SEQ_t	SEQ() const;

		/**
		 * Get the CMD_e.
		 *
		 * Get the command for which entry.
		 */
		CMD_e operator[](size_t Offset) const;

		/**
		 * The number of entries in _Entries.
		 */
		size_t size() const;
		
	private:
		/**
		 * CapabilityCommand - Constructor.
		 *
		 * @param SEQ The sequence number.
		 */
		CapabilityCommand();

		/**
		 * All capability entries in this command.
		 */
		std::vector<CapabilityEntry*> _Entries;
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
		 * Create a new CapabilityCommandPre object.
		 *
		 * @param SEQ The command sequence number.
		 */
		Command * New(SEQ_t SEQ);

		/**
		 * When a the Capability command comes in, call this method.
		 *
		 * @param Fd the associated socket file descriptor.
		 *
		 * @param Pkt The incomming packet.
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
		CapabilityCommandPre();
	};

	class CapabilityCommandPost
		: public CapabilityCommand
	{
	public:
		
		/**
		 * Create a new CapabilityCommandPost object.
		 *
		 * @param SEQ The command sequence number.
		 */
		Command * New(SEQ_t SEQ);
		
		/**
		 * When a the Capability command comes in, call this method.
		 *
		 * @param Fd the associated socket file descriptor.
		 *
		 * @param Pkt The incomming packet.
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
	
	class CppCapabilityVendor
		: public Vendor
	{
	public:
		/**
		 * Make a Vendor CAPABILITY.
		 *
		 * @param VendorCapabilityID The value to use for the
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
		static CapabilityEntry * New(uint32_t VendorCapabilityID,
																 uint8_t * Data,
																 Phoenix::Length Count);

		/**
		 * Initialize the PreAuth and PostAuth vectors for Capability.
		 */
		virtual void InitalizeKnown() = 0;

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
	
	class CppCapabilityVendorID
		: public VendorID
	{
	public:
		/**
		 * Make a Vendor CAPABILITY.
		 *
		 * @param VendorCapabilityID The value to use for the
		 * vendor capability.
		 *
		 * @note
		 * VendorString will be free()'d when the Command is
		 * deleted. So, duplicate it, if you need to keep the data.
		 */
		static CapabilityEntry * New(char * ID);

		/**
		 * Initialize the PreAuth and PostAuth vectors for Capability.
		 */
		virtual void InitalizeKnown();

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
