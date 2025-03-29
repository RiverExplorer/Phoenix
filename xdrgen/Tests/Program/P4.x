program Phoenix {
	version Draft_00 {
		void AUTHANONYMOUS_func(AUTHANONYMOUS_Payload*) = AUTHANONYMOUS;
		AUTHCERT_USER_ReplyPayload * AUTHCERT_USER_func(AUTHCERT_USER_Payload*) =  AUTHCERT_TLS;
		AUTHCERT_TLS_ReplyPayload * AUTHCERT_TLS_func(AUTHCERT_USER_Payload*) =  AUTHCERT_TLS;

		AUTHMD5_ReplyPayload * AUTHMD5_func(AUTHMD5_Payload*) = AUTHMD5;
		BYE_ReplyPayload * BYE_func(BYE_Payload*) = BYE;
		CAPABILITY_PRE_ReplyPayload * CAPABILITY_PRE_func(CAPABILITY_PRE_Payload*) = CAPABILITY_PRE;
		CAPABILITY_POST_ReplyPayload * CAPABILITY_POST_func(CAPABILITY_POST_Payload*) = CAPABILITY_POST;
		FILE_CREATE_ReplyPayload * FILE_CREATE_func(FILE_CREATE_Payload*) = FILE_CREATE;
		FILE_COPY_ReplyPayload * FILE_COPY_func(FILE_COPY_Payload*) = FILE_COPY;
		FILE_DELETE_ReplyPayload * FILE_DELETE_func(FILE_DELETE_Payload*) = FILE_DELETE;
		FILE_RENAME_ReplyPayload * FILE_RENAME_func(FILE_RENAME_Payload*) = FILE_RENAME;
		FILE_METADATA_ReplyPayload * FILE_METADATA_func(FILE_METADATA_Payload*) = FILE_METADATA;
		FILE_MOVE_ReplyPayload * FILE_MOVE_func(FILE_MOVE_Payload*) = FILE_MOVE;
		FILE_SHARE_ReplyPayload * FILE_SHARE_func(FILE_SHARE_Payload*) = FILE_SHARE;
		FILE_GET_ReplyPayload * FILE_GET_func(FILE_GET_Payload*) = FILE_GET;
		FILE_MODIFY_ReplyPayload * FILE_MODIFY_func(FILE_MODIFY_Payload*) = FILE_MODIFY;
		FOLDER_CAPABILITY_ReplyPayload * FOLDER_CAPABILITY_func(FOLDER_CAPABILITY_Payload*) = FOLDER_CAPABILITY;
		FOLDER_CREATE_ReplyPayload * FOLDER_CREATE_func(FOLDER_CREATE_Payload*) = FOLDER_CREATE;
		FOLDER_COPY_ReplyPayload * FOLDER_COPY_func(FOLDER_COPY_Payload*) = FOLDER_COPY;
		FOLDER_DELETE_ReplyPayload * FOLDER_DELETE_func(FOLDER_DELETE_Payload*) = FOLDER_DELETE;
		FOLDER_RENAME_ReplyPayload * FOLDER_RENAME_func(FOLDER_RENAME_Payload*) = FOLDER_RENAME;
		FOLDER_METADATA_ReplyPayload * FOLDER_METADATA_func(FOLDER_METADATA_Payload*) = FOLDER_METADATA;
		FOLDER_MOVE_ReplyPayload * FOLDER_MOVE_func(FOLDER_MOVE_Payload*) = FOLDER_MOVE;
		FOLDER_OPEN_ReplyPayload * FOLDER_OPEN_func(FOLDER_OPEN_Payload*) = FOLDER_OPEN;
		FOLDER_SHARE_ReplyPayload * FOLDER_SHARE_func(FOLDER_SHARE_Payload*) = FOLDER_SHARE;
		FOLDER_LIST_ReplyPayload * FOLDER_LIST_func(FOLDER_LIST_Payload*) = FOLDER_LIST;
		NOT_SUPPORTED_ReplyPayload * NOT_SUPPORTED_func(NOT_SUPPORTED_Payload*) = NOT_SUPPORTED;
		PING_ReplyPayload * PING_func(PING_Payload*) = PING;
		RESERVED_CMD_ReplyPayload * RESERVED_CMD_func(RESERVED_CMD_Payload*) = RESERVED_CMD;
		SERVER_CONFIGURE_ReplyPayload * SERVER_CONFIGURE_func(SERVER_CONFIGURE_Payload*) = SERVER_CONFIGURE;
		SERVER_KICK_USER_ReplyPayload * SERVER_KICK_USER_func(SERVER_KICK_USER_Payload*) = SERVER_KICK_USER;
		SERVER_LOGS_ReplyPayload * SERVER_LOGS_func(SERVER_LOGS_Payload*) = SERVER_LOGS;
		SERVER_MANAGE_BANS_ReplyPayload * SERVER_MANAGE_BANS_func(SERVER_MANAGE_BANS_Payload*) = SERVER_MANAGE_BANS;
		SERVER_SHUTDOWN_ReplyPayload * SERVER_SHUTDOWN_func(SERVER_SHUTDOWN_Payload*) = SERVER_SHUTDOWN;
		SERVER_VIEW_STATS_ReplyPayload * SERVER_VIEW_STATS_func(SERVER_VIEW_STATS_Payload*) = SERVER_VIEW_STATS;
		USER_CREATE_ReplyPayload * USER_CREATE_func(USER_CREATE_Payload*) = USER_CREATE;
		USER_DELETE_ReplyPayload * USER_DELETE_func(USER_DELETE_Payload*) = USER_DELETE;
		USER_LIST_ReplyPayload * USER_LIST_func(USER_LIST_Payload*) = USER_LIST;
		USER_PERMISSIONS_ReplyPayload * USER_PERMISSIONS_func(USER_PERMISSIONS_Payload*) = USER_PERMISSIONS;
		USER_RENAME_ReplyPayload * USER_RENAME_func(USER_RENAME_Payload*) = USER_RENAME;
		VENDOR_ID_ReplyPayload * VENDOR_ID_func(VENDOR_ID_Payload*) = VENDOR_ID;
	} = 0;

	version Draft_01 {
		double LoginName(int) = 98;
		float LoginName(void) = 100;
		int	 UserId(void) = 99;
	} = 1;
} = 6112;
