%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::UpdateMessage
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
% * @class RiverExplorer::Phoenix::UpdateMessageReply
% * The UpdateMessage Reply Command ...
% */
#endif
struct UpdateMessageReply
{
	int foo;
	/**@todo implement*/
};
