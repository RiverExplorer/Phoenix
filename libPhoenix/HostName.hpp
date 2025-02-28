/**
 * @file HostName.hpp
 * @copyright 2025 Douglas Mark Royer (A.K.A. RiverExplorer)
 * licensed under CC BY 4.0.
 * @author Doug Royer
 *
 * Check if string is valid IPv4, IPv6, or hostname.
 *
 * RiverExplorer is a trademark of RiverExplorer LLC.
 */
#ifndef _RIVEREXPLORER_PHOENIX_HOSTNAME_HPP_
#define _RIVEREXPLORER_PHOENIX_HOSTNAME_HPP_

#ifdef BUILDING_LIBPHOENIX

#else

#endif

#include <string>
#include <regex>
#include <netdb.h>      // For gethostbyname
#include <arpa/inet.h>  // For inet_addr and inet_pton
#include <errno.h>

namespace RiverExplorer::Phoenix
{
	/**
	 * @class HostName HostName.hpp <RiverExplorer/Phoenix/HostName.hpp>
	 */
	class HostName
	{
	public:

		/**
		 * Test if Str is an IPv4 address.
		 *
		 * @param Str The string to test.
		 *
		 * @return true if it is a valid IPv4 address.
		 */
		static bool IsIPv4Address(const std::string & Str);
		
		/**
		 * Test if Str is an IPv6 address.
		 *
		 * @param Str The string to test.
		 *
		 * @return true if it is a valid IPv6 address.
		 */
		static bool IsIPv6Address(const std::string & Str);

		/**
		 * Test if Strin is a host name.
		 *
		 * @param String the string to test.
		 *
		 * @return true if Str could be a host name.
		 */
		static bool IsHostName(const std::string & Str);

		/**
		 * Connect to host.
		 *
		 * @param HostName The host name to connect to.
		 *
		 * @param Port The port number to connect to.
		 *
		 * @param Address A reference to a struct addrinfo.
		 * The pointer will be set to nullptr, then tested.
		 * If you are re-using Address, call freeaddrinfo(Address)
		 * before calling Connect(), or you will have a memory leak.
		 *
		 * @return The socket or (-1) on failure.
		 */
		static int ConnectTcp(const std::string & HostName,
													uint16_t Port,
													struct ::addrinfo *& Address);
	};
}

#endif // _RIVEREXPLORER_PHOENIX_IO_HPP_
