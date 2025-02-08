%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::GetMessage
% * The GetMessage_Cmd ...
% */
#endif
struct GetMessage
{
	int foo;
	/**@todo implement*/
};
