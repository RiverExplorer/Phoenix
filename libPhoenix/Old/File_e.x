#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#endif
#endif

enum File_e {
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
		 FILE_CREATE = 0x1d,
		 FILE_COPY = 0x1e,
		 FILE_DELETE = 0x1f,
		 FILE_RENAME = 0x20,
		 FILE_METADATA = 0x21,
		 FILE_MOVE = 0x22,
		 FILE_SHARE = 0x23,
		 FILE_GET = 0x24,
		 FILE_MODIFY = 0x25
};
