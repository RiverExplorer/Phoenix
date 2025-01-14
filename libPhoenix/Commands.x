
#ifdef RPC_HDR
%/**
% * Command_e: An enumerated list of fetch commands.
% * <ul>
% * <li>
% * NotSupported_Cmd: This endpoint received a Command_e
% * that it does not support. Send back a NotSupported_Cmd
% * and use the original Command_e as the command.
% * <li>
% * Ping_Cmd: (reply) and keepalive (no-reply).
% * A ping with reply, check the other end.
% * A ping without a reply, keeps the connection alive.
% * </li>
% * <li>
% * Capability_Cmd: Get the capability of the endpoint.
% * The endpoint lists it capabilities.
% * </li>
% * <li>
% * Authenticate_Cmd: Authenticate and Unauthenticate.
% * These are modled after the IMAP autentication methods.
% * </li>
% * <li>
% * Select_Cmd: Select a folder in R/W or R/O mode.
% * Select one or more folders. Each can be in R/W or R/O mode.
% * The client can make the request, the server is the authority
% * and might return a R/O status on a folder that was requested
% * R/W access.
% * </li>
% * <li>
% * Create_Cmd: Create a new folder.
% * Create a new folder, folders can have folders.
% * </li>
% * <li>
% * Delete_Cmd: Delete a folder.
% * Delete one or more folders and their entire contents.
% * </li>
% * <li>
% * Rename_Cmd: Rename a folder.
% * </li>
% * <li>
% * Close_Cmd: Folder, one or all.
% * Once a folder is closed, it must have a Select_Cmd issued on it
% * to access it again.
% * </li>
% * <li>List_Cmd:
% * Also allows more verbose information about one or more folders.
% * List available folders, and ask for specific information
% * for each one. The reply will give R/W access, ACL, and if the
% * authenticated user has subscribed for notifications on the folder.
% * There is one reply for each folder. The replies may come in
% * in one batch, or separatly. 
% * </li>
% * <li>
% * Subscribe_Cmd: Allows for subscribe and unsubscribe.
% * </li>
% * <li>
% * AddMessage_Cmd: Add a message to a folder.
% * </li>
% * <li>
% * UpdateMessage_Cmd: Alter part of a draft message.
% * </li>
% * <li>
% * GetMessage_Cmd: Get a message, all or parts. One or more.
% * This allows for part or all of a message to be fetched.
% * </li>
% * <li>
% * CopyMessage_Cmd: Copy a message to another folder.
% * </li>
% * <li>
% * Expunge_Cmd: Cleanup folder or message.
% * </li>
% * <li>
% * Search_Cmd: Search for messages.
% * </li>
% * <li>
% * Acl_Cmd_Cmd: Access control operations.
% * </li>
% * <li>
% * Timeout_Cmd: The endpoint timed out.
% * </li>
% * <li>
% * Threads_Cmd: Get or send thread information.
% * When known, provide all of the message ID's in this message thread.
% * </li>
% * <li>PublicKey_Cmd:
% * Store your public key, request anothers public key.
% * If in the capability list, store the public key for the authenticated
% * user in a way that other can fetch it.
% * Allows for requesting others public key.
% * Allows for a list of keys to be stored for old, expired, and revoked
% * keys so that messages can be authenticated within the range of their
% * validity.
% * </li>
% * </ul>
% */
#endif
enum Command_e
{
	NotSupported_Cmd = -1,
	Ping_Cmd = 0,
	Capability_Cmd = 1,
	Authenticate_Cmd = 2,
	Folder_Cmd = 3,
	Subscribe_Cmd = 4,
	Message_Cmd = 5,
	Expunge_Cmd = 6,
	Search_Cmd = 7,
	Acl_Cmd = 8,
 	Timeout_Cmd = 9
};

#ifdef RPC_HDR
%/**
% * Each command sent, has a command identifier, a CommandID.
% * Each reply to a command has the same CommandID.
% */
typedef uint64_t CommandID;

%/**
% * A command consists of:
% * - The enumerated command (Command_e),
% * - An XDR opaque object (length + data).
% * The XDR opaque data was prepared by the Cmd specific code
% * and is set into place for transport.
% */
#endif
struct  CmdPacket
{
	CommandID ID;
	Command_e Cmd;
	opaque		Data<>;
};

#ifdef RPC_HDR
%/**
% * The transport top level is simple.
% * You can Send() a packet and get a packet back.
% * Or you send a notifcation that receives nothing back.
% * Or you send a broadcast message to all interested participiants, with no
% * reply expected.
% */
#endif


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
%typedef bool (*CommandCallback)(int Fd, CmdPacket * Pkt, XDR * ReadXdrs);
%}
%
%/**
% * Set the callback for a command.
% *
% * @param Cmd The command being registerd.
% *
% * @param Callback The user supplied callback function.
% *
% * @note
% * It is is recommended that each registred callback be thread safe.
% */
%namespace RiverExplorer::Phoenix
%{
%extern bool Register_Cmd(Command_e Cmd, CommandCallback * Callback);
%}
#endif

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
#endif
%namespace RiverExplorer::Phoenix
%{
%extern bool xdr_bool_t(XDR * xdrs, bool_t * BValue);
%}

#ifdef RPC_HDR
/**
 * 64 bit network to host byte order conversion.
 *
 * @param NetValue A uint64_t value in network byte order.
 *
 * @return A uint64_t value in host order
 */
#endif
%namespace RiverExplorer::Phoenix
%{
%extern uint64_t ntohll(uint64_t NetValue);
%}

#ifdef RPC_HDR
/**
 * 64 bit host to network byte order conversion.
 *
 * @param HostValue A uint64_t value in host byte order.
 *
 * @return A uint64_t value in network byte order.
 */
#endif
%namespace RiverExplorer::Phoenix
%{
%extern uint64_t htonll(uint64_t NetValue);
%}
