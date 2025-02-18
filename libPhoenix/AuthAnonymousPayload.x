#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "AOID.hpp"
%#else
%#include <RiverExplorer/Phoenix/AOID.hpp>
%#endif
#endif

struct AuthAnonymousPayload
{
	AOID_e Anonymous; /* Set to AUTHANONYMOUS (%x26) */
};
