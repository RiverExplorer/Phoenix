%/**
%* Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
%* RiverExplorer is a trademark of RiverExplorer Games LLC                      
%*/
%
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * The Ping_Cmd sends a tiny packet to the remote endpoint.
% * When WithReply is true, then the remote endpoint replies
% * with the same command ID and a Ping_Cmd with WithReply set to false.
% */
#endif
struct Ping
{
	CmdPacket Cmd;
	bool_t	WithReply;
};
