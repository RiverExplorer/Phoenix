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

%/**
% * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
% * RiverExplorer is a trademark of RiverExplorer Games LLC                      
% */

%
#ifdef RPC_HDR
%/**
% * Each command sent, has a command identifier, and a sequence.
% * Each reply to a command has the same CommandID and sequence.
% */
#endif
typedef uint64_t CommandSequence;

#ifdef RPC_HDR
%/**
% * An array of strings.
% */
#endif
typedef string StringType<>;

#ifdef RPC_HDR
%/**
% * Commands come in from the remote endpoint.
% * Each over the wire command includes its size.
% * The first value is the command, followed by
% * the size of data that follow. Followed by the related data.
% *
% * This is the callback signature to register to process a command.
% *
% * @param Fd The associated file descriptor.
% *
% * @param ReadXdrs An blob of XDR data to be decoded, parsed,
% * and processed.
% *
% * @return The callback returns TRUE on all okay, or FALSE
% * on an error.
% *
% * @note
% * It is is recommended that each registred callback be thread safe.
% *
% * @note
% * More than one function can me registerd for any command.
% * When there is more than one function registered for any specific
% * command, they are processed in random order.
% */
%namespace RiverExplorer::Phoenix
%{
%class CmdPacket;
%typedef bool (*CommandCallback)(int Fd, CmdPacket * Pkt, XDR * ReadXdrs);
%}
#endif
