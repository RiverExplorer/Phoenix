#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#endif
#endif
/**
 * A single MDN entry, when (or zero) and the email.
 */
struct MDMEntry
{
    UTC  Received;
    string EMail<>;
};    
