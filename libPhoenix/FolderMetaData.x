#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
 /*%#include "File.hpp"*/
%#else
 /*%#include <RiverExplorer/Phoenix/File.hpp>*/
%#endif
#endif

struct FolderMetaDataPayload {
	int foo;
};

struct FolderMetaDataReplyPayload {
	int foo;
};
