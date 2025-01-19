%/**
%* Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
%* RiverExplorer is a trademark of RiverExplorer Games LLC                      
%*/
%
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * Authentication is initiated by the endpoing doing the inital
% * connection, to the endpoint it is connecting to.
% *
% * The start of the authentication process take one of two
% * directions:
% *
% * (1) New account, new client, or new authentication mechanism being
% * attempted.
% *
% * (2) - Existing account from a previously used client using a
% * previously known to work (from the clients point of view) authentication
% * mechanism.
% *
% * New is divided up into (1.a) unknown or new account or (1.b) known account.
% *
% * (1.a) New, known account:
% * Just after the network connection is made, the client sends its
% * pre authentication capabilities to the server, then waits
% * for the pre authentication capabilities packet to arrive from
% * the server. The server supplied pre authentication packet includes
% * the authentication mechanisms it suppots.
% * 
% * (1.b) New, unknown account:
% * Starts off like (a), then checks the servers capability set for
% * "allow-new-accounts". If provided, then the sign-up procedure is
% * followed. (sign-up documentation below). Followed by an authentication
% * to verify it worked.
% *
% * If "allow-new-accounts" is not provided, then the account information
% * must be aquired by methods external to this protocol.
% *
% * @note
% * Allowing new users to sign up using this protocol can be site
% * specific and may include procedures external to this protocol
% * such as visiting web sites or other external verification processes.
% *
% * @note
% * For security and anti-junk user accounts, many site may choose not enable
% * "allow-new-accounts". For servers internal to orginazations or on secure
% * networks this might be enabled.
% *
% * (2) Existing or known:
% * Just after the network connection is made, the client sends its
% * pre authentication capabilities to the server. Then without
% * waiting sends any first step authentication data to the server.
% * These are two separate packets each unique and independent. 
% *
% * In both cases:
% * Authentication proceeds.
% */
#endif
struct AuthMD5
{
	string Account<>;
	string Md5Password<>;
};
