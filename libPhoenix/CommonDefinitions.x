#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file CommonDefinitions.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif
/**
 * The time in seconds since January 1 1970 in GMT.
 */
typedef uint64_t UTC;

/**
 * the number of octets from the beginning of 
 * the associated object.
 */
typedef uint32_t Offset;

/**
 * The number of octets in the associated object.
 */
typedef uint32_t Length;

/**
 * When a stringD value is set to this,
 * The the associated value has been deleted.
 */
const Deleted = 0xffffffff;

/**
 * A stringD is a string, or Deleted (the associated
 * data has been deleted).
 */
union stringD switch (uint32_t LengthOrDeleted) {

  case Deleted:
    void;

  default:
    opaque String<>;
};
	
/**
 * A string Key and its associated Value.
 * It uses stringD as the value to enable an indicator that the
 * key part (string part) has been deleted.
 */
struct KeyPair {
    string Key<>;
    stringD Value<>;
};

/**
 * An array of KeyPair objects.
 */
typedef KeyPair KeyPairArray<>;

/**
 * A 1-bit value;
 * The highest bit in the value, 1 means it is
 * a vendor extension.
 */
const VENDOR_BIT  = 0x1;

/**
 * A 1-bit value;
 * The highest bit in the value, 0 means it is
 * a Phoenix command or future Phoenix command.
 */
const PHOENIX_BIT = 0x0;

/**
 * Operations
 */
enum Op_e {
    OpSet    = 0x00,
    OpGet    = 0x01,
    OpUpdate = 0x02,
    OpDelete = 0x03
};

/**
 * Any one of OpSet, OpGet, OpUpdate, or OpDelete
 * cast to a (Op).
 */
typedef uint8_t Op;		
