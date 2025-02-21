/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLORER_PHOENIX_AUTHMD5_HPP_
#define _RIVEREXPLORER_PHOENIX_AUTHMD5_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "Commands.hpp"
#include "Register.hpp"
#include "AuthMD5.hpp"
#else
#include <RiverExplorer/Phoenix/Commands.hpp>
#include <RiverExplorer/Phoenix/Register.hpp>
#include <RiverExplorer/Phoenix/AuthMD5.hpp>
#endif

namespace RiverExplorer::Phoenix
{
	class CppAuthMD5
		: public PluginBase
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
		 * When a AUTHMD5 command comes in, call this method.
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
		 * Initialize the PreAuth and PostAuth vectors for Ping.
		 */
		virtual void InitalizeKnown();
		
	};
}
#endif // _RIVEREXPLORER_PHOENIX_AUTHMD5_HPP_
