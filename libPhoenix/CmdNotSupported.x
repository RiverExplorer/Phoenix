%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * Not supported. This is sent back to the initiating endpoint
% * when this endpoint does not support the command sent.
% *
% * @note
% * There is no data associated with a NotSupported_Cmd, only the
% * CmdPacket is sent.
% */
#endif

struct NotSupported
{
	int foo;
};

struct NotSupportedReply
{
	int foo;
};

