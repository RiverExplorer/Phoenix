#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file Bye.x
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
%#include "SEQ_t.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/SEQ_t.hpp>
%#endif
#endif

struct ByePayload {
	int foo;
};

struct ByeReplyPayload {
	int foo;
};

#ifdef RPC_HDR
%namespace RiverExplorer::Phoenix {
%extern Command * NewBye(SEQ_t Sequence);
%}
#endif
