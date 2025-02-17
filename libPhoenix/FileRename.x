#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
 /*%#include "File.hpp"*/
%#else
 /*%#include <RiverExplorer/Phoenix/File.hpp>*/
%#endif
#endif

struct FileRenamePayload {
	int foo;
};

struct FileRenameReplyPayload {
	int foo;
};
