namespace RiverExplorer:Phoenix;

#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file CmdAdministration.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @enum RiverExplorer::Phoenix::AdministrativeCommands_e
% * The Administratoin Commands
% */
#endif
enum AdministrativeCommands_e
{
	USER_CREATE = 0,
	USER_DELETE = 1,
	USER_RENAME = 2,
	USER_LIST = 3,
	USER_PERMISSIONS = 4,
	SERVER_SHUTDOWN = 5,
	SERVER_LOGS = 6,
	SERVER_KICK_USER = 7,
	SERVER_MANAGE_BANS = 8,
	SERVER_VIEW_STATS = 9
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phonenix::Administration
% * The Administratoin Command ...
% */
#endif
class Administration
{
	int Todo;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phonenix::AdministrationReply
% * The Administratoin Reply Command ...
% */
#endif
class AdministrationReply
{
	int Todo;
};
