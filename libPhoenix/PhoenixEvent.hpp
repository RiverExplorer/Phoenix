
#include <string>
#include <stdint.h>
#include <map>

namespace RiverExplorer::Phoenix
{
	/**
	 * @class Event 
	 * This object is a simple INTERNAL event system.
	 * Code can send and receive these events.
	 * @see Notification for an external event system.
	 *
	 * The caller registers for an event by providing
	 * the name of the event, as a string.
	 *
	 * They get back the ID for the event.
	 *
	 * An event is a string. This string is from
	 * the character set [a-z][A-A] and hyphen.
	 *
	 * -The event names can be mixed case.
	 * -The event names case is ignored when comparing them.
	 * So that "Abc" and "abc" are the same event.
	 */
	class Event
	{
	public:
		/**
		 * An event ID.
		 */
		typedef uint64_t	EventID;

		/**
		 * The callback signature for when an event happens.
		 *
		 * @param Fd The associated file descriptor.
		 *
		 * @param ID The associated event ID returned from Register.
		 *
		 * @param Data Optional data, defined by the event.
		 * May be nullptr when the event does not pass any information
		 * to the event handler.
		 */
		typedef void (*EventCallback)(int fd, EventID ID, void * Data);
		
		/**
		 * Register for an event.
		 * More than one callback can be registered for the same event.
		 * Example, multiple plugs may want the "New-Client-Connected" event.
		 *
		 * @param EventName The event being registred for.
		 *
		 * @param Cb The callback to call when the event happens.
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
		static EventID Register(std::string EventName, EventCallback Cb);

		/**
		 * Unregister for an event.
		 *
		 * @param ID The ID For the event name that Cb will
		 * be unregistered for.
		 *
		 * @param Cb The callback to call when the event happens.
		 *
		 * @note
		 * This unregisters the callback (Cb), the event name
		 * is still known.
		 */
		static void Unregister(EventID ID, EventCallback Cb);

		/**
		 * Invoke an event.
		 *
		 * @param Fd The associated clinet file descriptor.
		 *
		 * @param ID The event ID.
		 *
		 * @param Data Optional event specific data.
		 */
		static void	Invoke(int Fd, EventID ID, void * Data = nullptr);
		
	private:

		struct _EventMap
		{
			std::string Name;
			uint64_t		ID;

			~_EventMap();
		};
		static std::map<std::string,_EventMap*> _EventByName;
		static std::map<EventID,_EventMap*>	_EventByID;

		struct _Register
		{
			_EventMap			*	EventMap;
			EventCallback		 Callback;

			~_Register();
		};
		static std::multimap<EventID,_Register*>	_Registered;

		/**
		 * Use this as the next EventID to return.
		 */
		static uint64_t _NextEventID;
	};

}
