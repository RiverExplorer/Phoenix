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
	 * @defgroup Event RiverExplorer::Phoenix::Event
	 *
	 * This library implements a signaling mechanism
	 * that notifies other segments of this code about significant
	 * events. These are called a Event.
	 *
	 * The Event class is a pure virtual base class
	 * because at least one method is defined a pure virtual.
	 * This means that you can not use this class directly.
	 * You must derive from this class and implement your
	 * own Event lists and implantation. This
	 * base class handles the details and processing
	 * as described in the Event class object.
	 *
	 * @note
	 * These are not the same as notifications sent to client
	 * connections.
	 */
	
	/**
	 * @class Event
	 * @section Event The Event object.
	 *
	 * @addtogroup Event
	 *
	 * Event is a pure virtual base class for Events.
	 * 
	 * Event object is a simple INTERNAL event system.
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
	 * Event::Register(Server::Ready_t, MyServerReadyCallback);
	 *
	 * @endcode
	 *
	 * Now, when the server is ready, it will call
	 * the MyServerReadyCallback() method in your server implementation.
	 */
	class Event
	{
	public:

		/**
		 * @addtogroup Event
		 * The enumerated Phoenix Events.
		 */
		enum Event_e {
			/**
			 * No event.
			 */
			NoEvent_Event,
			
			/**
			 * This event will be sent just after authentication is
			 * successful.
			 *
			 * All callbacks registered for this Event will be
			 * called when the Server has authenticated the client.
			 * 
			 * This is the name of an Event that callers
			 * can register for with the Server to be informed
			 * that a client was blocked.
			 *
			 * The callback is supplied with the socket file descriptor
			 * of the incoming connection and Data is set to
			 * point to the associated (PeerInfo*).
			 */
			Authenticated_Event,
			
			/**
			 * This event will be sent just before it replies
			 * to the BYE.
			 *
			 * All callbacks registered for this Event will be
			 * called when the Server has receives a BYE command
			 * 
			 * This is the name of an Event that callers
			 * can register for with the Server to be informed
			 * that a client was is quitting.
			 *
			 * The callback is supplied with the socket file descriptor
			 * of the incoming connection and Data is set to
			 * point to the associated (PeerInfo*).
			 */
			Bye_Event,

			/**
			 * All callbacks registered for this Event will be
			 * called when the Server has determined that this
			 * client should be blocked. It will be called from
			 * the server library at the time all NewClientConnected
			 * Events have been called.
			 * 
			 * This is the name of an Event that callers
			 * can register for with the Server to be informed
			 * that a client was blocked.
			 *
			 * The callback is supplied with the socket file descriptor
			 * of the incoming connection and Data is set to
			 * point to the associated (PeerInfo*).
			 */
			ClientBlocked_Event,
			
			/**
			 * All callbacks registered for this Event will be
			 * called when a client disconnects, or when the server
			 * disconnects a client. The callback is called after
			 * the socket connection has been terminated.
			 * 
			 * This is the name of an Event that callers
			 * can register for with the Server.
			 *
			 * The callback is supplied with the socket file descriptor
			 * of the incoming connection.
			 */
			ClientDisconnected_Event,

			/**
			 * All callbacks registered for this Event will
			 * be called when there is some kind of error on a
			 * socket file descriptor. This means the whatever
			 * is associated with Fd, will no longer function.
			 *
			 * Watch for this event to detect that connections have been
			 * unexpectedly shut down.
			 *
			 * This is the name of an Event that callers
			 * can register for with the Server.
			 *
			 * No optional data is passed with this Event.
			 *
			 * @note
			 * ClientDisconnected_Event is also sent with this event when the
			 * error is on a client connection.
			 */
			ErrorOnFd_Event,
			
			/**
			 * This event will be sent when an error is logged.
			 */
			LogError_Event,

			/**
			 * This event will be sent when an warning is logged.
			 */
			LogWarning_Event,

			/**
			 * All callbacks registered for this Event will be
			 * called when the server code logs a message.
			 *
			 * This is the name of an Event that callers
			 * can register for with the Server.
			 *
			 * The callback is supplied with:
			 *
			 * -The socket file descriptor of the incoming connection.
			 * When the file descriptor is (-1) then no file descriptor
			 * is associated with the message.
			 *
			 * -A text message that is being logged.
			 */
			LoggingMessage_Event,
	
			/**
			 * All callbacks registered for this Event
			 * will be called when a new client connection has happened.
			 * The callback will be called before the server starts
			 * to accept I/O on this connection.
			 *
			 * This is the name of an Event that callers
			 * can register for with the Server.
			 *
			 * The callback is supplied with the socket file descriptor
			 * of the incoming connection.
			 *
			 * The callback is supplied a PeerInfo object set to
			 * the peer's IP address as the 'Data'.
			 *
			 * The user supplied callback must return false when
			 * it wishes the connection to fail.
			 *
			 * @note
			 * If an implementation desires to do IP filtering,
			 * they would register for this Event
			 * and have the callback return true when okay,
			 * or false if the IP should be blocked.
			 *
			 * @note
			 * If any of the callbacks registered for this event
			 * return false, then the servers invoks a "ServerClientBlocked"
			 * Event.
			 */
			NewClientConnection_Event,

			/**
			 * All callbacks registered for this Event will
			 * be called prior to the shutdown. After all callbacks
			 * have been called, the shutdown will continue.
			 *
			 * This is the name of an Event that callers
			 * can register for with the Server.
			 *
			 * No optional data is passed with this Event.
			 */
			ShuttingDown_Event,

			/**
			 * All callbacks registered for this Event
			 * will be called with the server is ready to start
			 * processing clients.
			 *
			 * This is the name of an Event that callers
			 * can register for with the Server.
			 *
			 * No optional data is passed with this Event.
			 */
			ServerReady_Event,
		};

		/**
		 * @addtogroup Event
		 *
		 * @typedef EventCallback
		 *
		 * @addtogroup Event
		 *
		 * @ref EventCallback is the callback signature
		 * for when an event happens.
		 *
		 * Example, you write a callback to filter blocked IP address.
		 * It would  return false to tell the caller not to proceed.
		 *
		 * When the server starts, it creates a Event.
		 * In this example, it creates the Server::Ready_s
		 * Event. This is done by calling Register().
		 *
		 * The Server object predefines the Ready_s string to
		 * "ServerReady".
		 * @see Server
		 *
		 * In this example that is from the Server code, it is creating a new
		 * Event that it allows other code to register to receive
		 * a Server::Ready_s
		 * @code
		 *
		 * Event::Register(Server::Ready_s, nullptr);
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
		 * the Server::Ready_s Event for others to use. If the
		 * second parameter had been a method name, it would create
		 * the Event (if it had not already been created),
		 * and it would also register the method provided as one of the
		 * callback methods to be invoked when the
		 * Event_e Event happens.
		 *
		 * In your own code in the server implementation,
		 * if you want a notification of when a new client
		 * connects, you would register for Server::NewClientConnection_s .
		 * In this example, you might want to know when a new client
		 * connection happens so you can validate the IP address as not blocked.
		 *
		 * @code{.cpp}
		 *
		 * Event::Register(Server::NewClientConnections_s, MyValidateIPAddress);
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
		 * bool MyValidateIPAddress(int Fd, Event::Event_e ID, void * Data)
		 * {
		 *		bool Results = true;
		 *
		 *		// If you use this callback from multiple Events,
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
		 * When any @ref EventCallback method type is called,
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
		 * Read the specific Event description for details
		 * on what it expects when false is returned.
		 *
		 */
		typedef bool (*EventCallback)(int Fd, Event_e ID, void * Data);
		
		/**
		 * @addtogroup Event
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
		static void Register(Event_e EventName, EventCallback Cb);

		/**
		 * @addtogroup Event
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
		 * Event::Unregister(Server::Ready_s, MyServerReadyCallback);
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
		static void Unregister(Event_e ID, EventCallback Cb);

		/**
		 * @addtogroup Event
		 *
		 * To Invoke an event, call Invoke();
		 *
		 * @note
		 * This Invoke is a pure virtual method in Event.
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
		 * Event for details.
		 */
		virtual bool	Invoke(int Fd, Event_e ID, void * Data = nullptr) = 0;

		/**
		 * @addtogroup Event
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
		 * Event for details.
		 */
		static bool InvokeMessage(int Fd, Event_e Event, const char * Msg);
		
	protected:

		/**
		 * @addtogroup Event
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
		static bool DispatchCallbacks(int Fd, Event_e ID, void * Data = nullptr);
		
	private:

		/**
		 * List of registered Events.
		 */
		struct _Register
		{
			/**
			 * Link to Event description.
			 */
			Event_e			_Event;

			/**
			 * The registered callback.
			 */
			EventCallback		 Callback;

			/**
			 * _Register - Destructor.
			 */
			~_Register();
		};

		/**
		 * List of all callbacks registered for all Events.
		 *
		 * @note
		 * It is a multimap because more than one callback
		 * can be registered for any specific Event.
		 */
		static std::multimap<Event_e,_Register*>	_Registered;
	};

}
#endif // _RIVEREXPLOER_PHOENIX_PHOENIXEVENT_HPP_
