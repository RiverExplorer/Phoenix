%/**
%* Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
%* RiverExplorer is a trademark of RiverExplorer Games LLC                      
%*/
%
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#include "KeyString.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/KeyString.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * The Capability command informs the other endpoint about
% * its capabilities.
% *
% * This is done once at connection time.
% * And once after authentication is successful.
% * All other attempts will get a NotSupported_Cmd reply.
% *
% * A single capability is a string key, and a string value.
% * The value for each capability is described in its own
% * section.
% *
% * A capability value may contain one or more values, each separated
% * as defined in the capability description. 
% * It is suggested that a comma (HEX 2C) be used. Except when
% * that would complicate the capability value.
% * In all cases, the capability value is defined separately for
% * each capability.
% *
% * If the capability list does not include the capability name,
% * then it is not supported.
% *
% * If the reply contains the capability name, then
% * its associated value will be used to determine the 
% * extent of its support.
% *
% * Capability keys MUST BE processed in all lower case.
% * If a capability arrives in upper or mixed case,
% * the receiver MUST covert then check to see if that is
% * valid. This prevents capabilities with the same name
% * and varying case from being used.
% *
% * Capability values SHOULD BE in all lower case.
% * Except when the capability itself contains values
% * that must be upper or mixed case.
% *
% * Capabilties that have a boolean value, SHOULD use
% * 'true' or 'false' and not 'yes', 'no' or other variations of true or false.
% *
% * Capabilties that have a enabled or disabled value, SHOULD use
% * 'enabled' or 'disabled' and not 'yes', 'no' or other variations of
% * enabled or disabled.
% *
% */
#endif

struct Capability
{
	KeyString Known<>;
};

#ifdef RPC_HDR
%/**
% * Capability FolderCapabilities_e Capabilities that
% * apply to folder or directories.
% *
% * Folder capabilites should not be provided in pre-authentication
% * capability packets.
% * 
% * @note
% * Not call capabilities are applicable to all endpoints, files,
% * folders, users, or specific commands. Read the associated
% * command and capability specifications to understand each
% * operation.
% *
% * The capability specifies the endpoing is capable of the
% * function. It may be further restriced by who is authenticated
% * or acess control lists (ACLs) on specific associated items.
% *
% * <ul>
% * <li>CanList: Has a key of "canlist" and has a boolean value.
% * -When true, then a list of files and folders can be accessed.
% * -When false, then only already known named files and folders can be accessed.
% * </li>
% *
% * <li>CanSubscribe: Has a key value of "cansubscribe" and has
% * a boolean value.
% * -When true, then this server supports unsolicited push notifcations.
% * -When false, then no unsolicited push notifcations can not be sent
% * from this endpoint.
% * </li>
% *
% * <li>CanCreate: Has a key value of "cancreate" and has a boolean value.
% * -When true, then this endpoint supports creating folders.
% * -When false, this endpoint does not support creating folders.
% * <li>
% *
% * <li>CanRemove: Has a key value of "canremove" and has a boolean value.
% * -When true, this endpoint supports removing folders.
% * -When fales, this endpoint does not support removing foldres.
% * <li>
% *
% * <li>CanRename: Has a key value of "canrename" and has a boolean value.
% * When true, this endpoint supports renaming folders.
% * When false, this endpoint does not support renaming folders.
% * <li>
% *
% * <li>CanCopy: Has a key value of "cancopy" and has a boolean value.
% * When true, this endpoint supports copying folders.
% * When false, this endpoint does not support copying folders.
% * <li>
% *
% * <li>CanSearch: Has a key value of "cansearch" and has a boolean value.
% * When true, this endpoint supports searching folders names using posix regex expressions
% * When false, this endpoint does not support searching folders.
% * <li>
% *
% * <li>Acls: Has a key value of 'acls' and has a string value.
% * When supplied, the endpoint supports at least one acl type.
% * The types currely defined in the Acls_e enumeration.
% */
#endif

enum FolderCapabilities_e
{
	CanList,
	CanSubscribe,
	CanCreate,
	CanRemove,
	CanAppend,
	CanRename,
	CanUpdate,
	CanCopy,
	CanSearch,
	Acls
};

enum Acls_e
{
	OwnerRW_t,
	OwnerRO_t,
	GroupRW_t,
	GroupRO_t,
	OtherRW_t,
	OtherRO_t,
	NamedListRW_t,
	NamedListRO_t
};

