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
 * An array of OpConfigSet values.
 */
struct ServerConfigurePayload {
	CMD_e				Aoid;      /* Set to SERVER_CONFIGURE. */
	ConfigSet		Values<>;  /* XDR arrays start with a length. */
};
