namespace RiverExplorer:Phoenix;

#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 10:21:18 doug>
% *
% * @file PacketBody.x
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
%#include "SEQ_t.hpp"
%#include "Commands.hpp"
%#include "Capability.hpp"
%#include "AuthAnonymous.hpp"
%#include "AuthCertTls.hpp"
%#include "AuthCertUser.hpp"
%#include "AuthMD5.hpp"
%#include "Bye.hpp"
%#include "FileCreate.hpp"
%#include "FileCopy.hpp"
%#include "FileDelete.hpp"
%#include "FileGet.hpp"
%#include "FileMetaData.hpp"
%#include "FileModify.hpp"
%#include "FileMove.hpp"
%#include "FileRename.hpp"
%#include "FileShare.hpp"
%#include "FolderCreate.hpp"
%#include "FolderCopy.hpp"
%#include "FolderDelete.hpp"
%#include "FolderList.hpp"
%#include "FolderMetaData.hpp"
%#include "FolderMove.hpp"
%#include "FolderOpen.hpp"
%#include "FolderRename.hpp"
%#include "FolderShare.hpp"
%#include "ServerConfigure.hpp"
%#include "ServerConfigureReply.hpp"
%#include "ServerLogs.hpp"
%#include "ServerManageBans.hpp"
%#include "ServerStats.hpp"
%#include "Shutdown.hpp"
%#include "UserCreate.hpp"
%#include "UserDelete.hpp"
%#include "UserList.hpp"
%#include "UserPermissions.hpp"
%#include "UserRename.hpp"

%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/SEQ_t.hpp>
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/Capability.hpp>
%#include <RiverExplorer/Phoenix/AuthAnonymous.hpp>
%#include <RiverExplorer/Phoenix/AuthCertTls.hpp>
%#include <RiverExplorer/Phoenix/AuthCertUser.hpp>
%#include <RiverExplorer/Phoenix/AuthMD5.hpp>
%#include <RiverExplorer/Phoenix/Bye.hpp>
%#include <RiverExplorer/Phoenix/FileCreate.hpp>
%#include <RiverExplorer/Phoenix/FileCopy.hpp>
%#include <RiverExplorer/Phoenix/FileDelete.hpp>
%#include <RiverExplorer/Phoenix/FileGet.hpp>
%#include <RiverExplorer/Phoenix/FileMetaData.hpp>
%#include <RiverExplorer/Phoenix/FileModify.hpp>
%#include <RiverExplorer/Phoenix/FileMove.hpp>
%#include <RiverExplorer/Phoenix/FileRename.hpp>
%#include <RiverExplorer/Phoenix/FileShare.hpp>
%#include <RiverExplorer/Phoenix/FolderCreate.hpp>
%#include <RiverExplorer/Phoenix/FolderCopy.hpp>
%#include <RiverExplorer/Phoenix/FolderDelete.hpp>
%#include <RiverExplorer/Phoenix/FolderList.hpp>
%#include <RiverExplorer/Phoenix/FolderMetaData.hpp>
%#include <RiverExplorer/Phoenix/FolderMove.hpp>
%#include <RiverExplorer/Phoenix/FolderOpen.hpp>
%#include <RiverExplorer/Phoenix/FolderRename.hpp>
%#include <RiverExplorer/Phoenix/FolderShare.hpp>
%#include <RiverExplorer/Phoenix/ServerConfigure.hpp>
%#include <RiverExplorer/Phoenix/ServerConfigureReply.hpp>
%#include <RiverExplorer/Phoenix/ServerLogs.hpp>
%#include <RiverExplorer/Phoenix/ServerManageBans.hpp>
%#include <RiverExplorer/Phoenix/ServerStats.hpp>
%#include <RiverExplorer/Phoenix/Shutdown.hpp>
%#include <RiverExplorer/Phoenix/UserCreate.hpp>
%#include <RiverExplorer/Phoenix/UserDelete.hpp>
%#include <RiverExplorer/Phoenix/UserList.hpp>
%#include <RiverExplorer/Phoenix/UserPermissions.hpp>
%#include <RiverExplorer/Phoenix/UserRename.hpp>
%#endif
#endif

/**
 * Mask to check if CMD value in packet
 * is vendor extension.
 */
const CMD_VENDOR_MASK = 0x80000000;

struct VendorDefined {
	uint32_t	VendorCommand; /* 0x80000000-fffffffe */
	opaque    Data<>; /* XDR arrays start with a length. */
};

/**
 * A CMD payload is one of these types.
 * With Cmd set to a CMD value.
 */ 
union OneCommand switch (CMD_e Cmd) {

 case AUTHANONYMOUS:
	 AuthAnonymousPayload AuthAnonymousCmd;
	 
 case AUTHCERT_TLS:
	 AuthCertTlsPayload AuthCertTlsCmd;
	 
 case AUTHCERT_USER:
	 AuthCertUserPayload AuthCertUserCmd;
	 
 case AUTHMD5:
	 AuthMD5Payload AuthMD5Cmd;
	 
 case BYE:
	 ByePayload ByeCmd;
	 
 case CAPABILITY_PRE:
	 CapabilityPayload CapabilityPreCmd;
	 
 case CAPABILITY_POST:
	 CapabilityPayload CapabilityPostCmd;
	 
 case FILE_CREATE:
	 FileCreatePayload FileCreateCmd;
	 
 case FILE_COPY:
	 FileCopyPayload FileCopyCmd;
	 
 case FILE_DELETE:
	 FileDeletePayload FileDeleteCmd;
	 
 case FILE_RENAME:
	 FileRenamePayload FileRenameCmd;
	 
 case FILE_METADATA:
	 FileMetaDataPayload FileMetaDataCmd;
	 
 case FILE_MOVE:
	 FileMovePayload FileMoveCmd;
	 
 case FILE_SHARE:
	 FileSharePayload FileShareCmd;
	 
 case FILE_GET:
	 FileGetPayload FileGetCmd;
	 
 case FILE_MODIFY:
	 FileModifyPayload FileModifyCmd;
	 
 case FOLDER_CREATE:
	 FolderCreatePayload FolderCreateCmd;
	 
 case FOLDER_COPY:
	 FolderCopyPayload FolderCopyCmd;
	 
 case FOLDER_DELETE:
	 FolderDeletePayload FolderDeleteCmd;
	 
 case FOLDER_RENAME:
	 FolderRenamePayload FolderRenameCmd;
	 
 case FOLDER_METADATA:
	 FolderMetaDataPayload FolderMetaDataCmd;
	 
 case FOLDER_MOVE:
	 FolderMovePayload FolderMoveCmd;
	 
 case FOLDER_OPEN:
	 FolderOpenPayload FolderOpenCmd;
	 
 case FOLDER_SHARE:
	 FolderSharePayload FolderShareCmd;
	 
 case FOLDER_LIST:
	 FolderListPayload FolderListCmd;
	 
 case NOT_SUPPORTED:
	 void;

 case RESERVED_CMD:
	 void;

 case SERVER_CONFIGURE:
	 ServerConfigurePayload ServerConfigCmd;
	 
 case SERVER_KICK_USER:
	 void;
	 
 case SERVER_LOGS:
	 ServerLogsPayload ServerLogsCmd;
	 
 case SERVER_MANAGE_BANS:
	 ServerManageBansPayload ServerBansCmd;
	 
 case SERVER_SHUTDOWN:
	 ServerShutdownPayload ServerShutdownCmd;
	 
 case SERVER_VIEW_STATS:
	 ServerStatsPayload ServerStatsCmd;
	 
 case USER_CREATE:
	 UserCreatePayload UserCreateCmd;
	 
 case USER_DELETE:
	 UserDeletePayload UserDeleteCmd;
	 
 case USER_LIST:
	 UserListPayload UserListCmd;
	 
 case USER_PERMISSIONS:
	 UserPermissionsPayload UserPermissionsCmd;
	 
 case USER_RENAME:
	 UserRenamePayload UserRenameCmd;

 default:
	 VendorDefined Vendor;
};

/**
 * A CMDReply payload is one of these types.
 * With Cmd set to a CMD value.
 */ 
union OneReply switch (CMD_e Cmd) {
	 
 case AUTHANONYMOUS:
	 void; /* Replies with CAPABILITY_PRE or CAPABILITY_POST */
	 
 case AUTHCERT_TLS:
	 AuthCertTlsReplyPayload AuthCertTlsCmd;
	 
 case AUTHCERT_USER:
	 AuthCertUserReplyPayload AuthCertUserCmd;
	 
 case AUTHMD5:
	 void;

 case BYE:
	 ByeReplyPayload ByeCmd;
	 
 case FILE_CREATE:
	 FileCreateReplyPayload FileCreateCmd;
	 
 case FILE_COPY:
	 FileCopyReplyPayload FileCopyCmd;
	 
 case FILE_DELETE:
	 FileDeleteReplyPayload FileDeleteCmd;
	 
 case FILE_RENAME:
	 FileRenameReplyPayload FileRenameCmd;
	 
 case FILE_METADATA:
	 FileMetaDataReplyPayload FileMetaDataCmd;
	 
 case FILE_MOVE:
	 FileMoveReplyPayload FileMoveCmd;
	 
 case FILE_SHARE:
	 FileShareReplyPayload FileShareCmd;
	 
 case FILE_GET:
	 FileGetReplyPayload FileGetCmd;
	 
 case FILE_MODIFY:
	 FileModifyReplyPayload FileModifyCmd;
	 
 case FOLDER_CREATE:
	 FolderCreateReplyPayload FolderCreateCmd;
	 
 case FOLDER_COPY:
	 FolderCopyReplyPayload FolderCopyCmd;
	 
 case FOLDER_DELETE:
	 FolderDeleteReplyPayload FolderDeleteCmd;
	 
 case FOLDER_RENAME:
	 FolderRenameReplyPayload FolderRenameCmd;
	 
 case FOLDER_METADATA:
	 FolderMetaDataReplyPayload FolderMetaDataCmd;
	 
 case FOLDER_MOVE:
	 FolderMoveReplyPayload FolderMoveCmd;
	 
 case FOLDER_OPEN:
	 FolderOpenReplyPayload FolderOpenCmd;
	 
 case FOLDER_SHARE:
	 FolderShareReplyPayload FolderShareCmd;
	 
 case FOLDER_LIST:
	 FolderListReplyPayload FolderListCmd;

 case RESERVED_CMD:
	 void;

 case SERVER_CONFIGURE:
	 ServerConfigureReplyPayload ServerConfigCmd;
		 
 case SERVER_LOGS:
	 ServerLogsReplyPayload ServerLogsCmd;
	 
 case SERVER_MANAGE_BANS:
	 ServerManageBansReplyPayload ServerManageBansCmd;
	 
 case SERVER_SHUTDOWN:
	 ServerShutdownReplyPayload ServerShutdownCmd;
	 
 case SERVER_VIEW_STATS:
	 ServerStatsReplyPayload ServerViewStatsCmd;
	 
 case USER_CREATE:
	 UserCreateReplyPayload UserCreateCmd;
	 
 case USER_DELETE:
	 UserDeleteReplyPayload UserDeleteCmd;
	 
 case USER_LIST:
	 UserListReplyPayload UserListCmd;
	 
 case USER_PERMISSIONS:
	 UserPermissionsReplyPayload UserPermissionsCmd;
	 
 case USER_RENAME:
	 UserRenameReplyPayload UserRenameCmd;

 default:
	 VendorDefined Vendor;
};

/*
 * One command.
 */
struct Command {
	SEQ_t      Sequence;
	OneCommand Payload;
};

/*
 * One Reply
 */
struct CommandReply {
	SEQ_t			Sequence;
	OneReply	Payload;
};

/**
 * A packet body.
 */
struct PacketBody {
  Command Commands<>; /* XDR arrays start with the Length */
};

/**
 * A packet reply.
 */
struct PacketReply {
  CommandReply Commands<>; /* XDR arrays start with the Length */
};
