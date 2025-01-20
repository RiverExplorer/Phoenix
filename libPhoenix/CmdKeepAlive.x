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
%extern const char * const KeepAliveWasReceived;
#endif
#ifdef RPC_XDR
%const char * const KeepAliveWasReceived  = "KeepAliveWasReceived";
#endif
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
