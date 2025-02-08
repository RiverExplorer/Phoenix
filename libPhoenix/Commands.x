
%#ifdef BUILDING_LIBPHOENIX
%#include "CppTypes.hpp"
%#else
%#include <RiverExplorer/Phoenix/CppTypes.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @enum RiverExplorer::Phoenix::Command_e
% * Command_e: An enumerated list of fetch commands.
% * <ul>
% * <li>
% *	Admin_Cmd - The packet contains an administrative command.
% * </li>
% * <li>
% *	AdminReply_Cmd - The packet contains an administrative command reply.
% * </li>
% * <li>
% *	Auth_Cmd - The packet contains an authentication command.
% * </li>
% * <li>
% *	AuthReply_Cmd - The packet contains an authentication command reply.
% * </li>
% * <li>
% *	Capability_Cmd - The packet contains a capability command.
% * A Capability_Cmd has no reply.
% * </li>
% * <li>
% *	Folder_Cmd - The packet contains a folder command.
% * </li>
% * <li>
% *	FolderReply_Cmd - The packet contains a folder command reply.
% * </li>
% * <li>
% * The NotSupported_Cmd is sent back to
% * the remote endpoint when it sends a command
% * that is not supported.
% * It can be because of access control list,
% * out of sequence, or other error.
% * A Capability_Cmd has no reply.
% * </li>
% * <li>
% *	Ping_Cmd - The packet contains a pint command reply.
% * The reply to a ping is the same packet back.
% * </li>
% * <li>
% *	Proto_Cmd - The packet contains a protocol command.
% * A protocol command is an extension command not built int
% * the core Phoenix protocol.
% * </li>
% * <li>
% *	ProtoReply_Cmd - The packet contains a protocol command reply.
% * </li>
% * <li>
% *	Reserved_Cmd - In the unlikely event that 2^32 commands
% * are ever created, this is an escape to allow more.
% * </li>
% * </ul>
% *
% */
#endif
enum Command_e
{
	Admin_Cmd = 1,
	AdminReply_Cmd = 2,
	Auth_Cmd = 3,
	AuthReply_Cmd = 4,
	Capability_Cmd = 5,
	Folder_Cmd = 6,
	FolderReply_Cmd = 7,
	NotSupported_Cmd = 8,
	Ping_Cmd = 9,
	Proto_Cmd = 10,
	ProtoReply_Cmd = 11,
	Reserved_Cmd = 0xffffffff
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

%#ifdef BUILDING_LIBPHOENIX
%#include "CmdAcl.hpp"
%#include "CmdAddMessage.hpp"
%#include "CmdAuthenticate.hpp"
%#include "CmdAdministration.hpp"
%#include "CmdCapability.hpp"
%#include "CmdCopyMessage.hpp"
%#include "CmdFolder.hpp"
%#include "CmdExpunge.hpp"
%#include "CmdGetMessage.hpp"
%#include "CmdKeepAlive.hpp"
%#include "CmdNotSupported.hpp"
%#include "CmdPing.hpp"
%#include "CmdSearch.hpp"
%#include "CmdSubscribe.hpp"
%#include "CmdTimeout.hpp"
%#include "CmdUpdateMessage.hpp"
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/CmdAcl.hpp>
%#include <RiverExplorer/Phoenix/CmdAddMessage.hpp>
%#include <RiverExplorer/Phoenix/CmdAuthenticate.hpp>
%#include <RiverExplorer/Phoenix/CmdAdministration.hpp>
%#include <RiverExplorer/Phoenix/CmdCapability.hpp>
%#include <RiverExplorer/Phoenix/CmdCopyMessage.hpp>
%#include <RiverExplorer/Phoenix/CmdFolder.hpp>
%#include <RiverExplorer/Phoenix/CmdExpunge.hpp>
%#include <RiverExplorer/Phoenix/CmdGetMessage.hpp>
%#include <RiverExplorer/Phoenix/CmdKeepAlive.hpp>
%#include <RiverExplorer/Phoenix/CmdNotSupported.hpp>
%#include <RiverExplorer/Phoenix/CmdPing.hpp>
%#include <RiverExplorer/Phoenix/CmdSearch.hpp>
%#include <RiverExplorer/Phoenix/CmdSubscribe.hpp>
%#include <RiverExplorer/Phoenix/CmdTimeout.hpp>
%#include <RiverExplorer/Phoenix/CmdUpdateMessage.hpp>
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#endif
#endif

union CmdData switch (Command_e Cmd)
{
 case Admin_Cmd:
	 Administration *	AdminData;
	 
 case AdminReply_Cmd:
	 AdministrationReply *	AdminReplyData;
	 
 case Auth_Cmd:
	 Authenticate		*	AuthData;

 case AuthReply_Cmd:
	 AuthenticateReply		*	AuthReplyData;

 case Capability_Cmd:
	 Capability		*	CapabilityData;

 case Folder_Cmd:
	 Folder	*	FolderData;
	 
 case FolderReply_Cmd:
	 FolderReply	*	FolderReplyData;
	 
 case NotSupported_Cmd:
	 void;

 case Ping_Cmd:
	 void;
	 
};

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
%namespace RiverExplorer::Phoenix
%{
%extern bool Register_Cmd(Command_e Cmd, CommandCallback * Callback);
%}
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
%namespace RiverExplorer::Phoenix
%{
%extern bool Register_Cmd(Command_e Cmd, CommandCallback * Callback);
%}
#endif

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CmdPacket
% * A command consists of:
% * - The enumerated command (Command_e),
% * - An XDR opaque object (length + data).
% * The XDR opaque data was prepared by the Cmd specific code
% * and is set into place for transport.
% */
#endif
struct  CmdPacket
{
	CommandSequence Sequence;
  CmdData Data;
};

