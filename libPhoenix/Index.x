%
%#ifdef BUILDING_LIBPHOENIX
%#include "Index.hpp"
%#else
%#include <RiverExplorer/Phoenix/Index.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @file Index.x
% * This is the XDR specification for the Interested header
% * list described in [Phoenix Index]https://RiverExplorer.games/Phoenix/
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% */
#endif
class HeaderID
{
	uint8_t	ClientHeaderID;
	string	Name<>;
};

#ifdef RPC_HDR
%/**
% * An array of interested headers.
% */
class InterestedHeader
{
	HeaderID	Interested<>;
};

#ifdef RPC_HDR
%/**
% * This is a MIME folder index for one Header index.
% *
% * -ClientHeaderID is the client assigned ID for the header found.
% *
% * -Value Is the offset from the start of the MIMEo object for
% * related header value.
% *
% * -Length Is the number of octets in the value.
% */
#endif
class InterestedHeaders
{
	uint8_t		ClientHeaderID;
	uint32_t	Value;
	uint32_t	Length;
};

#ifdef RPC_HDR
%/**
% * This is an index to a MIME object in a MIME folder.
% *
% * -The Offset is the octet count from the beginning of the MIME folder
% * where this MIME object starts.
% *
% * -The Length is the length of the MIME object.
% *
% * -The Headers is an array of InterestedHeaders providing the
% * octet offset into the MIME object where the client interested headers
% * exist. It is an ordered list, in the same order as in the MIME object.
% * When multiple identical header names exist, then this array will
% * have multiple entries with the same ID, each with their unique
% * offset for that instance of the header name.
% */
#endif
class MIMEObjectIndex
{
	uint32_t	Offset;
	uint32_t	Length;
	InterestedHeaders MimeHeaders<>;
	InterestedHeaders BodyHeaders<>;
};

#ifdef RPC_HDR
%/**
% * This is a MIME folder index to zero or more MIME objects.
% *
% * -TotalLength The total number of octets that follow
% * this value that are part of this FolderIndex.
% *
% * -ObjectIndex An array of MIME object indexes.
% */
#endif
class MIMEFolderIndex
{
	uint32_t				TotalLength;
	MIMEObjectIndex	ObjectIndex<>;
};
