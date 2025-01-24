%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * The UpdateMessage command ...
% */
#endif
struct UpdateMessage
{
	int foo;
	/**@todo implement*/
};

#ifdef RPC_HDR
%/**
% * The UpdateMessage Reply Command ...
% */
#endif
struct UpdateMessageReply
{
	int foo;
	/**@todo implement*/
};
