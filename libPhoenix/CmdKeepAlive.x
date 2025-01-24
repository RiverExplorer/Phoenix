%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * The KeepAlive command sends a packet to the remote endpoint.
% *
% * There is no reply to a KeepAlive command.
% */
#endif
struct KeepAlive
{
	int foo;
};
