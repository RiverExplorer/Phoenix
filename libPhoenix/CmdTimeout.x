%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::Timeout
% * The Timeout Command ...
% */
#endif
struct Timeout
{
	int foo;
	/**@todo implement*/
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::TimeoutReply
% * The Timeout Reply Command ...
% */
#endif
struct TimeoutReply
{
	int foo;
	/**@todo implement*/
};
