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

	MimeMessage::BodyPart::BodyPart(MimeMessage & Parent)
		: _Parent(Parent)
	{
		_BodyStart = 0;
		_BodyLength = 0;

		return;
	}

	MimeMessage::BodyPart::~BodyPart()
	{
		std::vector<Header*>::iterator HIt;
		Header * HPtr;
		
		for (HIt = _Headers.begin(); HIt != _Headers.end(); HIt++) {
			HPtr = *HIt;
			if (HPtr != nullptr) {
				delete HPtr;
				*HIt = nullptr;
			}
		}

		std::vector<BodyPart*>::iterator BIt;
		BodyPart * BPtr;

		for (BIt = _BodyParts.begin(); BIt != _BodyParts.end(); BIt++) {
			BPtr = *BIt;
			if (BPtr != nullptr) {
				delete BPtr;
				*BIt = nullptr;
			}
		}

		_BodyStart = 0;
		_BodyLength = 0;

		return;
	}

	uint8_t	*
	MimeMessage::GetEntireMessage(uint32_t & Length) const
	{
		Length = _EntireBody.Length;
		return(_EntireBody.Data);
	}
	
	uint32_t
	MimeMessage::_ParseBody(const uint8_t * EntireBodyStart,
													std::string Boundary,
													std::vector<std::string> & DebugMessages)
	{
		uint32_t					Results = 0;

		MimeMessage::BodyPart		*	NewBodyPart = nullptr;
		
		bool					Error = false;
		uint8_t			*	DataPtr = (uint8_t*)EntireBodyStart;
		bool					HeadersDone = false;
		bool					HasPreamble = false;
		bool					HasEpilogue = false;
		
		// For boundary of ABC.
		// Start is "--ABC"
		// Middle ones are "--ABC"
		// Last is "--ABC--"
		//
		std::string		BodyStart = "--";
		std::string		BodyEnd;

		BodyStart += Boundary;
		BodyEnd = BodyStart;
		BodyEnd += "--";

		const char * StartAsString = BodyStart.c_str();
		const size_t	StartLength = strlen(StartAsString);

		const char * LastAsString = BodyEnd.c_str();
		const size_t	EndLength = strlen(LastAsString);
		
		// The first part of the body, may be just an ordanary
		// 822 text.
		//
		// Or, the first line of BodyStart, should be Boundry.
		//
		uint8_t * PartStart = (uint8_t*)strstr((char*)EntireBodyStart, StartAsString);

		// Find the next one, it is the end of this one,
		// and the start of the next one.
		//
		// First look for the next one (when more than one).
		//
		uint8_t	*	PartEnd = (uint8_t*)strstr((char*)EntireBodyStart + StartLength, 
																				 StartAsString);

		// If did not find it, look for last one.
		//
		if (PartEnd == nullptr) {
			// Must be the last one?
			//
			PartEnd = (uint8_t*)strstr((char*)EntireBodyStart + StartLength, 
																 LastAsString);

			if (PartEnd == nullptr) {
				// ERROR
			}
		}

		// Is there more than whitespace after the last header value
		// and before the first boundary?
		// If yes, it has a preamble.
		// If not, just skip the white space.
		//
		const uint8_t * Ptr = EntireBodyStart;

		while (isspace(*Ptr++));

		if (Ptr != PartStart) {
			HasPreamble = true;
			// This is called the preable, and RFC-1341 says
			// it should be ignored, we keep it.
			// The User Agent can ignore it.
			//
			_PreambleStart = (uint32_t)(EntireBodyStart - _EntireMessage.Data);
			_PreambleLength = (uint32_t)(PartStart - EntireBodyStart);

			DataPtr = PartStart;
		}
		
		// Now look for MIME body parts.
		//
		do {
			// Just after PartStart is the start of this body.
			// Just before PartEnd is the end of this body part.
			//
			// We are parsing MIME body parts, they often have headers.
			// And they start right after the boundary.
			//
			uint8_t *	HeaderStart = DataPtr;
			
			// We should be pointing at the start of boundary.
			//
			if (strncmp(StartAsString, (char*)DataPtr, StartLength)) {
				HeaderStart += (StartLength + 2); // \r\n

				// We should be pointing at the start of any header.
				//
				NewBodyPart = new BodyPart(*this);
				_BodyParts.push_back(NewBodyPart);
				
				// If followed by a blank line, no headers.
				// Note, the header area is followed by a blank line,
				// that is not part of the body area.
				//
				if (isspace(*(char*)HeaderStart)) {
					// Has text, but no headers.
					// _BodyStart is an offset from the start of the entire message (an index).
					//
					NewBodyPart->_BodyStart = (uint32_t)(HeaderStart - _EntireMessage.Data); // An offset.
					NewBodyPart->_BodyLength = (uint32_t)(PartEnd - HeaderStart);
					
				} else {
					// Has Headers (we hope) followed by text.
					//
					Header * NewHeader = nullptr;
					PartStart = HeaderStart;
					
					do {
						NewHeader = _ParseHeader(HeaderStart, DebugMessages);
						if (NewHeader != nullptr) {
							NewBodyPart->_Headers.push_back(NewHeader);
							// Skip to next header.
							//
							HeaderStart = _EntireMessage.Data + (NewHeader->_ValueStart + NewHeader->_ValueLength);
							if (HeaderStart[0] != '\r' || HeaderStart[1] != '\n') {
								// ERROR
								// MSG TODO
								// Expected \r\n after header value.
								Error = true;
								break;
							}
							HeaderStart += 2; // Past \r\n at end of header value.

							// If another \r\n, then it is the start of the body
							// Else, it is the start of another header.
							//
							if (HeaderStart[0] == '\r' && HeaderStart[1] == '\n') {
								// End of headers. Start of body.
								//
								HeaderStart += 2;
								DataPtr = HeaderStart;
								HeadersDone = true;
								break;
							}
						} else {
							// ERROR _parseHeader error.
							//
							break;
						}
					}	while (!HeadersDone); // The only way out is error, or end of headers.

					// At this point it is the Start of the body area.
					//
					NewBodyPart->_BodyStart = (uint32_t)(HeaderStart - _EntireMessage.Data); // An offset.
					NewBodyPart->_BodyLength = (uint32_t)(PartEnd - HeaderStart);
					
				}
			} else {
				// End of body part headers.
				//
				break;
			}
				
		} while(!Error);

		// The entire length.
		//
		if (!Error) {
			Results = (uint32_t)(PartEnd - EntireBodyStart);
		}

		return(Results);
	}

}
