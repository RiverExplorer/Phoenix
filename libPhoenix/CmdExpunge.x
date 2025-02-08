%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::Expunge
% * The Expunge Command ...
% */
#endif
struct Expunge
{
	int foo;
	/**@todo implement*/
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::ExpungeReply
% * The Expunge Reply Command ...
% */
#endif
struct ExpungeReply
{
	int foo;
	/**@todo implement*/
};
