/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLOER_PHOENIX_PHOENIXEVENT_HPP_
#define _RIVEREXPLOER_PHOENIX_PHOENIXEVENT_HPP_

#include <string>
#include <stdint.h>
#include <map>

namespace RiverExplorer::Phoenix
{
	/**
	 * @defgroup PhoenixEvent RiverExplorer::Phoenix::PhoenixEvent
	 *
	 * This library implements a signaling mechanism
	 * that notifies other segments of this code about significant
	 * events. These are called a PhoenixEvent.
	 *
	 * The PhoenixEvent class is a pure virtual base class
	 * because at least one method is defined a pure virtual.
	 * This means that you can not use this class directly.
	 * You must derive from this class and implement your
	 * own PhoenixEvent lists and implantation. This
	 * base class handles the details and processing
	 * as described in the PhoenixEvent class object.
	 *
	 * @note
	 * These are not the same as notifications sent to client
	 * connections.
	 */
	
	/**
	 * @class PhoenixEvent
	 * @section PhoenixEvent The PhoenixEvent object.
	 *
	 * @addtogroup PhoenixEvent
	 *
	 * PhoenixEvent is a pure virtual base class for PhoenixEvents.
	 * 
	 * PhoenixEvent object is a simple INTERNAL event system.
	 * Code can send and receive these events.
	 *
	 * The caller registers for an event by providing
	 * the name of the event, as a string.
	 *
	 * They get back the ID for the event.
	 *
	 * An event name is a string. This string is from
	 * the character set [a-z][A-Z][_-][0-9] .
	 * 
	 * -The event names can be mixed case.
	 * -The event names can start with a number.
	 * -The event names case is ignored when comparing them.
	 * So that "Abc" and "abc" are the same event.
	 *
	 * Example registration when called with the server implementation:
	 * @code{.cpp}
	 *
	 * using RiverExplorer::Phoenix;
	 *
	 * PhoenixEvent::EventID ID = PhoenixEvent::Register(Server::Ready_t, MyServerReadyCallback);
	 *
	 * @endcode
	 *
	 * Now, when the server is ready, it will call
	 * the MyServerReadyCallback() method in your server implementation.
	 */
	class PhoenixEvent
	{
	public:

		/**
		 * A predefined Event name: Log Warning.
		 */
		static const char * const LogWarning_s;
		
		/**
		 * A predefined Event name: Log Error.
		 */
		static const char * const LogError_s;
		
		/**
		 * @addtogroup PhoenixEvent
		 *
		 * An EventID is returned from Register() as a handle
		 * to the specific PhoenixEvent that was registered for.
		 *
		 * It is also used in Unregister() and in the
		 * protected method Dispatch().
		 *
		 * @property EventID
		 * An event ID.
		 */
		typedef uint64_t	EventID;

		/**
		 * @addtogroup PhoenixEvent
		 *
		 * @typedef PhoenixEventCallback
		 *
		 * @addtogroup PhoenixEvent
		 *
		 * @ref PhoenixEventCallback is the callback signature
		 * for when an event happens.
		 *
		 * Example, you write a callback to filter blocked IP address.
		 * It would  return false to tell the caller not to proceed.
		 *
		 * When the server starts, it creates a PhoenixEvent.
		 * In this example, it creates the Server::Ready_s
		 * PhoenixEvent. This is done by calling Register().
		 *
		 * The Server object predefines the Ready_s string to
		 * "ServerReady".
		 * @see Server
		 *
		 * In this example that is from the Server code, it is creating a new
		 * PhoenixEvent that it allows other code to register to receive
		 * a Server::Ready_s
		 * @code
		 *
		 * PhoenixEvent::EventID EventID = PhoenixEvent::Register(Server::Ready_s, nullptr);
		 *
		 * @endcode
		 *
		 * Register() returns the ID of that event type.
		 *
		 * @note
		 * The value of the results do not persist across runs of the
		 * implementation. They are automatically set in
		 * the order they are generated. And this implementation
		 * is threaded, so the assigned ID's can happen in random order.
		 *
		 * By passing in nullptr to the second parameter, it creates
		 * the Server::Ready_s PhoenixEvent for others to use. If the
		 * second parameter had been a method name, it would create
		 * the PhoenixEvent (if it had not already been created),
		 * and it would also register the method provided as one of the
		 * callback methods to be invoked when the
		 * EventID PhoenixEvent happens.
		 *
		 * In your own code in the server implementation,
		 * if you want a notification of when a new client
		 * connects, you would register for Server::NewClientConnection_s .
		 * In this example, you might want to know when a new client
		 * connection happens so you can validate the IP address as not blocked.
		 *
		 * @code{.cpp}
		 *
		 * PhoenixEvent::EventID NewOneID = PhoenixEvent::Register(Server::NewClientConnections_s, MyValidateIPAddress);
		 *
		 * @endcode
		 *
		 * Then each time the server gets a new client connection, it would
		 * call your MyValidateIPAddress method.
		 * @see Server::NewClientConnectins_s for details.
		 *
		 * You MyValidateIPAddress callback would look something
		 * like this:
		 *
		 * @code{.cpp}
		 *
		 * bool MyValidateIPAddress(int Fd, PhoenixEvent::EventID ID, void * Data)
		 * {
		 *		bool Results = true;
		 *
		 *		// If you use this callback from multiple PhoenixEvents,
		 *		// You might want to make sure that (ID == NewOneID). Where
		 *		// NewOneID is the value returned from Register() in the previous
		 *		// example.
		 *		//
		 *		Server::PeerInfo * Peer = static_cast<Server::PeerInfo*>Data;
		 *
		 *		// You would write IsBlockedHosts()
		 *		// Or call any logging or code you need to
		 *		// perform work for a new connection.
		 *		//
		 *		if (IsBlockedHosts(Peer->Addr) {
		 *			Results = false;
		 *		}
		 *
		 *		return(Results);
		 * }
		 *
		 * @endcode
		 *
		 * When any @ref PhoenixEventCallback method type is called,
		 * it is called with
		 * three (3) parameters:
		 *
		 * @param Fd The associated file descriptor.
		 * When (-1) then the file descriptor is unrelated to this event.
		 *
		 * @param ID The associated event ID returned from Register.
		 *
		 * @param Data Optional data, defined by the event.
		 * May be nullptr when the event does not pass any information
		 * to the event handler.
		 *
		 * @return false if the callback does not want the associated
		 * and original action to go forward.
		 *
		 * Read the specific PhoenixEvent description for details
		 * on what it expects when false is returned.
		 *
		 */
		typedef bool (*PhoenixEventCallback)(int Fd, EventID ID, void * Data);
		
		/**
		 * @addtogroup PhoenixEvent
		 *
		 * To get events, you must call Register() for each event.
		 * More than one callback can be registered for the same event.
		 * Example, multiple plugins may want the "New-Client-Connected" event.
		 *
		 * Register() takes 2 parameters:
		 *
		 * @param EventName The event being registered for.
		 *
		 * @param Cb The callback to call when the event happens.
		 * When nullptr, then the event is being initialized by
		 * name for others to use.
		 *
		 * @return The ID of the event.
		 * This is the ID of EventName, not an instance specific
		 * registration ID. All callers with the same EventName
		 * will get back the same ID.
		 *
		 * @note
		 * The value returned does not persist across runs
		 * of the application. And are only valid within the currently
		 * running application. Do not save the results and use it
		 * the next time the application starts.
		 */
		static EventID Register(std::string EventName, PhoenixEventCallback Cb);

		/**
		 * @addtogroup PhoenixEvent
		 *
		 * Call Unregister() to stop receiving calls for an event.
		 * Unregister() takes the same parameters as Register(),
		 * and using the same parameter values that were set
		 * when the associated Register() call was placed.
		 *
		 * @code{.cpp}
		 *
		 * using RiverExplorer::Phoenix;
		 *
		 * PhoenixEvent::EventID ID = PhoenixEvent::Unregister(Server::Ready_s, MyServerReadyCallback);
		 *
		 * @endcode
		 *
		 * @param ID The ID For the event name that Cb will
		 * be unregistered for.
		 *
		 * @param Cb The callback to call when the event happens.
		 * When calling with Cb nullptr, then remove
		 * the event as a valid event.
		 */
		static void Unregister(EventID ID, PhoenixEventCallback Cb);

		/**
		 * @addtogroup PhoenixEvent
		 *
		 * To Invoke an event, call Invoke();
		 *
		 * @note
		 * This Invoke is a pure virtual method in PhoenixEvent.
		 * Derived classes must implement this method.
		 *
		 * This Invoke() takes 3 parameters:
		 *
		 * @param Fd The associated client file descriptor.
		 * Or (-1) when not used.
		 *
		 * @param ID The event ID.
		 *
		 * @param Data Optional event specific data.
		 *
		 * @return false when any called callback wishes processing
		 * to stop.
		 * This does not stop all of the other callbacks from being called.
		 *
		 * Example: When the server connects, it might call a callback
		 * that is blocking a specific IP address and the server
		 * would then terminate the connection.
		 *
		 * Many callbacks may ignore the results. See the specific
		 * PhoenixEvent for details.
		 */
		virtual bool	Invoke(int Fd, EventID ID, void * Data = nullptr) = 0;

		/**
		 * @addtogroup PhoenixEvent
		 *
		 * To Invoke an event, call Invoke();
		 *
		 * This Invoke() takes 2 parameters:
		 *
		 * @param Fd The associated client file descriptor.
		 * Or (-1) when not used.
		 *
		 * @param EName The event name.
		 *
		 * @param Msg The message.
		 *
		 * @return false when any called callback wishes processing
		 * to stop.
		 * This does not stop all of the other callbacks from being called.
		 *
		 * Example: When the server connects, it might call a callback
		 * that is blocking a specific IP address and the server
		 * would then terminate the connection.
		 *
		 * Many callbacks may ignore the results. See the specific
		 * PhoenixEvent for details.
		 */
		static bool InvokeMessage(int Fd, const char * Event, const char * Msg);
		
	protected:

		/**
		 * @addtogroup PhoenixEvent
		 *
		 * Derived classes call Dispatch() to call the callbacks.
		 * Dispatch() takes 3 parameters:
		 *
		 * @note
		 * This method is protected, and only callable from
		 * derived classes
		 *		 
		 *
		 * @param Fd The associated client file descriptor.
		 *
		 * @param ID The event ID.
		 *
		 * @param Data Optional event specific data.
		 *
		 * @return false when any called callback wishes processing
		 * to stop.
		 * This does not stop all of the other callbacks from being called.
		 */
		static bool DispatchCallbacks(int Fd, EventID ID, void * Data = nullptr);
		
	private:

		/**
		 * Information about known events.
		 */
		struct _EventMap
		{
			/**
			 * The name of the event.
			 */
			std::string Name;

			/**
			 * The ID of the event.
			 */
			uint64_t		ID;

			/**
			 * _EventMap - Destructor.
			 */
			~_EventMap();
		};

		/**
		 * Lookup map for PhoenixEvent by name.
		 */
		static std::map<std::string,_EventMap*> _EventByName;

		/**
		 * Lookup map for PhoenixEvent by ID.
		 */
		static std::map<EventID,_EventMap*>	_EventByID;

		/**
		 * List of registered PhoenixEvents.
		 */
		struct _Register
		{
			/**
			 * Link to PhoenixEvent description.
			 */
			_EventMap			*	EventMap;

			/**
			 * The registered callback.
			 */
			PhoenixEventCallback		 Callback;

			/**
			 * _Register - Destructor.
			 */
			~_Register();
		};

		/**
		 * List of all callbacks registered for all PhoenixEvents.
		 *
		 * @note
		 * It is a multimap because more than one callback
		 * can be registered for any specific PhoenixEvent.
		 */
		static std::multimap<EventID,_Register*>	_Registered;

		/**
		 * Use this as the next EventID to return.
		 */
		static uint64_t _NextEventID;
	};

}
#endif // _RIVEREXPLOER_PHOENIX_PHOENIXEVENT_HPP_
