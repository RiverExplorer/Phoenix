#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file CmdUpdateMessage.x
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
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::UpdateMessage
% * The UpdateMessage command ...
% */
#endif
struct UpdateMessage
{
	int foo;
	/**@todo implement*/
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::UpdateMessageReply
% * The UpdateMessage Reply Command ...
% */
#endif
struct UpdateMessageReply
{
	int foo;
	/**@todo implement*/
};
