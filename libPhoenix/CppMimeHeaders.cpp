/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0
 *
 * RiverExplorer is a trademark of RiverExplorer Games LLC
 */

#include <CppMime.hpp>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

namespace RiverExplorer::Phoenix
{

	MimeMessage::Headers::Headers(MimeMessage & Parent)
		: _Parent(Parent)
	{
		/** @todo MimeMessage::Headers::Headers() */
	}

	MimeMessage::Headers::~Headers()
	{
		std::vector<Header*>::iterator	it;
		Header * Data;

		for (it = _Headers.begin(); it != _Headers.end(); it++) {
			Data = *it;
			if (Data != nullptr) {
				delete Data;
			}
		}

		return;
	}

	std::vector<const MimeMessage::Header*>
	MimeMessage::Headers::operator[](const char * Name) const
	{
		std::vector<const Header*> Results;

		if (Name != nullptr) {
			uint32_t	NLength = (uint32_t)strlen(Name);
			std::vector<Header*>::const_iterator It;
			Header * Ptr;

			for (It = _Headers.cbegin(); It != _Headers.cend(); It++) {
				Ptr = *It;
				if (Ptr != nullptr) {
					if (Ptr->_HeaderLength == NLength) {
						if (strncasecmp(Name,
														(char*)&_Parent._EntireMessage.Data[Ptr->_HeaderOffset],
														NLength) == 0) {
							Results.push_back(Ptr);
						}
					}
				}
			}
		}
		
		return(Results);
	}

	void
	MimeMessage::Headers::Add(Header * NewHeader)
	{
		_Headers.push_back(NewHeader);

		return;
	}
	
	MimeMessage::Header::Header(MimeMessage  & Parent)
		: _Parent(Parent)
	{
		_HeaderOffset = 0;
		_HeaderLength = 0;
		
		_ValueOffset = 0;
		_ValueLength = 0;

		return;
	}

	MimeMessage::Header::Header(MimeMessage  & Parent,
															uint32_t HeaderOffset,
															uint32_t HeaderLength,
															uint32_t ValueOffset,
															uint32_t ValueLength)
		: _Parent(Parent)
	{
		_HeaderOffset = HeaderOffset;
		_HeaderLength = HeaderLength;
		_ValueOffset = ValueOffset;
		_ValueLength = ValueLength;

		return;
	}
	
	MimeMessage::Header::~Header()
	{
		_HeaderOffset = 0;
		_HeaderLength = 0;
		
		_ValueOffset = 0;
		_ValueLength = 0;

		return;
	}

	uint32_t
	MimeMessage::Header::Name(uint32_t & Length) const
	{
		Length = _HeaderLength;
		return(_HeaderOffset);
	}

	uint32_t
	MimeMessage::Header::Value(uint32_t & Length) const
	{
		Length = _ValueLength;
		return(_ValueOffset);
	}

	MimeMessage::Header *
	MimeMessage::Headers::operator[](uint32_t Index) const
	{
		Header	* Results = nullptr;

		if (Index < _Headers.size()) {
			Results = _Headers[Index];
		}
		
		return(Results);
	}
}
