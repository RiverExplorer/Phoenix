/**
 * @file HostName.cpp
 * @copyright 2025 Douglas Mark Royer (A.K.A. RiverExplorer)
 * licensed under CC BY 4.0.
 * @author Doug Royer
 *
 * Check if string is valid IPv4, IPv6, or hostname.
 *
 * RiverExplorer is a trademark of RiverExplorer LLC.
 */

#include "HostName.hpp"
#include <memory.h>
#include <unistd.h>

namespace RiverExplorer::Phoenix
{
	bool
	HostName::IsIPv4Address(const std::string & Str)
	{
		const std::regex IPv4Pattern(
		 "^([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\."
		 "([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\."
		 "([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\."
		 "([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$"
														);
    
    return std::regex_match(Str, IPv4Pattern);
	}
		
	bool
	HostName::IsIPv6Address(const std::string & Str)
	{
		bool Results = false;
		
		// First, try inet_pton for a quick validation
		//
    struct in6_addr addr;
		
    if (inet_pton(AF_INET6, Str.c_str(), &addr) == 1) {
			Results = true;

    } else {

			// More comprehensive regex check for IPv6
			// This handles full and compressed formats
			const std::regex IPv6Pattern(
			 "^(?:[0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}$|"     // Full format
			 "^::(?:[0-9a-fA-F]{1,4}:){0,6}[0-9a-fA-F]{1,4}$|" // Leading ::
			 "^[0-9a-fA-F]{1,4}::(?:[0-9a-fA-F]{1,4}:){0,5}[0-9a-fA-F]{1,4}$|" // Middle ::
			 "^(?:[0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}$|"  // Trailing :
			 "^(?:[0-9a-fA-F]{1,4}:){1,7}:$"                   // Only colons
															);

			Results = std::regex_match(Str, IPv6Pattern);
		}
    
    return(Results);
	}

	int
	HostName::ConnectTcp(const std::string & HostName,
											 uint16_t Port,
											 struct ::addrinfo *& Address)
	{
		int Results = -1;
		
		// Basic hostname validation
    // - Contains at least one character
    // - Contains only alphanumeric, dots, and hyphens
    // - Doesn't start or end with a dot or hyphen
    // - Not an IP address
    
    if (!HostName.empty()
				&& !IsIPv4Address(HostName)
				&& !IsIPv6Address(HostName)) {
    
			// Additional check: try to resolve the hostname
			//
			int S;
			struct addrinfo Hints;

			memset(&Hints, 0, sizeof(Hints));

			// AF_INET, AF_INET6, or AF_UNSPEC
			//
			Hints.ai_family = AF_UNSPEC;

			// SOCK_STREAM, SOCK_DGRAM, or 0.
			//
			Hints.ai_socktype = SOCK_STREAM;

			// AI_PASSIVE is for servers.
			// 0 is for clients.
			//
			Hints.ai_flags = 0;

			// any protocol.
			//
			Hints.ai_protocol = 0;

			char  PortBuf[10];

			snprintf(PortBuf, sizeof(PortBuf), "%u", Port);
			S = getaddrinfo(HostName.c_str(), PortBuf, &Hints, &Address);

			if (S == 0) {
				struct addrinfo * Ptr = Address;

				for (; Ptr != nullptr; Ptr = Ptr->ai_next) {
					Results = socket(Ptr->ai_family, Ptr->ai_socktype, Ptr->ai_protocol);

					if (Results == -1) {
						continue;
					}

					int Enable = 1;
					
					setsockopt(Results, SOL_SOCKET, SO_REUSEADDR, &Enable, sizeof(Enable));
					
					if (connect(Results, Ptr->ai_addr, Ptr->ai_addrlen) != -1) {
						break; // Success!
					}
					close(Results);
					Results = -1;
				}
			}
		}
    
    return(Results);
	}

	bool
	HostName::IsHostName(const std::string & Str)
	{
		bool Results = false;

		// Basic hostname validation
    // - Contains at least one character
    // - Contains only alphanumeric, dots, and hyphens
    // - Doesn't start or end with a dot or hyphen
    // - Not an IP address
    
    if (!Str.empty() && !IsIPv4Address(Str) && !IsIPv6Address(Str)) {
			struct hostent * he = gethostbyname(Str.c_str());

			if (he != nullptr	|| h_errno == TRY_AGAIN) {
				// TRY_AGAIN means temporary DNS failure

				const std::regex
					HostnamePattern("^[a-zA-Z0-9]([a-zA-Z0-9\\-\\.]*[a-zA-Z0-9])?$");
    
				Results = std::regex_match(Str, HostnamePattern);
			}
		}
    
    return(Results);
	}
}
