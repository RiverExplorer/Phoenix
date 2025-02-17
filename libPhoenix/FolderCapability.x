#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
 /*%#include "File.hpp"*/
%#else
 /*%#include <RiverExplorer/Phoenix/File.hpp>*/
%#endif
#endif

struct FolderCapabilityPayload {
	int foo;
};

struct FolderCapabilityReplyPayload {
	int foo;
};
