#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "AOID.hpp"
%#else
%#include <RiverExplorer/Phoenix/AOID.hpp>
%#endif
#endif

struct ServerKickPayload
{
	AOID_e Kick; /* With Kick set to SERVER_KICK_USER */
};
