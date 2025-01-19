/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLORE_PHOENIX_IPPEER_HPP_
#define _RIVEREXPLORE_PHOENIX_IPPEER_HPP_

#include "IPPeer.hpp"

#include <memory.h>

namespace RiverExplorer::Phoenix
{

	IPPeer::IPPeer()
	{
		memset(&Addr, 0, sizeof(Addr));
		AddrLen = 0;

		return;
	}

	IPPeer::~IPPeer()
	{
		// We clear for secuity.
		//
		memset(&Addr, 0, sizeof(Addr));
		AddrLen = 0;

		return;
	}


	IPPeer &
	IPPeer::operator=(const IPPeer & Other)
	{
		memcpy(&Addr, &Other.Addr, sizeof(Addr));
		AddrLen = Other.AddrLen;

		return(*this);
	}
}

#endif // _RIVEREXPLORE_PHOENIX_IPPEER_HPP_
