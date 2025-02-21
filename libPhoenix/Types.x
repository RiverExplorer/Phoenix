#ifdef RPC_HDR
/**
 * The orignal RPCGEN was "C" only, RPCGEN++ is "C++".
 * C useed bool_t, and C++ uses bool.
 * So this is a wrapper.
 *
 * @param xdrs An initialized XDR object.
 *
 * @param BValue The address of the bool_t object.
 *
 * @return false if failed.
 */
%namespace RiverExplorer::Phoenix
%{
%extern bool xdr_bool_t(XDR * xdrs, bool_t * BValue);
%}
#endif

#ifdef RPC_HDR
%/**
% * An array of strings.
% */
#endif
typedef string StringType<>;

#ifdef RPC_HDR
%/**
% * An array of strings.
% */
#endif
typedef StringType ArrayOfStrings<>;

#ifdef RPC_HDR
%/**
% * An object to data objects, some of which might
% * be memory mapped.
% *
% * - IsMMapped: When true, Data was memory mapped and not allocated.
% * - IsAllocated When true, Data was allocated.
% *
% * @note
% * Both IsMMapped and IsAllocated can be false when it is pointing
% * to a subset of an allocated or mmapped data.
% *
% * - Len The number of octets in Data.
% *
% * - Data A pointer to the data.
% */
#endif
class IoVec
{
	bool_t		IsMMapped;
	bool_t		IsAllocated;
	uint32_t	Length;
	uint8_t	* Data;	
};
