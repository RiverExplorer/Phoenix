#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file Ping.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif
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

