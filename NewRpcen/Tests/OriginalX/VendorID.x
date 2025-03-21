namespace RiverExplorer:Phoenix;

#ifdef RPC_HDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 09:18:33 doug>
% *
% * @file VendorID.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif

#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#endif
%
%/**
% * @typedef VendorID
% * A string that when supplied, identifies the vendor specific
% * command and capabilities.
% */
#endif
class VendorID
{
	CMD_e Cmd;
	string ID<>;
};
