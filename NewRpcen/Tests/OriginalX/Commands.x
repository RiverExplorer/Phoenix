namespace RiverExplorer:Phoenix;

#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 08:28:49 doug>
% *
% * @file Commands.x
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% *
% * Licensed under the MIT License. See LICENSE file
% * or https://opensource.org/licenses/MIT for details.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif

#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CppTypes.hpp"
%#else
%#include <RiverExplorer/Phoenix/CppTypes.hpp>
%#endif
#endif

 /*
	* The commands.
	* An XDR enum is 32-bits in size.
	* With the VENDOR_BIT set to zero, making it not a vendor command.
	*/
enum CMD_e {
		 AUTHANONYMOUS = 0x26,
		 AUTHCERT_TLS = 0x27,
		 AUTHCERT_USER = 0x28,
		 AUTHMD5 = 0x10,
		 BYE = 0x2d,
		 CAPABILITY_PRE = 0x29,
		 CAPABILITY_POST =  0x2a,
		 FILE_CREATE = 0x1d,
		 FILE_COPY = 0x1e,
		 FILE_DELETE = 0x1f,
		 FILE_RENAME = 0x20,
		 FILE_METADATA = 0x21,
		 FILE_MOVE = 0x22,
		 FILE_SHARE = 0x23,
		 FILE_GET = 0x24,
		 FILE_MODIFY = 0x25,
		 FOLDER_CAPABILITY = 0x13,
		 FOLDER_CREATE = 0x14,
		 FOLDER_COPY = 0x15,
		 FOLDER_DELETE = 0x16,
		 FOLDER_RENAME = 0x17,
		 FOLDER_METADATA = 0x18,
		 FOLDER_MOVE = 0x19,
		 FOLDER_OPEN = 0x1a,
		 FOLDER_SHARE = 0x1b,
		 FOLDER_LIST = 0x1c,
		 NOT_SUPPORTED = 0x2b,
		 PING = 0x2c,
		 RESERVED_CMD = 0xffffffff,
		 SERVER_CONFIGURE = 0x05,
		 SERVER_KICK_USER = 0x06,
		 SERVER_LOGS = 0x07,
		 SERVER_MANAGE_BANS = 0x08,
		 SERVER_SHUTDOWN = 0x09,
		 SERVER_VIEW_STATS = 0x0a,
		 USER_CREATE = 0x0b,
		 USER_DELETE = 0x0c,
		 USER_LIST = 0x0d,
		 USER_PERMISSIONS = 0x0e,
		 USER_RENAME = 0x0f,
		 VENDOR_ID = 0x12
};

#ifdef RPC_HDR
%/**
% * The transport top level is simple.
% * You can Send() a packet and get a packet back.
% * Or you send a notification that receives nothing back.
% * Or you send a broadcast message to all interested participants, with no
% * reply expected.
% */
#endif

#ifdef RPC_HDR
%/**
% * @fn RiverExplorer::Phoenix::Register_Cmd
% * Set the callback for a command.
% *
% * @param Cmd The command being registered.
% *
% * @param Callback The user supplied callback function.
% *
% * @note
% * It is is recommended that each registered callback be thread safe.
% */
%extern bool Register_Cmd(CMD_e Cmd, CommandCallback * Callback);
#endif

#ifdef RPC_HDR
%/**
% * @fn RiverExplorer::Phoenix::Register_Cmd
% * Set the callback for a command.
% *
% * @param Cmd The command being registered.
% *
% * @param Callback The user supplied callback function.
% *
% * @note
% * It is is recommended that each registered callback be thread safe.
% */
%extern bool Register_Cmd(CMD_e Cmd, CommandCallback * Callback);
#endif
