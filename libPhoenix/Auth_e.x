#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#endif
#endif

enum Auth_e {
    AUTHANONYMOUS    = 0x26,
    AUTHMD5          = 0x10,
    AUTHCERT         = 0x11,
    AUTHCERT_TLS     = 0x27,
    AUTHCERT_USER    = 0x28
};
