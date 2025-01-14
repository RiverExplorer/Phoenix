
#ifndef _RIVEREXPLORER_PHOENIX_REGISTER_HPP_
#define _RIVEREXPLORER_PHOENIX_REGISTER_HPP_

#include <Commands.hpp>
#include <CmdNotSupported.hpp>
#include <IO.hpp>

#include <map>
#include <mutex>
#include <list>
#include <deque>
#include <vector>

namespace RiverExplorer::Phoenix
{
	/**
	 * @class CmdRegister Register.hpp <RiverExplorer/Phoenix/Register.hpp>
	 *
	 * -Register a method that handles a specific command.
	 * -Dispatch incomming packets to the correct function.
	 *
	 * An 'PhoenixEvent' is a string.
	 * These events are sent to all registered listeners.
	 * These events can only be sent by registered senders.
	 *
	 * Built in PhoenixEvent's include:
	 *
	 * -"New-Clinet-Connected"
	 * -"Client-Disconnected"
	 * -"Account-Authenticated"
	 * -"Account-Logout"
	 *
	 * More can be added ...
	 */
	class  Register
	{
	public:

		/**
		 * For each capability supportd by this command,
		 * a callback and capability must be supplied.
		 */
		struct PluginCapability
		{
			/**
			 * The name of the plugin to send when the pre
			 * authentication capability is sent.
			 */
			const char * CapabilityName;

			/**
			 * The callback to call when invoked.
			 */
			CommandCallback Cb;

			/**
			 * Events I generate.
			 * A NIL terminated array.
			 */
			const char * EventsIGenerate;
			/**
			 * Events I want to receive
			 * A NIL terminated array.
			 */
			const char * EventsINeed;
		};
		
		/**
		 * Register a callback.
		 * More than one function can be registred for a command.
		 * The order they are called is undefined and unreliable.
		 * All functions will be called that have registered for a command.
		 *		 
		 * @param Cmd The command to register.
		 *
		 * @param Cb The callback to call.
		 *
		 * @param PreAuth A list of pre authentaction capabilities
		 * to list during the inital connection suppored by this
		 * callback. The list may be empty when none.
		 *
		 * @param PostAuth A list of post authentaction capabilities
		 * to list during the inital connection suppored by this
		 * callback. The list may be empty when none.
		 */
		static bool RegisterCallback(Command_e Cmd,
																 CommandCallback Cb,
																 std::vector<PluginCapability*> & PreAuth,
																 std::vector<PluginCapability*> & PostAuth);
																 

		/**
		 * A new packet came in, here is it, call
		 * the associated routine.
		 *
		 * @param Fd The file descriptor it came in on.
		 *
		 * @param Pkt The packet received.
		 */
		static void Dispatch(int Fd, CmdPacket * Pkt);

		/**
		 * Initialize all known commands.
		 */
		static void InitializeKnown();
		
	private:
		/**
		 * The list of functions registered to handle commands.
		 *
		 * @note
		 * This is a one to many mapping. More than one function
		 * may register for a command.
		 */
		static std::multimap<Command_e, CommandCallback> _Callbacks;

		/**
		 * A mutex lock for Callbacks.
		 * Keeps only one hand in the pot at a time.
		 */
		static std::mutex		_CallbackMutex;

	};

	struct PluginBase
	{
	public:

		/**
		 * Initialize the plugin.
		 */
		virtual void InitializePlugin() = 0;
	};
}

#endif // _RIVEREXPLORER_PHOENIX_REGISTER_HPP_
