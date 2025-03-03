#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file PacketHeader.x
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
%#include "CommonDefinitions.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#endif
#endif
/**
 * The length of a packet.
 */
typedef Length PacketHeader;
