%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#include "AuthMD5.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#include <RiverExplorer/Phoenix/AuthMD5.hpp>
%#endif

struct Authenticate
{
	AuthMD5	Md5;
};

struct AuthenticateReply
{
	bool_t	Accepted;
};

