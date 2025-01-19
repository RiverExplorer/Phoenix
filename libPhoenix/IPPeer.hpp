/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLORE_PHOENIX_PEER_HPP_
#define _RIVEREXPLORE_PHOENIX_PEER_HPP_

#ifndef W64
#include <arpa/inet.h>
#include <net/if.h>
#else
/**@todo W64 IPPeer.hpp*/
#endif

namespace RiverExplorer::Phoenix
{
	
	/**
	 * @class Peer "Certs.hpp <RiverExplorer/Phoenix/Peer.hpp>
	 * An object holds a peer IP connection address.
	 */
	class IPPeer
	{
	public:

		/**
		 * IPPeer - Default Constructor.
		 */
		IPPeer();

		/**
		 * IPPeer - Destructor.
		 */
		~IPPeer();

		/**
		 * IPPeer - Copy operator.
		 *
		 * @param Other The other IPPeer to copy from.
		 */
		IPPeer & operator=(const IPPeer & Other);
		
		union {
			in_addr		IpV4;
			in6_addr	IpV6;
		} Addr;

		socklen_t	AddrLen;

		
	};
}

#endif // _RIVEREXPLORE_PHOENIX_CERTS_HPP_
