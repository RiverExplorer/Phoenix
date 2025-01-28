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
		 * Well known MIME strings defined in RFC-2045
		 */
		static const char * const Boundary_s;
		static const char * const MimeVersion_s;
		static const char * const MimeVersion_Value_1_s;
		static const char * const	ContentDescription_s;
		static const char * const	ContentID_s;
		static const char * const	ContentType_s;
		static const char * const	ContentTransferEncoding_s;

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
						 uint8_t * HeaderStart,
						 uint32_t HeaderLength,
						 uint8_t * ValueStart,
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
			uint8_t	*	_HeaderStart;

			/**
			 * Length of the header.
			 */
			uint32_t	_HeaderLength;
			
			/**
			 * Pointer in the message to the start of the header value.
			 */
			uint8_t	*	_ValueStart;

			/**
			 * Length of the value.
			 */
			uint32_t	_ValueLength;
			
			/**
			 * The parent MIME message this object is part of.
			 */
			MimeMessage & _Parent;
		};

		/**
		 * Parse a file.
		 *
		 * @param FileName The file  to parse.
		 *
		 * @param DebugMessage List of errors for debugging.
		 * When Parse returns true, DebugMessages should be empty.
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
		 * and contains "Mime-Version:1.0"
		 */
		bool	IsMimeVersion_1() const;


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
		 * Get header by name, can return zero to many of them.
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
		 * Get the entire body part.
		 * It will include ALL of the body parts.
		 *
		 * @param Length Length will be set to the length of the result.
		 *
		 * @return A pointer to the entire body of the message.
		 */
		uint8_t	*	GetEntireBody(uint32_t & Length) const;
		
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
			 * Body - Constructor.
			 * Construct from a memory address.
			 *
			 * @param Message A pointer to the start of the message.
			 *
			 * @param MediaTypeOffset The offset into Message
			 * that points to the Content-Type value.
			 *
			 * @param HeaderStartOffset The offset into Message
			 * that points to the start of the body part headers.
			 *
			 * @param DataOffset The offset into Message
			 * that points to the contents.
			 * This is the offset to the first octet in the message
			 * itself.
			 *
			 * @param DataLength The number of octets that are
			 * the message, after DataOffset.
			 */
			BodyPart(uint8_t * Message,
							 uint32_t MediaTypeOffset,
							 uint32_t HeaderStartOffset,
							 uint32_t	DataOffset,
							 uint32_t DataLength);

			/**
			 * Body - Destructor.
			 */
			~BodyPart();

			/**
			 * @return This body parts media type.
			 */
			MediaType	Type() const;

			/**
			 * Set this body parts media type.
			 *
			 * @param Type The media type to set this body part o.
			 */
			void Type(MediaType & Type);
		
			/**
			 * Get this body parts data.
			 *
			 * @param Length Will be set to the number of octets in the results.
			 *
			 * @return The data.
			 * Returns NULL when no data is available or Length is zero (0).
			 */
			uint8_t * Data(uint32_t & Length);

			/**
			 * Set this body parts data.
			 *
			 * @param Blob The body part data.
			 *
			 * @param BlobLength The size of Blob
			 */
			void Data(uint8_t * Blob, uint32_t BlobLength);

			/**
			 * Get this body parts headers.
			 *
			 * @return This body parts headers.
			 * Returns NULL when none set.
			 */
			MimeHeaders			* Headers(uint32_t & Length) const;
		
			/**
			 * Get the number of headers in this body.
			 *
			 * @return The number of headers in this body part.
			 * Return zero (0) when none set.
			 */
			uint32_t			HeaderCount() const;

			/**
			 * Get a header, by index.
			 *
			 * @param Index Which header to get.
			 *
			 * @return The Index'th Header object.
			 * Returns NULL when none at this index.
			 */
			MimeHeader	*	GetHeader(uint32_t Index);

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
			uint8_t	*	_BodyStart;

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
													std::vector<std::string> & DebugMessages);

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
												std::vector<std::string>    &DebugMessages);
		

		/**
		 * May be nullptr, the preable.
		 * This is data in a MIME object, after the headers
		 * and before the MIME body parts.
		 */
		uint8_t	*	_PreambleStart;

		/**
		 * Length of _PreambleStart.
		 */
		uint32_t	_PreambleLength;
			
		/**
		 * May be nullptr, the epilogue.
		 * This is data in a MIME object, after the last body part
		 * and before the end of the message.
		 */
		uint8_t	*	_EpilogueStart;

		/**
		 * Length of _EpilogueStart.
		 */
		uint32_t	_EpilogueLength;
	};
}
#endif // _RIVEREXPLORER_MIME_CPPMIME_HPP_
