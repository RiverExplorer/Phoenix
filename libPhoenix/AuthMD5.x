%
%#ifdef BUILDING_LIBPHOENIX
%#include "AuthMD5.hpp"
%#include "Commands.hpp"
%#include "SEQ_t.hpp"
%#else
%#include <RiverExplorer/Phoenix/AuthMD5.hpp>
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/SEQ_t.hpp>
%#endif
%#include <string>

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::AuthMD5
% * The client is authenticating.
% *
% * A reply is required.
% * Good: Reply with CAPABILITY_POST
% * Bad: An CAPABILITY_PRE with same sequence number from the AUTH.
% */
#endif

struct AuthMD5Payload
{
	string AccountName<>;
	string Md5Password<>;
};

#ifdef RPC_HDR
%namespace RiverExplorer::Phoenix {
%	/**
%	 * Construct a new AUTHMD5 command.
%	 *
%	 * @param Sequence The sequence number to create.
%	 *
%	 * @return The new command.
%	 */
%	extern Command * NewAuthMD5(SEQ_t Sequence);
%
%	/**
%	 * The authentication information was valid.
%	 * Generate a good reply.
%	 * Which is a CAPABILITY_POST.
%	 *
%	 * @return A Command set to CAPABILITY_POST with the
%	 * list of post authentication capabilities.
%	 */
%	extern Command * AuthMD5Good();
%
%	/**
%	 * The authentication information was NOT valid.
%	 * Generate a BAD reply.
%	 * Which is a CAPABILITY_PRE with the sequence
%	 * number the same as in the AUTH that was sent..
%	 *
%	 * @return A Command set to CAPABILITY_PRE with the
%	 * list of post authentication capabilities.
%	 */
%	extern Command * AuthMD5Bad(SEQ_t Sequence);
%
%	/**
%	 * Generate an MD5 string from a plain text string.
%	 *
%	 * @param Plain The plain text string.
%	 *
%	 * @return The MD5 of the plain text string.
%	 */
%	extern std::string AuthMD5MD5(std::string Plain);
%}
#endif
