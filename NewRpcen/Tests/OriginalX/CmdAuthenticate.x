namespace RiverExplorer:Phoenix;

#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file CmdAuthenticate.x
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
%#include "AuthMD5.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#include <RiverExplorer/Phoenix/AuthMD5.hpp>
%#endif

struct Authenticate
{
	AuthMD5	Md5;
};

struct AuthenticateReply
{
	bool_t	Accepted;
};

