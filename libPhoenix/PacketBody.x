#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "CommonDefinitions.hpp"
%#include "Admin.hpp"
%#include "Auth.hpp"
%#include "FilePayload.hpp"
%#include "ProtoPayload.hpp"
%#else
%#include <RiverExplorer/Phoenix/CommonDefinitions.hpp>
%#include <RiverExplorer/Phoenix/Admin.hpp>
%#include <RiverExplorer/Phoenix/Auth.hpp>
%#include <RiverExplorer/Phoenix/ProtoPayload.hpp>
%#endif
#endif
/*
 * The ADMIN commands.
 * An XDR enum is 32-bits in size.
 * With the VENDOR_BIT set to zero, making it not a vendor command.
 */
enum CMD {
    /**
     * The data that follows is ADMIN CMD data.
     */
    ADMIN_CMD  = 0x00,
		 
    /**
     * The data that follows is AUTH CMD data.
     */
    AUTH_CMD   = 0x01,
		 
    /**
     * The data that follows is FILE or FOLDER CMD data.
     */
    FILE_CMD   = 0x02,
		 
    /**
     * The data that follows is Phoenix PROTO CMD data.
     */
    PROTO_CMD  = 0x03
};

/**
 * Mask to check if CMD value in packet
 * is vendor extension.
 */
const CMD_VENDOR_MASK = 0x80000000;

/**
 * Define a SEQ (sequence) type.
 */
typedef uint32_t  SEQ_t;

/**
 * A CMD payload is one of these types.
 * With Cmd set to a CMD value.
 */ 
union CmdPayload switch (CMD Cmd) {

    case ADMIN_CMD:
        AdminPayload AdminBodyPayload;
		
    case AUTH_CMD:
        AuthPayload  AuthBodyPayload;
		
    case FILE_CMD:
        FilePayload  FileBodyPayload;
		
    case PROTO_CMD:
        ProtoPayload ProtoBodyPayload;
};

/**
 * A packet body.
 */
struct PacketBody {
    SEQ_t      SEQ;
    CmdPayload Payload;
};
