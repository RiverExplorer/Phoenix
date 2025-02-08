%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::Search
% * The Search_Cmd ...
% */
#endif
struct Search
{
	int foo;
	/**@todo implement*/
};

struct SearchReply
{
	int foo;
	/**@todo implement*/
};
