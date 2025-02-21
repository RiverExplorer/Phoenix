/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0 
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#include "PhoenixEvent.hpp"
#include <vector>

namespace RiverExplorer::Phoenix
{
	// The list of all known PhoenixEvent's.
	//
	std::multimap<Event::Event_e,Event::_Register*>	Event::_Registered;

	void
	Event::Register(Event_e Name, EventCallback Cb)
	{
		Event_e Results = Name;

		//
		_Register * NewRegister = new _Register;

		// Fill in the details of the registration map.
		//
		NewRegister->_Event = Name;
		NewRegister->Callback = Cb;

		// Add the new map to the list of all registered maps, by
		// Event ID.
		//
		_Registered.insert(std::make_pair(Results,NewRegister));

		return;
	}
	
	void
	Event::Unregister(Event_e ID, EventCallback Cb)
	{
		std::multimap<Event_e,_Register*>::iterator It;

		_Register * Item;

		// A list of iterators to be erased.
		//
		std::vector<std::multimap<Event_e,_Register*>::iterator> ToBeErased;
		
		// Unregister the callback
		//
		for (It = _Registered.begin(); It != _Registered.end(); It++) {
			Item = It->second;

			// If Cb is provided, unregister the specific Callback.
			//
			if (Cb != nullptr) {
				if (Item->_Event == ID && Item->Callback == Cb) {
					ToBeErased.push_back(It);
					break;
				}
			} else {
				// Erase all callbacks for ID.
				//
				if (Item->_Event == ID) {
					ToBeErased.push_back(It);
					break;
				}
			}
		}

		// Now remove the ones we collected for erasure.
		//
		std::vector<std::multimap<Event_e,_Register*>::iterator>::iterator EraseIt;

		for (EraseIt = ToBeErased.begin()
					 ; EraseIt != ToBeErased.end()
					 ; EraseIt++) {

			Item = (*EraseIt)->second;
			delete Item;
			_Registered.erase(*EraseIt);
		}

		return;
	}

	Event::_Register::~_Register()
	{
		// We do not delete it, others may be using it.
		//
		_Event = NoEvent_Event;
		Callback = nullptr;

		return;
	}
	
	bool
	Event::DispatchCallbacks(int Fd, Event_e ID, void * Data)
	{
		bool Results = true;

		// The result set from any match found for ID.
		//
		std::pair<std::multimap<Event_e,_Register*>::iterator,
							std::multimap<Event_e,_Register*>::iterator> Match;

		// The iterator for Match.
		//
		std::multimap<Event_e,_Register*>::iterator MIt;

		// Look for all matches that are callbacks for ID.
		//
		Match = _Registered.equal_range(ID);

		// The found item.
		//
		_Register * Item;

		// The results of any individual callback.
		//
		bool CbResults;

		// For all instances that registered for ID.
		//
		for (MIt = Match.first; MIt != Match.second; MIt++) {

			// Get the details.
			//
			Item = MIt->second;

			// If there is a callback.
			//
			if (Item->Callback != nullptr) {

				// Call the callback.
				//
				CbResults = Item->Callback(Fd, ID, Data); 
				if (!CbResults) {
					// Return false, even if only one returns false.
					//
					Results = false;
				}
			}
		}

		return(Results);
	}

	bool
	Event::InvokeMessage(int Fd, Event_e Event, const char * Msg)
	{
		if (Msg != nullptr) {
			std::string Msg2 = Msg;

			if (Msg2.back() != '\n') {
				Msg2 += '\n';
			}
			Msg = Msg2.c_str();
		} 
		return(DispatchCallbacks(Fd, Event, (void*)Msg));
	}
	
}
