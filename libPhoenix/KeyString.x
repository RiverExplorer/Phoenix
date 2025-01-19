%/**
%* Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
%* RiverExplorer is a trademark of RiverExplorer Games LLC                      
%*/
%

#ifdef RPC_HDR
%/**
% * @note
% * The "C" routines return a bool_t, the C++ API return a bool.
% */
%
%/**
% * Generic string Key and string Value set.
% * This is a single value.
% */
#endif

struct KeyString
{
#ifdef RPC_HDR 
%	/**
%	 * The key to the key/value pair.
%	 */
#endif
	string Key<>;
#ifdef RPC_HDR
%
%	/**
%	 * The value part of the the key/value pair.
%	 */
#endif
	string Value<>;
};
