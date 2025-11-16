namespace RiverExplorer:Phoenix;

#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-24 22:08:23 doug>
% *
% * @file FileCreate.x
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
 /*%#include "File.hpp"*/
%#else
 /*%#include <RiverExplorer/Phoenix/File.hpp>*/
%#endif
#endif

typedef uint64_t FileID;

struct FileCreatePayload {
	string Name<>;
};

struct FileCreateReplyPayload {
	bool Success;
	FileID	ID;
};
