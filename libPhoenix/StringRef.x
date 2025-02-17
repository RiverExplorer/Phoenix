#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#endif
#endif
/**
 * A reference to the start and length of a string.
 */
struct StringRef {
    Offset StartOffset;
    Length StringLength;
};				
