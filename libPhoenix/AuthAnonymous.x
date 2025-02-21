#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#endif
#endif

struct AuthAnonymousPayload
{
	CMD_e Anonymous; /* Set to AUTHANONYMOUS (%x26) */
};
