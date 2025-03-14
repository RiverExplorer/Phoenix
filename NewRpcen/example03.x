// text
namespace RiverExplorer:rpcgen:test ;

union CapabilityEntry switch (CMD_e CapabilityCmd) {

	case 0:
	int Int<10>;

	// text 2
	case 1:
	float Float[20];

 case 2:
	 opaque O2<>;

 case 3:
	 opaque O3<15>;

 case 4:
	 opaque O4[22];

 case 5:
	 opaque * O5<>;

 case 6:
	 opaque * O6<15>;

 case 7:
	 opaque * O7[22];

 case 11:
	 RandomName  U11<>;

 case 12:
	 RandomName U12<11>;

 case 13:
	 RandomName U13[33];

 case 14:
	 RandomName  * U14<>;

 case 15:
	 RandomName * U15<11>;

 case 16:
	 RandomName * U16[33];
	 
	 case AUTHANONYMOUS :
		 void;
		
	 case AUTHCERT_TLS :
		 void;
		
	 case AUTHCERT_USER :
		 void;
		
	 case AUTHMD5 :
		 AuthMD5 * AuthMD5Payload;
		
	 case BYE :
		 void;
		
	 case CAPABILITY_PRE :
		 Capability * Capabilities<>;
		
	 case CAPABILITY_POST :
		 void;
		
	 case FILE_CREATE :
		 FileCreate * FileCreatePayload;
		
	 case FILE_COPY :
		 FileCopy * FileCopyPayload;
		
	 case FILE_DELETE :
		 FileDelete * FileDeletePayload;
		
	 case FILE_RENAME :
		 FileRename * FileRenamePayload;
		
	 case FILE_METADATA :
		 FileMetaData * FileMetaDataPayload<10>;
		
	 case FILE_MOVE :
		 FileMove * FileMovePayload;
		
	 case FILE_SHARE :
		 FileShare * FileSharePayload;
		
	 case FILE_GET :
		 FileGet * FileGetPayload;
		
	 case FILE_MODIFY :
		 FileModify * FileModifyPayload;
		
	 case FOLDER_CAPABILITY :
		 FolderCapability FolderCapabilityPayload;
		
	 case FOLDER_CREATE :
		 FolderCreate * FolderCreatePayload;
		
	 case FOLDER_COPY :
		 FolderCopy * FolderCopyPayload;
		
	 case FOLDER_DELETE :
		 FolderDelete * FolderDeletePayload;
		
	 case FOLDER_RENAME :
		 FolderRename * FolderRenamePayload;
		
	 case FOLDER_METADATA :
		 FolderMetaData * FolderMetaDataPayload[25];
		
	 case FOLDER_MOVE :
		 FolderMove * FolderMovePayload;
		
	 case FOLDER_OPEN :
		 FolderOpen * FolderOpenPayload;
		
	 case FOLDER_SHARE :
		 FolderShare * FolderSharePayload;
		
	 case FOLDER_LIST :
		 FolderList * FolderListPayload<>;
		
	 case NOT_SUPPORTED :
		 void;
		
	 case PING :
		 void;
		
	 case RESERVED_CMD :
		 void;
		
	 case SERVER_CONFIGURE :
		 ServerConfigure * ServerConfigurePayload<>;
		
	 case SERVER_KICK_USER :
		 void;
		
	 case SERVER_LOGS :
		 void;
		
	 case SERVER_MANAGE_BANS :
		 void;
		
	 case SERVER_SHUTDOWN :
		 void;
		
	 case SERVER_VIEW_STATS :
		 void;
		
	 case USER_CREATE :
		 void;
		
	 case USER_DELETE :
		 void;
		
	 case USER_LIST :
		 void;
		
	 case USER_PERMISSIONS :
		 void;
		
	 case USER_RENAME :
		 void;
		
	 case VENDOR_ID :
		 string VendorID<>;

	 default :
		 VendorCapability Vendor;
};
