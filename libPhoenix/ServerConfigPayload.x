#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "AOID.hpp"
%#include "ConfigSet.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/AOID.hpp>
%#include <RiverExplorer/Phoenix/ConfigSet.hpp>
%#endif
#endif

/**
 * An array of OpConfigSet values.
 */
struct ServerConfigPayload {
    AOID_e       Aoid;      /* Set to SERVER_CONFIGURE. */
    ConfigSet    Values<>;  /* XDR arrays start with a length. */
};
