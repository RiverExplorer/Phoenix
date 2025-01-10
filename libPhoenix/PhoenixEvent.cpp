
#include "PhoenixEvent.hpp"

namespace RiverExplorer::Phoenix
{
	std::map<std::string,Event::_EventMap*>		Event::_EventByName;
	std::map<Event::EventID,Event::_EventMap*>Event::_EventByID;
	std::multimap<Event::EventID,Event::_Register*>	Event::_Registered;
	uint64_t																	Event::_NextEventID = 1;

	Event::EventID
	Event::Register(std::string Name, EventCallback Cb)
	{
		EventID Results = 0;

		// Is Name already known?
		//
		std::map<std::string,_EventMap*>::iterator ByNameIt;
		_EventMap * Map = nullptr;
		
		ByNameIt = _EventByName.find(Name);

		if (ByNameIt != _EventByName.end()) {
			Results = ByNameIt->second->ID;
			Map = ByNameIt->second;
			
		} else {
			Results = _NextEventID++;
			Map = new _EventMap();
			Map->Name = Name;
			Map->ID = Results;
		}
		_Register * NewRegister = new _Register;

		NewRegister->EventMap = Map;
		NewRegister->Callback = Cb;
		_EventByName.insert(std::make_pair(Name,Map));
		_EventByID.insert(std::make_pair(Results,Map));
		_Registered.insert(std::make_pair(Results,NewRegister));

		return(Results);
	}
	
	void
	Event::Unregister(EventID ID, EventCallback Cb)
	{
		std::multimap<EventID,_Register*>::iterator It;

		_Register * Item;
		
		for (It = _Registered.begin(); It != _Registered.end(); It++) {
			Item = It->second;

			if (Item->EventMap->ID == ID && Item->Callback == Cb) {
				_Registered.erase(It);
				delete Item;
				break;
			}
		}

		return;
	}

	Event::_EventMap::~_EventMap()
	{
		Name = "";
		ID = 0;

		return;
	}

	Event::_Register::~_Register()
	{
		// We do not delete it, others may be using it.
		//
		EventMap = nullptr;
		Callback = nullptr;

		return;
	}

	void
	Event::Invoke(int Fd, EventID ID, void * Data)
	{
		std::pair<std::multimap<EventID,_Register*>::iterator,
							std::multimap<EventID,_Register*>::iterator> Match;

		std::multimap<EventID,_Register*>::iterator MIt;
		
		Match = _Registered.equal_range(ID);

		_Register * Item;

		for (MIt = Match.first; MIt != Match.second; MIt++) {
			Item = MIt->second;

			if (Item->EventMap->ID == ID) {
				if (Item->Callback != nullptr) {
					Item->Callback(Fd, ID, Data); // For all instances that registered.
				}
			}
		}

		return;
	}
	
}
