/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0 
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#include "PhoenixEvent.hpp"
#include <vector>

namespace RiverExplorer::Phoenix
{
	// The list of known PhoenixEvent's by name.
	//
	std::map<std::string,PhoenixEvent::_EventMap*>		PhoenixEvent::_EventByName;
	
	// The list of known PhoenixEvent's by ID.
	//
	std::map<PhoenixEvent::EventID,PhoenixEvent::_EventMap*>PhoenixEvent::_EventByID;
	
	// The list of all known PhoenixEvent's.
	//
	std::multimap<PhoenixEvent::EventID,PhoenixEvent::_Register*>	PhoenixEvent::_Registered;

	// When registring an new event, use this as its ID.
	//
	uint64_t		PhoenixEvent::_NextEventID = 1;

	// Predefined event "LogError".
	//
	const char * const PhoenixEvent::LogError_s = "LogError";

	// Predefined event "LogWarning".
	//
	const char * const PhoenixEvent::LogWarning_s = "LogWarning";

	PhoenixEvent::EventID
	PhoenixEvent::Register(std::string Name, PhoenixEventCallback Cb)
	{
		EventID Results = 0;

		std::map<std::string,_EventMap*>::iterator ByNameIt;
		_EventMap * Map = nullptr;

		// Look to see if Name is already known.
		//
		ByNameIt = _EventByName.find(Name);

		if (ByNameIt != _EventByName.end()) {
			// Name is known, use the existing ID.
			//
			Results = ByNameIt->second->ID;
			Map = ByNameIt->second;
			
		} else {
			// Name was not known, use a new ID.
			// Create a new callback map for it.
			//
			Results = _NextEventID++;	// Get and setup for next.
			Map = new _EventMap();
			Map->Name = Name;
			Map->ID = Results;
		}

		//
		_Register * NewRegister = new _Register;

		// Fill in the details of the registration map.
		//
		NewRegister->EventMap = Map;
		NewRegister->Callback = Cb;

		// Add the new map to the list of callbacks for Name PhoenixEvent.
		// An index by PhoenixEvent Name.
		//
		_EventByName.insert(std::make_pair(Name,Map));

		// Add the new map to the list of callbacks for Name PhoenixEvent.
		// An index by PhoenixEvent ID.
		//
		_EventByID.insert(std::make_pair(Results,Map));

		// Add the new map to the list of all registered maps, by
		// PhoenixEvent ID.
		//
		_Registered.insert(std::make_pair(Results,NewRegister));

		return(Results);
	}
	
	void
	PhoenixEvent::Unregister(EventID ID, PhoenixEventCallback Cb)
	{
		std::multimap<EventID,_Register*>::iterator It;

		_Register * Item;

		// A list of iterators to be erased.
		//
		std::vector<std::multimap<EventID,_Register*>::iterator> ToBeErased;
		
		// Unregister the callback
		//
		for (It = _Registered.begin(); It != _Registered.end(); It++) {
			Item = It->second;

			// If Cb is provided, unregister the specific Callback.
			//
			if (Cb != nullptr) {
				if (Item->EventMap->ID == ID && Item->Callback == Cb) {
					ToBeErased.push_back(It);
					break;
				}
			} else {
				// Erase all callbacks for ID.
				//
				if (Item->EventMap->ID == ID) {
					ToBeErased.push_back(It);
					break;
				}
			}
		}

		// Now remove the ones we collected for erasure.
		//
		std::vector<std::multimap<EventID,_Register*>::iterator>::iterator EraseIt;

		for (EraseIt = ToBeErased.begin()
					 ; EraseIt != ToBeErased.end()
					 ; EraseIt++) {

			Item = (*EraseIt)->second;
			delete Item;
			_Registered.erase(*EraseIt);
		}

		return;
	}

	PhoenixEvent::_EventMap::~_EventMap()
	{
		Name = "";
		ID = 0;

		return;
	}

	PhoenixEvent::_Register::~_Register()
	{
		// We do not delete it, others may be using it.
		//
		EventMap = nullptr;
		Callback = nullptr;

		return;
	}
	
	bool
	PhoenixEvent::DispatchCallbacks(int Fd, EventID ID, void * Data)
	{
		bool Results = true;

		// The result set from any match found for ID.
		//
		std::pair<std::multimap<EventID,_Register*>::iterator,
							std::multimap<EventID,_Register*>::iterator> Match;

		// The iterator for Match.
		//
		std::multimap<EventID,_Register*>::iterator MIt;

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
	PhoenixEvent::InvokeMessage(int Fd, const char * Event, const char * Msg)
	{
		EventID ID;
		std::map<std::string,_EventMap*>::iterator ByNameIt;

		ByNameIt = _EventByName.find(Event);

		if (ByNameIt == _EventByName.end()) {
			ID = Register(Event, nullptr);
		} else {
			ID = ByNameIt->second->ID;
		}

		if (Msg != nullptr) {
			std::string Msg2 = Msg;

			if (Msg2.back() != '\n') {
				Msg2 += '\n';
			}
			Msg = Msg2.c_str();
		} 
		return(DispatchCallbacks(Fd, ID, (void*)Msg));
	}
	
}
