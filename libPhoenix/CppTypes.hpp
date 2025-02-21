
#ifndef _RIVEREXPLORER_MIME_CPPMIME_HPP_
#define _RIVEREXPLORER_MIME_CPPMIME_HPP_

#ifdef BUILDING_LIBPHOENIX
//#include "Types.hpp"
#else
//#include <RiverExplorer/Phoenix/Types.hpp>
#endif

namespace RiverExplorer::Phoenix
{
	class Command;
	
	/**
	 * 64 bit network to host byte order conversion.
	 *
	 * @param NetValue A uint64_t value in network byte order.
	 *
	 * @return A uint64_t value in host order
	 */
	extern uint64_t ntohll(uint64_t NetValue);

	/**
	 * 64 bit host to network byte order conversion.
	 *
	 * @param HostValue A uint64_t value in host byte order.
	 *
	 * @return A uint64_t value in network byte order.
	 */
	extern uint64_t htonll(uint64_t NetValue);
	
	/**
	 * Commands come in from the remote endpoint.
	 * Each over the wire command includes its size.
	 * The first value is the command, followed by
	 * the size of data that follow. Followed by the related data.
	 *
	 * This is the callback signature to register to process a command.
	 *
	 * @param Fd The associated file descriptor.
	 *
	 * @param ReadXdrs An blob of XDR data to be decoded, parsed,
	 * and processed.
	 *
	 * @return The callback returns TRUE on all okay, or FALSE
	 * on an error.
	 *
	 * @note
	 * It is is recommended that each registred callback be thread safe.
	 *
	 * @note
	 * More than one function can me registerd for any command.
	 * When there is more than one function registered for any specific
	 * command, they are processed in random order.
	 */
	typedef bool (*CommandCallback)(int Fd, Command * Pkt, XDR * ReadXdrs);

}
#endif // _RIVEREXPLORER_MIME_CPPMIME_HPP_
