
#ifdef RPC_HDR
%/**
% * @note
% * The "C" routines return a bool_t, the C++ API return a bool.
% */
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#include "MetaData.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#include <RiverExplorer/Phoenix/MetaData.hpp>
%#endif
%
%/**
% * A MediaType is a string pair as defined
% * at https://www.iana.org/assignments/media-types/media-types.xhtml
% *
% * Some examples:
% * @verbatim
% *
% * text/html
% * application/pdf
% * audio/ogg
% * image/png
% * multipart/mixed
% *
% * @endverbatim
% *
% * Would have a Key of 'From' and a value of 'RiverExplorer.US@gmail.com'.
% *
% * @note
% * This does not define new or alter existing IANA media types.
% * This is a containter to transport them.
% */
#endif
typedef MetaData MediaType;

#ifdef RPC_HDR
%/**
% * A MIME header is a key/string value.
% */
#endif
typedef MetaData MimeHeader;

#ifdef RPC_HDR
%/**
% * Headers is an array of MimeHeader.
% *
% * Example:
% * @verbatim
% *
% * From: RiverExplorer.US@example.com
% * To: CEO@example.com
% *
% * @endverbatim
% *
% * @note
% * This does not define new or alter existing mime or email headers.
% * This is a containter to transport them.
% */
#endif
typedef MimeHeader MimeHeaders<>;

#ifdef RPC_HDR
%
%/**
% * A MIME object. Has a MediaType, MimeHeaders and data.
% *
% * To use:
% *
% * @code
% *
% * MimeBodyPart	* TheMimeBodyPart;
% *
% * // ...however you fill in or get the MimeBodyPart object data ...
% * //
% * TheMimeBodyPart = GetTheData();
% *
% * uint32_t		Length;
% * const char*	TheMediaType = MimeBodyPart_GetMediaType(TheMimeBodyPart);
% * uint8_t		* Data = MimeBodyPart_GetData(TheMimeBodyPart, &Length);
% * ...
% *
% * // At this point, you have the media-type, Data, and length
% * // of the data.
% *
% * @endcode
% */
#endif

#ifdef RPC_HDR
%/**
%	* - Type: The media-type of the object.
% * @note
% * Media-type is not duplicated in Headers.
% *
%	* - Headers: The MIME headers.
% *
% * - Data: The data itself.
% * The data is a binary blob without any encoding.
% */
#endif
struct MimeBodyPart
{
	MediaType		Type;
	MimeHeaders Headers;
	IoVec				Data;
};
