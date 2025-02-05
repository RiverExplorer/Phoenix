/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0
 *
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#ifndef _RIVEREXPLORER_MIME_CPPMIME_HPP_
#define _RIVEREXPLORER_MIME_CPPMIME_HPP_

#ifdef BUILDING_LIBPHOENIX
#include "MetaData.hpp"
#include "Mime.hpp"
#include "Iov.hpp"
#else
#include <RiverExplorer/Phoenix/MetaData.hpp>
#include <RiverExplorer/Phoenix/Mime.hpp>
#include <RiverExplorer/Phoenix/Iov.hpp>
#endif

#include <string>
#include <vector>

namespace RiverExplorer::Phoenix
{

	/**
	 * @class MimeMessage CppMime.hpp <RiverExplorer/Phoenix/CppMime.hpp>
	 * A RFC-2045 MIME message.
	 *
	 * In this implementation, messages are stored in physical space.
	 * On a disk drive.
	 *
	 * So many of the objects point to files and offsets into files
	 * for the data.
	 *
	 * When transmitted over the wire, it will arrive unchanged.
	 */
	class MimeMessage
	{
	public:

		/**
		 * The body part ID is the offset into the message where
		 * the headers part of the body part starts.
		 */
		typedef unint32_t	BodyPartID_t;
		
		/**
		 * Well known MIME strings defined in RFC-2045
		 */

		/**
		 * Contains the string "boundary".
		 */
		static const char * const Boundary_s;

		/**
		 * Contains the string "MIME-Version".
		 */
		static const char * const MimeVersion_s;

		/**
		 * Contains the string "1.0".
		 */
		static const char * const MimeVersion_Value_1_s;

		/**
		 * Contains the string "Content-Description".
		 */
		static const char * const	ContentDescription_s;

		/**
		 * Contains the string "Content-ID".
		 */
		static const char * const	ContentID_s;

		/**
		 * Contains the string "Content-Type".
		 */
		static const char * const	ContentType_s;

		/**
		 * Contains the string "Content-Transfer-Encoding".
		 */
		static const char * const	ContentTransferEncoding_s;

		/**
		 * Contains the string "Multipart/mixed".
		 */
		static const char * const MultipartMixed_s;
		
		/**
		 * Contains the string "Multipart/alternative".
		 */
		static const char * const MultipartAlternative_s;
		 
		/**
		 * Contains the string "Multipart/digest".
		 */
		static const char * const MultipartDigest_s;
		 
		/**
		 * Contains the string "Multipart/parallel".
		 */
		static const char * const MultipartParallel_s;
		 
		/**
		 * MIME - Default Constructor.
		 */
		MimeMessage();

		/**
		 * MIME - Destructor.
		 */
		~MimeMessage();
		

		// Forward ref
		//
		class Header;
		
		/**
		 * @class Headers CppMime.hpp <RiverExplorer/Phoenix/CppMime.hpp>
		 * A set of headers.
		 */
		class Headers
		{
		public:
			
			/**
			 * Headers - Default Constructor.
			 *
			 * @param Mime The parent MIME message.
			 * It is a reference to the MIME message this header belongs to.
			 */
			Headers(MimeMessage & Mime);

			/**
			 * Headers - Destructor.
			 */
			~Headers();

			/**
			 * Get header by name, can return zero to many of them.
			 * They will be in the order they are in the MIME object.
			 *
			 * @param Name The name of the header to get
			 * The header name comparison is case insensitive.
			 *
			 * @return A possibly empty list of matches, in the order
			 * they appear in this object.
			 */
			std::vector<const Header*> operator[](const char * Name) const;

			/**
			 * Get header by position.
			 * They will be in the order they are in the MIME object.
			 *
			 * @param Index The position of the header.
			 * The first one has an index value of zero (0).
			 *
			 * @return The Header. Or nullptr if Index is out of range.
			 */
			Header	* operator[](uint32_t range) const;

			/**
			 *  Add a header.
			 *
			 * @param NewHeader The header.
			 */
			void Add(Header * NewHeader);
			
		private:

			/**
			 * An ordered list of headers.
			 */
			std::vector<Header*>	_Headers;

			/**
			 * The parent MIME message this object is part of.
			 */
			MimeMessage & _Parent;
			
		}; // End class Headers.

		/**
		 * @class Header CppMime.hpp <RiverExplorer/Phoenix/CppMime.hpp>
		 * Represents one header.
		 */
		class Header
		{
		public:

			/**
			 * Header - Default Constructor.
			 * Sets HeaderOffset and ValueOffset to zero.
			 *
			 * @param Mime The parent MIME message.
			 * It is a reference to the MIME message this header belongs to.
			 */
			Header(MimeMessage & Mime);

			/**
			 * Header - Constructor.
			 *
			 * @param Mime The parent MIME message.
			 * It is a reference to the MIME message this header belongs to.
			 *
			 * @param HeaderStart The pointer to the start of the header.
			 *
			 * @param HeaderLength The length of the header.
			 * Does not include the ':'.
			 *
			 * @param ValueStart The pointer to the start of the header value.
			 *
			 * @param ValueLength The lenght of the header value.
			 * Does not incldue the terminating "\r\n".
			 */
			Header(MimeMessage & Mime,
						 uint32_t HeaderStart,
						 uint32_t HeaderLength,
						 uint32_t ValueStart,
						 uint32_t ValueLength);

			/**
			 * Header - Destructor.
			 */
			~Header();

			/**
			 * Get the header name offset and length.
			 *
			 * @param Len The length of the header name.
			 * Len will be set to the header name length.
			 *
			 * @return Return a pointer to the start of the name.
			 */
			uint8_t	*	Name(uint32_t & Len) const;
			
			/**
			 * Get the header value offset and length.
			 *
			 * @param Len The length of the value.
			 * Len will be set to the value length.
			 *
			 * @return Return a pointer to the start of the value.
			 */
			uint8_t	*	Value(uint32_t & Len) const;
			
		private:

			// Forward ref.
			//
			class Headers;
			friend RiverExplorer::Phoenix::MimeMessage::Headers;
			friend RiverExplorer::Phoenix::MimeMessage;

			/**
			 * Pointer in the message to the start of the header.
			 */
			uint32_t	_HeaderStart;

			/**
			 * Length of the header.
			 */
			uint32_t	_HeaderLength;
			
			/**
			 * Pointer in the message to the start of the header value.
			 */
			uint32_t	_ValueStart;

			/**
			 * Length of the value.
			 */
			uint32_t	_ValueLength;
			
			/**
			 * The parent MIME message this object is part of.
			 */
			MimeMessage & _Parent;
		}; // End class Header.

		/**
		 * @class BodyPart CppMime.hpp <RiverExplorer/Phoenix/CppMime.hpp>
		 * This is a C++ wrapper for the XDR/RPCGEN MimeBodyPart object.
		 */
		class BodyPart
			: private MimeBodyPart
		{
		public:

			/**
			 * BodyPart - Default Constructor.
			 *
			 * @param Parent The parent object.
			 */
			BodyPart(MimeMessage & Parent);

			/**
			 * Body - Constructor.
			 *
			 * @param NewType The Media Type.
			 *
			 * @param NewHeaders The MIME headers.
			 *
			 * @param NewData The body part data.
			 *
			 * @param NewLength The body part data length.
			 */
			BodyPart(MediaType & NewType,
							 MimeHeaders & NewHeaders,
							 uint8_t * NewData,
							 uint32_t NewLength);

			/**
			 * Body - Destructor.
			 */
			~BodyPart();

			/**
			 * @return This body parts media type. (Content-Type)
			 */
			MediaType	Type() const;

			/**
			 * Set this body parts media type. (Content-Type)
			 *
			 * @param Type The media type to set this body part o.
			 */
			void Type(MediaType & Type);
		
			/**
			 * Get this body part area data. (Does not the headers.)
			 *
			 * @param Length Will be set to the number of octets in the results.
			 * This is the body area, not including the headers in
			 * the body part.
			 *
			 * @return The body area part data.
			 * Returns nullptr when no data is available or Length is zero (0).
			 */
			uint8_t * BodyData(uint32_t & Length);

			/**
			 * Set this body part area data.
			 * This does not inlude the headers to the body part.
			 *
			 * @param Blob The body part data.
			 *
			 * @param BlobLength The size of Blob
			 */
			void BodyData(uint8_t * Blob, uint32_t BlobLength);

			/**
			 * Get this body parts headers.
			 *
			 * @return This body parts headers.
			 * Returns NULL when none set.
			 */
			const MimeHeaders			* Headers(uint32_t & Length) const;
		
			/**
			 * Get the number of headers in this body part.
			 *
			 * @return The number of headers in this body part.
			 * Returns zero (0) when none set.
			 */
			uint32_t			HeaderCount() const;

			/**
			 * Get a header, by index.
			 *
			 * @param Index Which header to get.
			 *
			 * @return The Index'th Header object.
			 * Returns nullptr when none at this index.
			 */
			const MimeHeader	*	GetHeader(uint32_t Index);

			/**
			 * Add a new header to this body part.
			 *
			 * @param NewHeader The new header to add.
			 */
			void					AddHeader(MimeHeader & NewHeader);
		
			/**
			 * Add a new header to this body part.
			 *
			 * @param Key The new header to add.
			 *
			 * @param Value The new header value to add.
			 */
			void					AddHeader(std::string Key, std::string Value);

			/**
			 * XDR encode this object.
			 *
			 * @param xdrs The already initialized XDR object.
			 */
			bool					ToXDR(XDR & xdrs);

			/**
			 * XDR decode this object from an XDR stream.
			 *
			 * @param xdrs The already initialized XDR object.
			 */
			bool					FromXDR(XDR & xdrs);

		private:

			friend RiverExplorer::Phoenix::MimeMessage::Headers;
			friend RiverExplorer::Phoenix::MimeMessage;

			/**
			 * Body parts can have body parts.
			 */
			std::vector<BodyPart*>	_BodyParts;

			/**
			 * The parent Object.
			 */
			MimeMessage & _Parent;

			/**
			 * The start of this body part in _Parent.
			 */
			uint32_t	_BodyStart;

			/**
			 * The start of this body area part in _Parent.
			 * May be the same as _BodyStart, when this
			 * body part has no headers.
			 *
			 * @note
			 * The body area length is: (_BodyLength - _BodyAreaStart)
			 */
			uint32_t	_BodyAreaStart;

			/**
			 * The length of _BodyStart.
			 */
			uint32_t	_BodyLength;

			/**
			 * A MIME body part has headers.
			 * An 822 body part, does not.
			 */
			std::vector<Header*>	_Headers;
			
		}; // End class BodyPart
			
		/**
		 * Parse an RFC-822 or MIME file.
		 *
		 * @param FileName The file  to parse.
		 *
		 * @param[out] DebugMessage List of errors for debugging.
		 * When Parse returns true, DebugMessages should be empty.
		 * These messages are not designed to be seen by the end user.
		 * They are implementation specific messages for logging,
		 * or administrative control.
		 *
		 * @return true if the file coulde be parsed.
		 */
		bool	Parse(const char * FileName,
								std::vector<std::string> & DebugMessages);
			
		/**
		 * See if this object is a MIME 1.0 message,
		 * or a pre-MIME message.
		 *
		 * @return true If this message complies with MIME (RFC-2045)
		 * and contains "Mime-Version: 1.0".
		 */
		bool	IsMime() const;

		/**
		 * Get a pointer to the entire message.
		 *
		 * @param Len The length of the value.
		 * Len will be set to the value length.
		 *
		 * @return A pointer to the entire message.
		 */
		uint8_t  *	Message(uint32_t & Len) const;

		/**
		 * Get header by position.
		 * They will be in the order they are in the MIME object.
		 *
		 * @param Index The position of the header.
		 * The first one has an index value of zero (0).
		 *
		 * @return The Header. Or nullptr if Index is out of range.
		 */
		Header	* GetHeader(uint32_t Index) const;

		/**
		 * Get a header from a body part, by position from a body part ID and index.
		 * They will be in the order they are in the body part object.
		 *
		 * @param FromID The ID of the body part.
		 * FromID is a value returned from BodyPart() or BodyPartPart().
		 *
		 * @param Index The position of the header.
		 * The first one has an index value of zero (0).
		 *
		 * @return The Header. Or nullptr if Index is out of range.
		 *
		 * @note
		 * Not all body parts have headers.
		 *
		 * @see BodyPart()
		 * @see BodyPartPart()
		 */
		Header	* GetHeader(BodyPartID_t FromID, uint32_t Index) const;

		/**
		 * Get all MIME message headers by name, can return zero to many of them.
		 * They will be in the order they are in the MIME object.
		 *
		 * @param Name The name of the header to get
		 * The header name comparison is case insensitive.
		 *
		 * @return A possibly empty list of matches, in the order
		 * they appear in this object.
		 */
		std::vector<const Header*> GetHeaders(const char * Name) const;

		/**
		 * Get all body part headers by name, can return zero to many of them.
		 * They will be in the order they are in the MIME object.
		 *
		 * @param FromID The ID of the body part.
		 * FromID is a value returned from BodyPart() or BodyPartPart().
		 *
		 * @param Name The name of the header to get
		 * The header name comparison is case insensitive.
		 *
		 * @return A possibly empty list of matches, in the order
		 * they appear in this object.
		 *
		 * @see BodyPart()
		 * @see BodyPartPart()
		 */
		std::vector<const Header*> GetHeaders(BodyPartID_t FromID,
																					const char * Name) const;

		/**
		 * Get the entire body part.
		 * It will include ALL of the body part.
		 *
		 * @param FromID The ID of the body part.
		 * FromID is a value returned from BodyPart() or BodyPartPart().
		 *
		 * @param Length Length will be set to the length of the result.
		 *
		 * @return A pointer to the entire body of the message.
		 * Returns a pointer to the start of the body part header area.
		 */
		uint8_t	*	GetEntireBody(BodyPartID_t FromID, uint32_t & Length) const;

		/**
		 * Get a bodypart.
		 *
		 * @param FromID The ID of the body part.
		 * FromID is a value returned from BodyPart() or BodyPartPart().
		 *
		 * @return A BodyPart object, or nullptr when FromID is not valid.
		 *
		 * @see BodyPart()
		 * @see BodyPartPart()
		 */
		BodyPart	*	GetBodyPart(FromID);
		
		/**
		 * Get the index to any MIME preamble in this message.
		 *
		 * When !IsMime(), returns zero (0) as non-MIME messages
		 * do not have preamble.
		 *
		 * @note
		 * MIME messages may or might not have a preamble.
		 *
		 * @return
		 * IsMime() == true: The offset into the entire message where the
		 * data part of the MIME preamble starts.
		 * Returns zero (0) when there is no preamble in this MIME message.
		 *
		 * !IsMime() returns zero (0), as non-MIME messages do not
		 * have a preamble.
		 */
		uint32_t	GetPreambleIndex() const;

		/**
		 * Get the index to any MIME epilogue in this message.
		 *
		 * When !IsMime(), returns zero (0) as non-MIME messages
		 * do not have preamble.
		 *
		 * @note
		 * MIME messages may or might not have a preamble.
		 *
		 * @return
		 * IsMime() == true: The offset into the entire message where the
		 * data part of the MIME preamble starts.
		 * Returns zero (0) when there is no preamble in this MIME message.
		 *
		 * !IsMime() returns zero (0), as non-MIME messages do not
		 * have a preamble.
		 */
		uint32_t	GetEpilogueIndex() const;

		/**
		 * Get the offset into the entire message for the requested
		 * body part, also gets the content type for the part.
		 *
		 * This gets the top level body part index information.
		 * @see BodyPartPart() for getting any embedded body parts.
		 *
		 * @param Which Which top level body part to get. The first is zero (0).
		 *
		 * @param[out] ContentTypeValue The content type of the body part.
		 * When no ContentType header was provided, the will be set
		 * to an empty string.
		 *
		 * @return The unique ID for this body part in this message.
		 * This also is the offset from the start of the entire message,
		 * where this body part starts. 
		 *
		 * Returns zero (0) when the Which parameter is not a valid
		 * for this message.
		 *
		 * @note
		 * The returned value is valid before and after
		 * the client fetches the message.
		 * After getting this index, the client could fetch the entire
		 * message, or just the body parts it desires.
		 */
		BodyPartID_t	BodyPart(uint32_t Which, std::string & ContentTypeValue);

		/**
		 * Get the offset into the a specific body part
		 * for its embedded body part, also gets the content type for the part.
		 *
		 * This gets the body part index information for embedded body parts.
		 * @see BodyPart() for getting any top level body parts.
		 *
		 * @param FromWhich Which body part to get.
		 * This must be a value returned from BodyPart() or BodyPartPart().
		 *
		 * @param Which Which top level body part to get.
		 *
		 * @param[out] ContentTypeValue The content type of the body part.
		 * When no ContentType header was provided, the will be set
		 * to an empty string.
		 *
		 * @return The unique ID for this body part in this message.
		 * This also is the offset from the start of the entire message,
		 * where this body part starts. 
		 *
		 * Returns zero (0) when the Which parameter is not a valid
		 * for this message.
		 *
		 * @note
		 * The returned value is valid before and after
		 * the client fetches the message.
		 * After getting this index, the client could fetch the entire
		 * message, or just the body parts it desires.
		 */
		BodyPartID_t	BodyPartPart(uint32_t FromWhich,
															 uint32_t Which,
															 std::string & ContentTypeValue);

		/**
		 * In order to reduce the memory footprint, this implementation
		 * tries not to copy strings.
		 *
		 * A MimeString is an offset into the message where
		 * the string starts, with a length.
		 */
		struct MimeString
		{
			/**
			 * The offset into the MIME object where the string starts.
			 */
			uint32_t	Start;

			/**
			 * The length of the string.
			 */
			uint32_t	Length;
		};
		
		/**
		 * When the client wants to find all of the top level
		 * content-type values for all of the MIME message body parts.
		 *
		 * Get a list of all of the content-type values, for all of the
		 * body parts.
		 *
		 * @note
		 * Body parts may contain body parts.
		 * This method gets all of the content-type values for the
		 * top level MIME object.
		 * To get any possible content-type values within a specific
		 * body part, call GetContentType(BodyPartID_t).
		 *
		 * @return A list of MimeString, to all of the content-type values
		 * in all of the MIME message body parts. Ordered
		 * by the order in the message.
		 *
		 * @see GetContentTypes(BodyPartID_t) to get them
		 * all of the ones in a body part, that contains body parts.
		 *
		 * @note
		 * Each MIME message has a Content-Type list.
		 * And each body part has a Content-Type list.
		 * They may be empty.
		 */
		const std::vector<MimeString*>	&	GetContentTypes() const;
			
		/**
		 * When the client wants to find all of the content-type values
		 * within a body part.
		 *
		 * Get a list of all of the content-type values, for all of the
		 * body parts within a body part.
		 *
		 * @param FromWhich Which body part to get.
		 * This must be a value returned from BodyPart() or BodyPartPart().
		 *
		 * @note
		 * Body parts may contain body parts.
		 * This method gets all of the content-type values for a
		 * specific body part.
		 *
		 * @return A list of all of the body part content-type values.
		 * It is an ordered list, as they are found in the message.
		 */
		const std::vector<MimeString &>	& GetContentTypes(uint32_t FromWhich) const;
		
	private:

		/**
		 * All headers in the order they appear.
		 */
		Headers _MessageHeaders;

		/** 
		 * Data about the current message.
		 */
		IoVec		_EntireMessage;

		/**
		 * The entire body part.
		 */
		IoVec		_EntireBody;

		/**
		 * And Body parts can have body parts.
		 */
		std::vector<BodyPart*>	_BodyParts;
		
		/**
		 * Increment the pointer, except when it goes past
		 * the end of the message.
		 *
		 * @param Ptr The pointer to increment.
		 *
		 * @return true on error.
		 */
		bool _IncDataPointer(uint8_t *& Ptr) const;

		/**
		 * Parse a single header.
		 *
		 * @param HeaderStart A pointer to the start of the header.
		 *
		 * @param DebugMessage List of errors for debugging.
		 * When Parse returns true, DebugMessages should be empty.
		 *
		 * @return The new header, or nullptr on error.
		 */
		Header * _ParseHeader(const uint8_t * HeaderStart,
													std::vector<std::string> & DebugMessage);

		/**
		 * Parse a MIME Body.
		 * It can be recursive.
		 *
		 * @param BodyText A pointer to the text (or binary) of the body.
		 *
		 * @param BoundryLine The MIME body part boundry line.
		 *
		 * @return The length of the segment parsed. Or zero (0) if it
		 * has nothing, or when it was not a MIME body part.
		 */
		uint32_t _ParseBody(const uint8_t * BodyText,
												std::string BoundaryLine,
												std::vector<std::string> & DebugMessages);
		
		/**
		 * May be nullptr, the preable.
		 * This is data in a MIME object, after the headers
		 * and before the MIME body parts.
		 */
		uint32_t	_PreambleStart;

		/**
		 * Length of _PreambleStart.
		 */
		uint32_t	_PreambleLength;
			
		/**
		 * May be nullptr, the epilogue.
		 * This is data in a MIME object, after the last body part
		 * and before the end of the message.
		 */
		uint32_t	_EpilogueStart;

		/**
		 * Length of _EpilogueStart.
		 */
		uint32_t	_EpilogueLength;
	};
}
#endif // _RIVEREXPLORER_MIME_CPPMIME_HPP_
