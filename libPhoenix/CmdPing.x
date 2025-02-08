%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::Ping
% * The Ping command sends a packet to the remote endpoint.
% * The other endpoint does a PingReply with no data.
% *
% * The reply is required.
% */
#endif
class Ping
{
	int foo;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::PingReply
% * The Ping Reply command sends a packet to the remote endpoint.
% * The other endpoint does a PingReply with no data.
% *
% * The reply is required.
% */
#endif
class PingReply
{
	int foo;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CmdPacket
% * @return a new Ping CmdPacket.
% */
#endif
%namespace RiverExplorer::Phoenix
%{
%class CmdPacket;
%extern CmdPacket * NewPing(CommandSequence Seq);
%}

