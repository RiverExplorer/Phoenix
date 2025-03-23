#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file ServerConfigure.x
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
%#include "CommonDefinitions.hpp"
%#include "ConfigSet.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/ConfigSet.hpp>
%#endif
#endif

/**
 * An array of OpConfigSet values.
 */
struct ServerConfigurePayload {
	CMD_e				Aoid;      /* Set to SERVER_CONFIGURE. */
	ConfigSet		Values<>;  /* XDR arrays start with a length. */
};
