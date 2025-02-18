#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "AOID.hpp"
%#include "ServerConfigPayload.hpp"
%#include "ServerConfigReplyPayload.hpp"
%#include "AdminKick.hpp"
%#include "ServerLogs.hpp"
%#include "ManageBans.hpp"
%#include "Shutdown.hpp"
%#include "ServerStats.hpp"
%#include "UserCreate.hpp"
%#include "UserDelete.hpp"
%#include "UserList.hpp"
%#include "UserPermissions.hpp"
%#include "UserRename.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/AOID.hpp>
%#include <RiverExplorer/Phoenix/ServerConfigPayload.hpp>
%#include <RiverExplorer/Phoenix/ServerConfigReplyPayload.hpp>
%#include <RiverExplorer/Phoenix/AdminKick.hpp>
%#include <RiverExplorer/Phoenix/ServerLogs.hpp>
%#include <RiverExplorer/Phoenix/ManageBans.hpp>
%#include <RiverExplorer/Phoenix/Shutdown.hpp>
%#include <RiverExplorer/Phoenix/ServerStats.hpp>
%#include <RiverExplorer/Phoenix/UserCreate.hpp>
%#include <RiverExplorer/Phoenix/UserDelete.hpp>
%#include <RiverExplorer/Phoenix/UserList.hpp>
%#include <RiverExplorer/Phoenix/UserPermissions.hpp>
%#include <RiverExplorer/Phoenix/UserRename.hpp>
%#endif
#endif

union AdminPayload switch(AOID_e Aoid) {

  case SERVER_CONFIGURE:
    ServerConfigPayload ConfigPayload;

  case SERVER_KICK_USER:
    ServerKickPayload   KickPayload;
	
	case SERVER_LOGS:
    ServerLogsPayload   LogsPayload;
		
  case SERVER_MANAGE_BANS:
    ServerBansPayload   BansPayload;
		
  case SERVER_SHUTDOWN:
    ServerShutdownPayload ShutdownPayload;
		
  case SERVER_VIEW_STATS:
    ServerStatsPayload  StatsPayload;
		
  case USER_CREATE:
    UserCreatePayload   UCreatePayload;
		
  case USER_DELETE:
    UserDeletePayload   UDeletePayload;
		
  case USER_LIST:
    UserListPayload     UListPayload;
		
  case USER_PERMISSIONS:
    UserPermissionsPayload UPermissionsPayload;
		
  case USER_RENAME:
    UserRenamePayload   URenamePayload;
};

union AdminReplyPayload switch(AOID_e Aoid) {

  case SERVER_CONFIGURE:
    ServerConfigReplyPayload    ConfigPayload;

  case SERVER_KICK_USER:
		void; /* No reply allowd for SERVER_KICK_USER */
	
	case SERVER_LOGS:
    ServerLogsReplyPayload      LogsPayload;
		
  case SERVER_MANAGE_BANS:
    ServerBansReplyPayload      BansPayload;
		
  case SERVER_SHUTDOWN:
    ServerShutdownReplyPayload  ShutdownPayload;
		
  case SERVER_VIEW_STATS:
    ServerStatsReplyPayload     StatsPayload;
		
  case USER_CREATE:
    UserCreateReplyPayload      UCreatePayload;
		
  case USER_DELETE:
    UserDeleteReplyPayload      UDeletePayload;
		
  case USER_LIST:
    UserListReplyPayload        UListPayload;
		
  case USER_PERMISSIONS:
    UserPermissionsReplyPayload UPermissionsPayload;
		
  case USER_RENAME:
    UserRenameReplyPayload      URenamePayload;
};
