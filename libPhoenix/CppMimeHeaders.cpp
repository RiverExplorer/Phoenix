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
	MimeMessage::GetHeaders(const char * Name) const
	{
		return(_MessageHeaders[Name]);
	}
	
	MimeMessage::Header*
	MimeMessage::GetHeader(uint32_t Index) const
	{
		return(_MessageHeaders[Index]);
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
														(char*)_Parent._EntireMessage.Data + Ptr->_HeaderStart,
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
		_HeaderStart = 0;
		_HeaderLength = 0;
		
		_ValueStart = 0;
		_ValueLength = 0;

		return;
	}

	MimeMessage::Header::Header(MimeMessage  & Parent,
															uint32_t HeaderStart,
															uint32_t HeaderLength,
															uint32_t ValueStart,
															uint32_t ValueLength)
		: _Parent(Parent)
	{
		_HeaderStart = HeaderStart;
		_HeaderLength = HeaderLength;
		_ValueStart = ValueStart;
		_ValueLength = ValueLength;

		return;
	}
	
	MimeMessage::Header::~Header()
	{
		_HeaderStart = 0;
		_HeaderLength = 0;
		
		_ValueStart = 0;
		_ValueLength = 0;

		return;
	}

	uint8_t	*
	MimeMessage::Header::Name(uint32_t & Length) const
	{
		Length = _HeaderLength;
		return(_Parent._EntireMessage.Data + _HeaderStart);
	}
	
	uint8_t	*
	MimeMessage::Header::Value(uint32_t & Length) const
	{
		Length = _ValueLength;
		return(_Parent._EntireMessage.Data + _ValueStart);
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

	MimeMessage::Header *
	MimeMessage::_ParseHeader(const uint8_t * HeaderBeginPtr,
														std::vector<std::string> & DebugMessages)
	{
		Header				*	Results = nullptr;
		
		// We should be at the start of a header.
		//
		uint8_t				* DataPtr = (uint8_t*)HeaderBeginPtr;
		uint32_t				HeaderLength = 0;
		uint8_t				*	ValueStart = 0;
		uint32_t				ValueLength = 0;
		bool						Error = false;
		
		HeaderLength = 1;
		ValueLength = 1;

		if (!_IncDataPointer(DataPtr)) {
			// Not error.
			//

			while (*DataPtr != ':' && !Error) {
				Error = _IncDataPointer(DataPtr);
				if (Error) {
					DebugMessages.push_back(std::string("_IncDataPointer() returned false. (6)"));
					break;
				}
				HeaderLength++;
			}
			DataPtr++;

			if (!Error) {
				// End of header name.
				//
				// -HeaderStart is the offset to start of
				// this header.
				// -HeaderValueLength is set to the
				// lenght of the header.
				//
				// Now get the header value.
				//
				while (isspace(*DataPtr)) {
					if (!_IncDataPointer(DataPtr)) {
					} else {
						Error = true;
						DebugMessages.push_back(std::string("_IncDataPointer() returned error. (7)"));
						break;
					}
				}

				// Now pointing at the start of the header value.
				// It could be a single line,
				// or it could have continutation lines.
				//
				ValueStart = DataPtr;
				bool HaveHeaderValue = false;
				
				do {
					do {
						while(*DataPtr != '\r' && !Error) {
							Error = _IncDataPointer(DataPtr);
							if (Error) {
								DebugMessages.push_back(std::string("_IncDataPointer() returned error. (8)"));
								break;
							}
						}

						// If is end of all headers.
						//
						if (DataPtr[0] == '\r'
								&& DataPtr[1] == '\n'
								&& DataPtr[2] == '\r'
								&& DataPtr[3] == '\n') {

							// DONE.
							//
							HaveHeaderValue = true;

						} else {						
							// If EOL is followed by whitespace,
							// then it is a continuation line.
							//
							if (DataPtr[0] == '\r'
									&& DataPtr[1] == '\n'
									&& isspace(DataPtr[2])) {
								DataPtr +=3;
								while (isspace(*DataPtr)) {
									DataPtr++;
								}
								continue;

							} else {
								HaveHeaderValue = true;
							}
						}
						
						// Keep going if continutation line.
						//
					} while (!HaveHeaderValue);

					if (!Error) {
						// Now pointing at the end of the header value.
						//
						ValueLength = (uint32_t)(DataPtr - ValueStart);

						// Now we are pointing at the start of
						// the next header,
						// Or a continutation line.
						// Or the end of all headers.
						//
						// If it is a '\r', it is the end of the headers.
						//
						// Else if it is whitespace, it is a
						// continutation line.
						//
					} else {
						break;
					}
				} while ((isspace(*DataPtr)	&& *DataPtr != '\r'	&& !Error)
								 || !HaveHeaderValue);
				
				// All headers processed, now
				// use all or just process the body parts.
				//
				if (!Error) {
					Header * NewHeader;
						
					// We have the start of the header.
					// We have the header length.
					// We have the start of the header value.
					// We have the header value length.
					//
					NewHeader = new Header(*this,
																 (uint32_t)(HeaderBeginPtr - _EntireMessage.Data),
																 HeaderLength,
																 (uint32_t)(ValueStart - _EntireMessage.Data),
																 ValueLength);
					Results = NewHeader;
				}
			}
		} else {
			DebugMessages.push_back(std::string("_IncDataPointer() returned false. (11)"));
		}

		return(Results);
	}
}
