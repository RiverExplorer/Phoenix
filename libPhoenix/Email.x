%/**
% * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
% * RiverExplorer is a trademark of RiverExplorer Games LLC                      
% */
%

#ifdef RPC_HDR
%/**
% * @note
% * The "C" routines return a bool_t, the C++ API return a bool.
% */
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#include "KeyString.hpp"
%#include "Mime.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#include <RiverExplorer/Phoenix/KeyString.hpp>
%#include <RiverExplorer/Phoenix/Mime.hpp>
%#endif
%#include <string>
%#include <vector>

#endif

#ifdef RPC_HDR
%/**
% * EMail headers are a vector of KeyString.
% *
% * Some examples:
% * @verbatim
% *
% * From: RiverExplorer.USexample.com
% * To: CEO@example.com
% * Subject: The holidays!
% *
% * @endverbatim
% *
% * @note
% * This does not define new or alter existing header types.
% * This is a container to transport them.
% */
#endif
typedef KeyString EMailHeader;
typedef EMailHeader EMailHeaders<>;

#ifdef RPC_HDR
%
%/**
% * An email is a vector of MimeBodyPart
% *
% * @note
% * This does not define new or alter existing mime or email headers.
% * This is a container to transport them.
% */
#endif
typedef MimeBodyPart EMailBodyParts<>;

#ifdef RPC_HDR
%
%/**
% * Check for the existence of a specific Header.
% *
% * @param Headers The Headers that is being processed.
% *
% * @param Key The header name being looked for.
% *
% * @return The number of matches.
% * Returns zero (0) when none are found.
% * Returns ((uint64_t)-1) when Obj or Key are NULL.
% */
%namespace RiverExplorer::Phoenix{
%extern uint64_t * EMail_HasHeader(EMailHeaders * Headers, const char * Key);
%}
#endif
 
#ifdef RPC_HDR
%
%/**
% * Get a specific Header.
% *
% * @param Headers The Headers that is being processed.
% *
% * @param Key The header name being looked for.
% *
% * @return A vector of the matches.
% * Will return zero (0) or more matches, in the original order.
% * Will return NULL of Obj or Key are NULL.
% * Will return NULL when Key is not found.
% */
%namespace RiverExplorer::Phoenix{
%extern EMailHeader  * EMail_GetHeader(EMailHeaders * Headers, const char * Key);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Headers count.
% *
% * @param Headers The Headers that is being processed.
% *
% * @return The number of EMailHeader in Headers.
% * Returns ((uint64_t)-1) when Headers is NULL.
% */
%namespace RiverExplorer::Phoenix{
%extern uint64_t EMail_GetHeaderCount(EMailHeaders * Headers);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Headers iterator.
% *
% * @param Headers The Headers that is being processed.
% *
% * @param Which The header to get, the first is zero (0).
% *
% * @return A pointer to the EMailHeader.
% * Returns NULL when Headers or Key is NULL.
% * Returns NULL when Which is not a valid index.
% */
%namespace RiverExplorer::Phoenix{
%extern EMailHeader * EMail_GetHeaderByIndex(EMailHeaders * Headers, uint64_t Which);
%}
#endif

#ifdef RPC_HDR
%
%/**
% * Common headers names.
% */
%namespace RiverExplorer::Phoenix{
%extern const char * Date_s;			/** "Date" */
%extern const char * From_s;			/** "From" */
%extern const char * Subject_s;		/** "Subject" */
%extern const char * To_s;				/** "To" */
%}
#endif

#ifdef RPC_HDR
#ifdef __cplusplus
} // End extern "C"
%
#endif
%#ifdef __cplusplus
%namespace RiverExplorer::Phoenix::EMail {
%class Message
%{
% public:
%
%	/**
%	 * Message - Default Destructor.
%	 */
%	Message();
%
%	/**
%  * Message - Destructor.
%  */
%
%	/**
%  * Get the headers.
%	 *
%	 * @return All of the headers.
%  */
%	EMailHeaders * Headers() const;	
%
% /**
%  * Add a header.
%  *
%  * @param NewHeader The new header to add.
%  */
%  void Add(EMailHeader & NewHeader);
%		
% /**
%  * Add a header.
%  *
%  * @param Key The new header to add.
%  *
%  * @param Value The new header value to add.
%  */
%  void Add(std::string Key, std::string Value);
%
% /**
%  * Get the named header.
%  *
%  * @param Key The new header to add.
%  *
%	 * @return A vector of all the matches. It will contain
%	 * zero or more entries.
%  *
%  * @note
%  * You MUST delete result when finished or you will have
%	 * a memory leak. You MUST NOT delete the
%	 * entries in the vector, they are still in the email message.
%  */
%  std::vector<const EMailHeader*>	* Header(std::string Key);
%
% /**
%  * Get the number of body parts.
%  *
%  * @return The number of body parts.
%	 */
% const uint64_t Count() const;
%
% /**
%  * Get the body parts.
%  *
%  * @return The body parts.
%  *
%  * @note
%  * Do not delete the results.
%	 */
% const MimeBodyPart * Body() const;
%
% /**
%  * Get the nTh body part.
%  *
%  * @param nTh Which to get, first is zero (0).
%  *
%  * @return The body part.
%  * Returns nullptr when nTh does not exist.
%  *
%  * @note
%  * Do not delete the results.
%	 */
% const MimeBodyPart * Body(uint64_t nTh) const;
%
% /**
%	 * This ID is used as a transport ID between
%	 * endpoints to uniquely identify this message for this
%	 * account on this server.
%	 *
%	 * @return The message ID For this message.
%	 * A return value of zero (0) indicates that this message
%  * does not have an ID.
%	 *
%	 * @note
%	 * This is NOT the 'Message-ID' in email headers.
%	 * This is the ID that uniquely identifies this message to endpoints.
%	 *
%	 * @note
%	 * This ID is unique to the server or message provider and might
%	 * not be unique on the client side.
%	 * Implementations may wish to provide their own local ID
%	 * to uniquely identify this message in their system that is
%	 * separate from this ID.
%	 */
% uint64_t ID() const;
%
% /**
%  * Messages in a message store have an ID.
%  * This sets the message ID for this message.
%	 *
%	 * @param TransportID The transport ID to associate with this message.
%	 * A value of zero (0) indicates that this message
%  * does not have an ID.
%	 *
%	 * @note
%	 * This is NOT the 'Message-ID' in email headers.
%	 * This is the ID that uniquely identifies this message to endpoints.
%	 *
%	 * @note
%	 * This ID is unique to the server or message provider and might
%	 * not be unique on the client side.
%	 * Implementations may wish to provide their own local ID
%	 * to uniquely identify this message in their system that is
%	 * separate from this ID.
%	 */
% void ID(uint64_t StoreID) const;
%};
%} // End namespace EMail
%#endif // class
#endif // RPC_HDR
