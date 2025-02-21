#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#endif
#endif

struct ServerKickPayload
{
	CMD_e Kick; /* With Kick set to SERVER_KICK_USER */
};
