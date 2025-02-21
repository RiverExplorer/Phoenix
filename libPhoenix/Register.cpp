/**                                                                             
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed un\
der CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC                      
 */


/**
 * -This code takes received packets and dispatches them
 * The any registred function for the command.
 *
 * -Processes any CmdNotSupported packets.
 *
 * -Listens for incomming data.
 *
 * -Sends out outgoing data.
 */
#include "Register.hpp"
#include "IO.hpp"

/** include all 'known' commnds so they can
 * be registered at start up.
 *
 * @see InitializeKnown()
 */
#include "CppPing.hpp"
#include "Capability.hpp"
#include "CppCapability.hpp"
#include "CppAuthMD5.hpp"
#include "CppBye.hpp"

#include <sys/socket.h>

namespace RiverExplorer::Phoenix
{
	/**
	 * The list of functions registered to handle commands.
	 *
	 * @note
	 * This is a one to many mapping. More than one function
	 * may register for a command.
	 */
	std::multimap<CMD_e, CommandCallback> Register::_Callbacks;

	/**
	 * A mutex lock for Callbacks.
	 * Keeps only one hand in the pot at a time.
	 */
	std::mutex		Register::_CallbackMutex;

	static bool KnownInitialized = false;

	static std::vector<Register::PluginCapability*> PreAuthCapabilities;
	static std::vector<Register::PluginCapability*> PostAuthCapabilities;
	
	bool
	Register::RegisterCallback(CMD_e Cmd,
														 CommandCallback Cb,
														 std::vector<Register::PluginCapability*> & Pre,
														 std::vector<Register::PluginCapability*> & Post)
	{
		bool Results = false;

		if (!KnownInitialized) {
			InitializeKnown();
		}
		if (Cb != nullptr) {
			_CallbackMutex.lock();

			_Callbacks.insert(std::make_pair(Cmd, Cb));
			_CallbackMutex.unlock();
			Results = true;
		}

		std::vector<PluginCapability*>::iterator it;

		for (it = Pre.begin(); it != Pre.end(); it++) {
			PreAuthCapabilities.push_back(*it);
		}

		for (it = Post.begin(); it != Post.end(); it++) {
			PreAuthCapabilities.push_back(*it);
		}
		
		return(Results);
	}
	
	void
	Register::Dispatch(int Fd, PacketBody * Pkt)
	{
		if (!KnownInitialized) {
			InitializeKnown();
		}
		// A PacketBody can have 1 or more commands.
		// ...

		/**@todo Register::Dispatch() */
		
		return;
	}

	void
	Register::InitializeKnown()
	{
		RegisterCallback(PING,
										 CppPing::Callback,
										 CppPing::PreAuth,
										 CppPing::PostAuth);
		
		RegisterCallback(CAPABILITY_PRE,
										 CppCapabilityPre::Callback,
										 CppCapabilityPre::PreAuth,
										 CppCapabilityPre::PostAuth);

		RegisterCallback(CAPABILITY_POST,
										 CppCapabilityPost::Callback,
										 CppCapabilityPost::PreAuth,
										 CppCapabilityPost::PostAuth);

		RegisterCallback(AUTHMD5,
										 CppAuthMD5::Callback,
										 CppAuthMD5::PreAuth,
										 CppAuthMD5::PostAuth);
										 
		RegisterCallback(BYE,
										 CppBye::Callback,
										 CppBye::PreAuth,
										 CppBye::PostAuth);
										 

	}

	
}
