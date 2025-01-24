%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * The Subscribe Command ...
% */
#endif
struct Subscribe
{
	int Foo;
	/**@todo implement*/
};

#ifdef RPC_HDR
%/**
% * The Subscribe Reply Command ...
% */
#endif
struct SubscribeReply
{
	int Foo;
	/**@todo implement*/
};
