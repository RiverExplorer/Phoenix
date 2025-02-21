%
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#include "SEQ_t.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/SEQ_t.hpp>
%#endif
%
%#include <vector>

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CapabilityEntry
% * Each capability has a CMD_e and may have data.
% */
#endif
struct CapabilityEntry
{
	CMD_e	Capability;
	opaque Data<>; /* May be zero length */
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CapabilityPrePayload
% * The client is authenticating.
% *
% * A reply is required.
% * Good: Reply with CAPABILITY_POST
% * Bad: An CAPABILITY_PRE with same sequence number from the AUTH.
% */
#endif
struct CapabilityPrePayload
{
	CapabilityEntry Capabilities<>; /** An array of CapabilityEntries */
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::CapabilityPostPayload
% * The client is authenticating.
% *
% * A reply is required.
% * Good: Reply with CAPABILITY_POST
% * Bad: An CAPABILITY_POST with same sequence number from the AUTH.
% */
#endif
struct CapabilityPostPayload
{
	CapabilityEntry Capabilities<>; /** An array of CapabilityEntries */
};

#ifdef RPC_HDR
%namespace RiverExplorer::Phoenix {
%	/**
%	 * Gathers the registered PRE capabilities and creates
%	 * the command.
%	 */
%	Command * NewCapabilityPre(SEQ_t Sequence);
%	
%	/**
%	 * Gathers the registered POST capabilities and creates
%	 * the command.
%	 */
%	Command * NewCapabilityPost(SEQ_t Sequence);
%}
#endif
