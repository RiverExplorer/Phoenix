#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 10:39:39 doug>
% *
% * @file Capability.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif
%
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "Commands.hpp"
%#include "SEQ_t.hpp"
%#include "Vendor.hpp"
%#include "VendorID.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/SEQ_t.hpp>
%#include <RiverExplorer/Phoenix/Vendor.hpp>
%#include <RiverExplorer/Phoenix/VendorID.hpp>
%#endif
%
%#include <vector>

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
		 VendorID VendorIDCapability;

	 default:
		 Vendor VendorCapability;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CapabilityPayload
% */
#endif
struct CapabilityPayload
{
	CapabilityEntry Capabilities<>; /** An array of CapabilityEntries */
};
