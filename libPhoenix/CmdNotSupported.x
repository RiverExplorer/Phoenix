%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
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

#ifdef RPC_HDR
%/**
% * Make a new CmdPacket that can be sent back to
% * the original endpoint.
% *
% * @param ID The ID of the command not supported.
% *
% * @return The new packet, ready to send.
% */
#endif
%namespace RiverExplorer::Phoenix
%{
%extern CmdPacket * NewNotSupported(CommandID ID);
%}
