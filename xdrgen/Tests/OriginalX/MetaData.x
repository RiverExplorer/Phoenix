#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file MetaData.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif

#ifdef RPC_HDR
%/**
% * @note
% * The "C" routines return a bool_t, the C++ API return a bool.
% */
%
%/**
% * @class RiverExplorer::Phoenix::MetaData
% * Generic MetaData is a string Key and string Value set.
% * This is a single value.
% */
#endif

class MetaData
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
