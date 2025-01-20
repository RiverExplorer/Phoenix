%/**
% * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
% * RiverExplorer is a trademark of RiverExplorer Games LLC                      
% */
%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
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
% * The Expunge Reply Command ...
% */
#endif
struct ExpungeReply
{
	int foo;
	/**@todo implement*/
};
