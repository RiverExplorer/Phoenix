#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "AOID.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/AOID.hpp>
%#endif
#endif
/**
 * Reply to SERVER_CONFIGURE
 */
struct ServerConfigReplyPayload {
    AOID_e    ACmd;           /* Set to SERVER_CONFIGURE */
    KeyPair   ResultValues<>; /* XDR arrays start with a length */
};
