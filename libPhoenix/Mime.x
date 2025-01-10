
#ifdef RPC_HDR
%/**
% * @note
% * The "C" routines return a bool_t, the C++ API return a bool.
% */
%#ifdef BUILDING_LIBPHOENIX
%#include "KeyString.hpp"
%#else
%#include <RiverExplorer/Phoenix/KeyString.hpp>
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
typedef KeyString MediaType;

#ifdef RPC_HDR
%
%/**
% * A MIME header is a key/string value.
% */
#endif
typedef KeyString MimeHeader;

#ifdef RPC_HDR
%
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
% * Create a new MimeHeaders object.
% *
% * @return A new MimeHeaders object.
% */
%namespace RiverExplorer::Phoenix{
%extern MimeHeaders * MimeHeaders_New();
%}
%
%/**
% * Free a MimeHeaders object and all of its data.
% * This frees all headers and the containers.
% *
% * @param Obj The MimeHeaders object to free.
% */
%namespace RiverExplorer::Phoenix{
%extern void MimeHeaders_Free(MimeHeaders * Obj);
%}
%
%/**
% * Get the number of headers in MimeHeaders.
% *
% * @param Obj The MimeHeaders object to process.
% *
% * @return The number of MimeHeader objects in Obj.
% */
%namespace RiverExplorer::Phoenix{
%extern uint64_t MimeHeaders_Count(MimeHeaders * Obj);
%}
%
%/**
% * Determine if MimeHeaders contains at least one of a specific header.
% *
% * @param Obj The MimeHeaders object to process.
% *
% * @param Search The header to look for.
% *
% * @return The number of Search headers found.
% */
%namespace RiverExplorer::Phoenix{
%extern uint64_t MimeHeaders_Has(MimeHeaders * Obj, const char * Search);
%}
%
%/**
% * Get a specific MimeHeader.
% *
% * @param Obj The MimeHeaders object to process.
% *
% * @param Search The header to look for.
% *
% * @param Count a pointer to a uint64_t variable that will
% * be updated to contain the number of MimeHeader objects in the
% * returned value.
% *
% * @return All of the MimeHeader objects found in MimeHeaders as
% * as an array of Count MimeHeader objects.
% * Returns NULL when none found.
% *
% * @note
% * You must free the results returned from MimeHeaders_Get() when
% * done with the results.
% * You MUST NOT free any of the element values in the returned array.
% * Those are still inside Obj.
% */
%namespace RiverExplorer::Phoenix{
%extern MimeHeader * MimeHeaders_Get(MimeHeaders * Obj, const char * Search, uint64_t * Count);
%}
#endif

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
% * uint64_t		Length;
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

struct MimeBodyPart
{
#ifdef RPC_HDR
%
%/**
%	* The media-type of the object.
% */
#endif
	MediaType	Type;
#ifdef RPC_HDR
%
%/**
%	* The media-type of the object.
% */
#endif
	MimeHeaders Headers;
	
#ifdef RPC_HDR
%
%/**
% * The data itself.
% * The data is a binary blob without any encoding.
% *
% * It does not exclude encoding. This object is orthogonal with
% * respect to encodings.
% */
#endif
	opaque	Data<>;
};

#ifdef RPC_HDR
%
%/**
% * Create a new MimeBodyPart that contains a string of data.
% *
% * @param MediaType The media-type ("text/text", ...)
% * May be set to NULL, you can add later.
% *
% * @note
% * Only very basic checking is done on this value.
% * It must be of the that is defined in RFC-xxxxx as the YYYY.
% * This implementation does not check for the value, just
% * the Type structure.
% *
% * @param Headers The MIME headers for this new MimeBodyPart.
% * May be set to NULL, you can add later.
% *
% * @param Data The data part of the MIME object.
% * May be set to NULL, you can add it later.
% *
% * @note
% * This can also be a pointer to a string. Just set DataLength
% * To the length of the string.
% *
% * @param DataLength The number of Octets in Data.
% * May be set to zero (0), you can add later with data.
% *
% * @see https://www.iana.org/assignments/media-types/media-types.xhtml
% *
% * @return The the new MimeBodyPart.
% * Returns NULL when Type is NULL.
% * Returns NULL when Data is NULL.
% * Returns NULL when Type does not look like a valid IANA media-type.
% *
% */
%namespace RiverExplorer::Phoenix{
%extern MimeBodyPart * MimeBodyPart_New(const char *MediaType, MimeHeaders * Headers, const uint8_t * Data, uint64_t DataLength);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Free a MimeBodyPart and all of its data.
% *
% * @param BodyPart The MimeBodyPart to free.
% */
%namespace RiverExplorer::Phoenix{
%extern void MimeBodyPart_Free(MimeBodyPart * BodyPart);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Get the MIME TYPE of the MimeBodyPart.
% *
% * @param Obj The MimeBodyPart that is being processed.
% *
% * @return The MIME TYPE as a string. (Example: "image/gif")
% * Returns NULL when Obj is NULL.
% * Returns NULL when Obj->MediaType.Key is NULL.
% * Returns NULL when Obj->MediaType.Value is NULL.
% */
%namespace RiverExplorer::Phoenix{
%extern MediaType * MimeBodyPart_GetMediaType(MimeBodyPart * Obj);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Set the MIME TYPE of the MimeBodyPart.
% * This overwrites any existing MediaType in the MimeBodyPart.
% *
% * @param Obj The MimeBodyPart that is being processed.
% *
% * @param Type The new MediaType for this MimeBodyPart.
% */
%namespace RiverExplorer::Phoenix{
%extern void MimeBodyPart_SetMediaType(MimeBodyPart * Obj, MediaType * Type);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Get the MimeHeaders for the MimeBodyPart.
% *
% * @param Obj The MimeBodyPart that is being processed.
% *
% * @return The the MimeHeaders.
% * Returns NULL when Obj is NULL.
% * Returns An empty list, when none have been added.
% */
%namespace RiverExplorer::Phoenix{
%extern MimeHeaders * MimeBodyPart_GetHeaders(MimeBodyPart * Obj);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Set the MimeHeaders for the MimeBodyPart
% * This overwrites any existing MediaType in the MimeBodyPart.
% *
% * @param Obj The MimeBodyPart that is being processed.
% *
% * @param NewHeaders The new MimeHeaders for this MimeBodyPart.
% *
% * @note
% * You can later add, delete, and modify the headers without
% * having to call MimeBodyPart_SetHeaders() again by using the
% * same Headers variable that was provided here.
% */
%namespace RiverExplorer::Phoenix{
%extern void MimeBodyPart_SetHeaders(MimeBodyPart * Obj, MimeHeaders * NewHeaders);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Get the MimeBodyPart data.
% *
% * @param Obj The MimeBodyPart that is being processed.
% *
% * @param Length The address of a uint64_t variable that
% * will be filled in with the value in
% * Obj->MimeData.MimeData_len;
% *
% * @return The data of the MimeBodyPart.
% * -Returns NULL when Obj is NULL.
% * -Returns NULL when Obj->MimeData.MimeData_val is NULL.
% * -Length will be set to zero when
% * Obj->MimeData.MimeData_len is zero (0).
% *
% * @note
% * When MimeBodyPart_GetBinary() returns NULL, Length will
% * not be updated.
% */
%namespace RiverExplorer::Phoenix{
%extern uint8_t * MimeBodyPart_GetData(MimeBodyPart * Obj, uint64_t * Length);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Set the MimeBodyPart data.
% * This overwrites any existing MediaType in the MimeBodyPart.
% *
% * @param Obj The MimeBodyPart that is being processed.
% *
% * @param Data The MimeBodyPart data to add.
% * Setting this to NULL will create an emapy MimeBodyPart and
% * Length will be forced to zero.
% *
% * @param Length The number of octets in Data.
% * Setting this to zero (0) will create an emapy MimeBodyPart and
% * Data will be ignored.
% */
%namespace RiverExplorer::Phoenix{
%extern void MimeBodyPart_SetData(MimeBodyPart * Obj, uint8_t * Data, uint64_t * Length);
%}
#endif
