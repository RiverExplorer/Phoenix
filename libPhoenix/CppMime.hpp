
#ifndef _RIVEREXPLORER_MIME_CPPMIME_HPP_
#define _RIVEREXPLORER_MIME_CPPMIME_HPP_

#include <Mime.h>
#include <string>

namespace RiverExplorer::Phoenix
{
	class BodyPart
		: private MimeBodyPart
	{
	public:

		/**
		 * BodyPart - Default Constructor.
		 */
		BodyPart();

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
						 uint64_t NewLength);

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
		uint8_t * Data(uint64_t & Length);

		/**
		 * Set this body parts data.
		 *
		 * @param Blob The body part data.
		 *
		 * @param BlobLength The size of Blob
		 */
		void Data(uint8_t * Blob, uint64_t BlobLength);

		/**
		 * Get this body parts headers.
		 *
		 * @return This body parts headers.
		 * Returns NULL when none set.
		 */
		MimeHeaders			* Headers(uint64_t & Length) const;
		
		/**
		 * Get the number of headers in this body.
		 *
		 * @return The number of headers in this body part.
	   * Return zero (0) when none set.
		 */
		uint64_t			HeaderCount() const;

		/**
		 * Get a header, by index.
		 *
		 * @param Index Which header to get.
		 *
		 * @return The Index'th Header object.
		 * Returns NULL when none at this index.
		 */
		MimeHeader	*	GetHeader(uint64_t Index);

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
	};
}
#endif // _RIVEREXPLORER_MIME_CPPMIME_HPP_
