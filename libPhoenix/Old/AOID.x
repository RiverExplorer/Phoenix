/**
 * ADMIN commands.
 * In XDR, enum values are 32-bit.
 * The high bit is zero, which means the PHOENIX_BIT is zero
 * in these values.
 */
enum AOID_e {
    SERVER_CONFIGURE   = 0x00,
    SERVER_KICK_USER   = 0x01,
    SERVER_LOGS        = 0x02,
    SERVER_MANAGE_BANS = 0x03,
    SERVER_SHUTDOWN    = 0x04,
    SERVER_VIEW_STATS  = 0x05,
    USER_CREATE        = 0x06,
    USER_DELETE        = 0x07,
    USER_LIST          = 0x08,
    USER_PERMISSIONS   = 0x09,
    USER_RENAME        = 0x0A
};

/**
 * All ADMIN commands are sent as 32-bit values.
 */
typedef AOID_e AOID;
