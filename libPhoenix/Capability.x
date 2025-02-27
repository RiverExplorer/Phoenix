%
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "Commands.hpp"
%#include "SEQ_t.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/SEQ_t.hpp>
%#endif
%
%#include <vector>

struct VendorCapability
{
	Length Count;
	uint8_t * Data;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CapabilityEntry
% * Each capability has a CMD_e and may have data.
% */
#endif
union CapabilityEntry switch (CMD_e Capability) {
		
	 case AUTHANONYMOUS:
		 void;
		
	 case AUTHCERT_TLS:
		 void;
		
	 case AUTHCERT_USER:
		 void;
		
	 case AUTHMD5:
		 void;
		
	 case BYE:
		 void;
		
	 case CAPABILITY_PRE:
		 void;
		
	 case CAPABILITY_POST:
		 void;
		
	 case FILE_CREATE:
		 void;
		
	 case FILE_COPY:
		 void;
		
	 case FILE_DELETE:
		 void;
		
	 case FILE_RENAME:
		 void;
		
	 case FILE_METADATA:
		 void;
		
	 case FILE_MOVE:
		 void;
		
	 case FILE_SHARE:
		 void;
		
	 case FILE_GET:
		 void;
		
	 case FILE_MODIFY:
		 void;
		
	 case FOLDER_CAPABILITY:
		 void;
		
	 case FOLDER_CREATE:
		 void;
		
	 case FOLDER_COPY:
		 void;
		
	 case FOLDER_DELETE:
		 void;
		
	 case FOLDER_RENAME:
		 void;
		
	 case FOLDER_METADATA:
		 void;
		
	 case FOLDER_MOVE:
		 void;
		
	 case FOLDER_OPEN:
		 void;
		
	 case FOLDER_SHARE:
		 void;
		
	 case FOLDER_LIST:
		 void;
		
	 case NOT_SUPPORTED:
		 void;
		
	 case PING:
		 void;
		
	 case RESERVED_CMD:
		 void;
		
	 case SERVER_CONFIGURE:
		 void;
		
	 case SERVER_KICK_USER:
		 void;
		
	 case SERVER_LOGS:
		 void;
		
	 case SERVER_MANAGE_BANS:
		 void;
		
	 case SERVER_SHUTDOWN:
		 void;
		
	 case SERVER_VIEW_STATS:
		 void;
		
	 case USER_CREATE:
		 void;
		
	 case USER_DELETE:
		 void;
		
	 case USER_LIST:
		 void;
		
	 case USER_PERMISSIONS:
		 void;
		
	 case USER_RENAME:
		 void;
		
	 case VENDOR_ID:
		 string VendorID<>;

	 default:
		 VendorCapability Vendor;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CapabilityPrePayload
% * The client is authenticating.
% *
% * A reply is required.
% * Good: Reply with CAPABILITY_POST
% * Bad: An CAPABILITY_PRE with same sequence number from the AUTH.
% */
#endif
struct CapabilityPrePayload
{
	CapabilityEntry Capabilities<>; /** An array of CapabilityEntries */
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CapabilityPostPayload
% * The client is authenticating.
% *
% * A reply is required.
% * Good: Reply with CAPABILITY_POST
% * Bad: An CAPABILITY_POST with same sequence number from the AUTH.
% */
#endif
struct CapabilityPostPayload
{
	CapabilityEntry Capabilities<>; /** An array of CapabilityEntries */
};
