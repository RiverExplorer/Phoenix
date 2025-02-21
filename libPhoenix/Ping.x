%
%#ifdef BUILDING_LIBPHOENIX
%#include "PacketBody.hpp"
%#else
%#include <RiverExplorer/Phoenix/PacketBody.hpp>
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
class PingPayload
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
class PingReplyPayload
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
%class Command;
%extern Command * NewPing(SEQ_t Seq);
%}

