#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
 /*%#include "File.hpp"*/
%#else
 /*%#include <RiverExplorer/Phoenix/File.hpp>*/
%#endif
#endif

struct FolderCreatePayload {
	int foo;
};

struct FolderCreateReplyPayload {
	int foo;
};
