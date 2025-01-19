/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLORER_PHOENIX_CPPCAPABILITY_HPP_
#define _RIVEREXPLORER_PHOENIX_CPPCAPABILITY_HPP_

#include <Commands.hpp>
#include <Register.hpp>
#include <CmdCapability.hpp>

namespace RiverExplorer::Phoenix
{
	class CppCapability
	{
	public:
		
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

		/**
		 * When a Capability command comes in, call this method.
		 *
		 * @param Fd the associated socket file descriptor.
		 *
		 * @param Pkt The incomming packet.
		 *
		 * @param ReadXdrs The XDR input stream for the still encoded
		 * XDR data.
		 */
		static bool Callback(int Fd, CmdPacket * Pkt, XDR * ReadXdrs);

		/**
		 * Initialize the PreAuth and PostAuth vectors for Capability.
		 */
		virtual void InitalizeKnown();
	};
}
#endif // _RIVEREXPLORER_PHOENIX_CPPCAPABILITY_HPP_
