#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#include "CommonDefinitions.hpp"
%#include "ConfigSet.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/ConfigSet.hpp>
%#endif
#endif

/**
 * Reply to SERVER_CONFIGURE
 */
struct ServerConfigureReplyPayload {
	CMD_e    ACmd;           /* Set to SERVER_CONFIGURE */
	KeyPair   ResultValues<>; /* XDR arrays start with a length */
};
