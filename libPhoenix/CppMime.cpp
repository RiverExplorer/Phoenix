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
	const char * const MimeMessage::Boundary_s			= "boundary";
	const char * const MimeMessage::MimeVersion_s		= "MIME-Version";
	const char * const MimeMessage::MimeVersion_Value_1_s	= "1.0";
	const char * const MimeMessage::ContentDescription_s = "Content-Description";
  const char * const MimeMessage::ContentID_s		= "Content-ID";
  const char * const MimeMessage::ContentType_s = "Content-Type";
	
  const char * const MimeMessage::ContentTransferEncoding_s =
		"Content-Transfer-Encoding";

	MimeMessage::MimeMessage()
		: _MessageHeaders(*this)
	{
		
		_EntireMessage.IsMMapped = false;
		_EntireMessage.IsAllocated = false;
		_EntireMessage.Data = nullptr;
		_EntireMessage.Length = 0;

		return;
	}
	
	MimeMessage::~MimeMessage()
	{

		if (_EntireMessage.IsMMapped
				&& _EntireMessage.Data != nullptr
				&& _EntireMessage.Length > 0) {
			
			munmap(_EntireMessage.Data, _EntireMessage.Length);
			_EntireMessage.Data = nullptr;
		}

		if (_EntireMessage.IsAllocated
				&& _EntireMessage.Data != nullptr
				&& _EntireMessage.Length > 0) {

			delete[] _EntireMessage.Data;
			_EntireMessage.Data = nullptr;
		}
		_EntireMessage.Length = 0;

		return;
	}


	bool
	MimeMessage::_IncDataPointer(uint8_t *& Ptr) const
	{
		bool Results = false;

		Ptr++;

		if (Ptr > _EntireMessage.Data + _EntireMessage.Length - 1) {
			--Ptr;
			Results = true;
		}
		
		return(Results);
	}
	
	bool
	MimeMessage::Parse(const char * FileName)
	{
		bool				Results = true;

		bool				IsMime_10 = false;
		size_t			Mime_10Len = strlen(MimeVersion_s);
		size_t			ContentTypeLen = strlen(ContentType_s);
		bool				Error = false;
		
		std::string	ContentTypeValue;
		std::string	BoundaryLine;
		std::string	MimeBoundary;
		
		// mmap the file.
		//
		if (FileName != nullptr) {
			int Fd = open(FileName, O_RDONLY);

			if (Fd > -1) {
				struct stat SBuf;

				fstat(Fd, &SBuf);

				if (SBuf.st_size < 1) {
					Results = false;
				} else {
					uint8_t	*	Data = (uint8_t*)mmap(nullptr,
																					SBuf.st_size,
																					PROT_READ,
																					MAP_PRIVATE,
																					Fd,
																					0);

					close(Fd);
					// Headers followed by body.
					//
					if (Data != nullptr) {

						uint8_t	*	DataPtr = Data;
						uint32_t	DataOffset = 0;
						uint32_t	HeaderStart = 0;
						uint32_t	HeaderLength = 0;
						uint32_t	HeaderValueStart = 0;
						uint32_t	HeaderValueLength = 0;
						Header	*	NewHeader = nullptr;
					
						// Remember the entire message data.
						//
						_EntireMessage.IsMMapped = true;
						_EntireMessage.IsAllocated = false;
						_EntireMessage.Length = (uint32_t)SBuf.st_size;
						_EntireMessage.Data = Data;

						// The first character should be
						// Start parsing the headers.
						//
						if (*Data != '\r') {
							// Not a new line (they end in \r\n).
							// Parse Headers.
							//
							do {
								// We should be at the start of a header.
								//
								while (*DataPtr != '\r' && !Error) {
									HeaderStart = DataOffset;
									HeaderLength = 1;
									HeaderValueLength = 1;

									if (!_IncDataPointer(DataPtr)) {
										DataOffset++;

										while (*DataPtr != ':' && !Error) {
											Error = _IncDataPointer(DataPtr);
											if (Error) {
												break;
											}
											DataOffset++;
											HeaderLength++;
										}
										DataOffset++;
										DataPtr++;

										if (!Error) {
											// End of header name.
											//
											// HeaderStart is the offset to start of this header.
											// HeaderValueLength is set to the lenght of the header.
											//
											// Now get the header value.
											//
											while (isspace(*DataPtr)) {
												Error = _IncDataPointer(DataPtr);
												if (Error) {
													break;
												}
												DataOffset++;
											}

											// Now pointing at the start of the header value.
											// It could be a single line,
											// or it could have continutation lines.
											//
											HeaderValueStart = DataOffset;

											do {
												while(*DataPtr != '\r' && !Error) {
													Error = _IncDataPointer(DataPtr);
													DataOffset++;
												}

												if (!Error) {
													// Now pointing at the end of the header value.
													//
													HeaderValueLength = DataOffset - HeaderValueStart;
										
													// Skip over the EOL
													//
													Error = _IncDataPointer(DataPtr);
													if (Error) {
														break;
													}
													Error = _IncDataPointer(DataPtr);
													if (Error) {
														break;
													}

													DataOffset += 2;

													// Now we are pointing at the start of the next header,
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
											} while (isspace(*DataPtr)
															 && *DataPtr != '\r'
															 && !Error);
											
											// All headers processed, now
											// use all or just process the body parts.
											//
											if (!Error) {
												// We have the start of the header.
												// We have the header length.
												// We have the start of the header value.
												// We have the header value length.
												//
												NewHeader = new Header(*this,
																							 HeaderStart,
																							 HeaderLength,
																							 HeaderValueStart,
																							 HeaderValueLength);
												_MessageHeaders.Add(NewHeader);

												// We want to know if it is a MIME
												// message, or a pre MIME 822 message type.
												//
												if (strncasecmp((char*)&Data[HeaderStart],
																				MimeVersion_s,
																				Mime_10Len) == 0) {
													if (strncasecmp((char*)&Data[HeaderValueStart],
																				MimeVersion_Value_1_s,
																				3) == 0) {
														IsMime_10 = true;
													}
												}
												// We also want the Content type
												// for MIME messages, so we can extract
												// and boundry signatures.
												//
												if (strncasecmp((char*)&Data[HeaderStart],
																				ContentType_s,
																				ContentTypeLen) == 0) {

													// Get the value.
													//
													uint8_t * CTValue = &Data[HeaderValueStart];

													ContentTypeValue = std::string((char*)CTValue,
																												 HeaderValueLength);
													BoundaryLine = "\r\n--";
													BoundaryLine += ContentTypeValue;
													BoundaryLine = "\r\n";

													const char * Find = BoundaryLine.c_str();
													const char * Body = (char*)&Data[BodyStart];
													
													// Now scan the full body and
													// chunk it into body parts.
													//
													_ParseBody(_Parent, Body, Find);
													
												}
											} else {
												break;
											}
										} else {
											break;
										}
									} else {
										break;
									}
								}
							} while(*DataPtr != '\r' && !Error);

							// At this point, we are at the start of the body
							//
							uint32_t BodyStart = DataOffset;
							uint32_t BodyLength = 0;

							// Evertything is the body.
							//
							_EntireBody.IsMMapped = false;
							_EntireBody.IsAllocated = false;
							_EntireBody.Data = &Data[BodyStart];
							_EntireBody.Length = _EntireMessage.Length - DataOffset;
								
							// If MIME, tag the location of the mime part body parts.
							//
							if (IsMime_10) {
								// Get the "Content-Type" from the main headers.
								// Extract out the 'boundry' value.
								// Then divide up by boundry chunks.
								//
								/**@todo tag MIME body parts */

								const char * Str = ContentTypeValue.c_str();
								const char * Boundary = strstr(Str, Boundary_s);

								if (Boundary != nullptr) {
									// Found the boundry marker.
									// Skip '=';
									//
									while (*Boundary != '=') {
										Boundary++;
										if (*Boundary == '\r' || *Boundary == '\n') {
											Error = true;
											break;
										}
									}
									if (!Error) {
										// Point past '='
										//
										Boundary++;

										// Now check for single or double quote.
										//
										char  Quote;
										
										while (*Boundary != '\'' && *Boundary != '"') {
											Boundary++;
											if (*Boundary == '\r' || *Boundary == '\n') {
												Error = true;
												break;
											}
											if (!Error) {
												Quote = *Boundary;
												Boundary++;
												
												// Now grab the boundary value.
												//
												while (*Boundary != Quote) {
													MimeBoundary += *Boundary;
													Boundary++;
													if (*Boundary == '\r' || *Boundary == '\n') {
														Error = true;
														break;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			} else {
				// Could not open file
				//
				Error = true;
			}
		}
		if (Error) {
			Results = false;
		}
		return(Results);
	}

	uint8_t	*
	MimeMessage::Message(uint32_t & Length) const
	{
		Length = _EntireMessage.Length;
		return(_EntireMessage.Data);
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

	uint8_t	*
	MimeMessage::GetEntireBody(uint32_t & Length) const
	{
		Length = _EntireBody.Length;
		return(_EntireBody.Data);
	}
	
}
