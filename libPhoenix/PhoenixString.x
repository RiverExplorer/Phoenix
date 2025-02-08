%
%#ifdef BUILDING_LIBPHOENIX
%#include "PhoenixString.hpp"
%#else
%#include <RiverExplorer/Phoenix/PhoenixString.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @enum RiverExplorer::Phoenix::PhoenixString
% * This is the XDR specification for the PhoenixString
% * described in the specification:
% * <a href="https://htmlpreview.github.io/?https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/draft-royer-phoenix.html#name-phoenixstring" target="_blank">Phoenix String (PhoenixString)</a>
% *
% * -Offset
% * The offfset from the start of the object, to the first character
% * in the string.
% * 
% * -Length The number of octets in the string.
% * Not including any terminating zero.
% * 
% * -String
% * The string octets, not including any terminating zero.
% */
#endif
class PhoenixString
{
	uint32_t	Offset;
	string		String<>;
};

#ifdef RPC_HDR
%/**
% * Create a phoenix string.
% *
% * @param Offset The offset to the start of the string.
% *
% * @param Length The length of the string.
% *
% * @return A new PhoenixString object.
% */
%namespace RiverExplorer::Phoenix {
%PhoenixString * NewPhoenixString(uint32_t Offset, uint32_t Length);
%}
#endif
