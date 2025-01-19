/**                                                                             
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed un\
der CC BY 4.0                                                                   
 * RiverExplorer is a trademark of RiverExplorer Games LLC                      
 */


#include <CppMime.hpp>

namespace RiverExplorer::Phoenix
{
	BodyPart::BodyPart()
	{
		/*EMPTY*/

		return;
	}

	BodyPart::BodyPart(MediaType & NewType,
										 MimeHeaders & NewHeaders,
										 uint8_t * NewData,
										 uint64_t NewLength)
	{
	}

	BodyPart::~BodyPart()
	{
	}

	MediaType
	BodyPart::Type() const
	{
	}

	void
	BodyPart::Type(MediaType & Type)
	{
	}

	uint8_t *
	BodyPart::Data(uint64_t & Length)
	{
	}

	void
	BodyPart::Data(uint8_t * Blob, uint64_t BlobLength)
	{
	}

	MimeHeaders	*
	BodyPart::Headers(uint64_t & Length) const
	{
	}
		
	uint64_t
	BodyPart::HeaderCount() const
	{
	}

	MimeHeader	*
	BodyPart::GetHeader(uint64_t Index)
	{
	}

	void
	BodyPart::AddHeader(MimeHeader & NewHeader)
	{
	}
		
	void
	BodyPart::AddHeader(std::string Key, std::string Value)
	{
	}

	bool
	BodyPart::ToXDR(XDR & xdrs)
	{
	}

	bool
	BodyPart::FromXDR(XDR & xdrs)
	{
	}

}
