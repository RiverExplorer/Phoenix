
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
