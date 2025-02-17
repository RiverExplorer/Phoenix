#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#endif
#endif
/**
 * The number of emails in the MDN record set.
 */
typedef Length MDNListCount;

/**
 * When the MDN was sent.
 */
typedef UTC MDNSent;

/**
 * A list of all MDN records for the associated object.
 */
struct MDNRecord {
    MDNSent   TimeSent;

    /* The first item in an XDR array, is its size (MDNListCount) */
    MDNRecord Entries<>; 
};
