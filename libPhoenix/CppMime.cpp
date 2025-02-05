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
	MimeMessage::Parse(const char * FileName,
										 std::vector<std::string> & DebugMessages)
	{
		bool				Results = true;

		bool				Error = false;
		
		std::string	ContentTypeValue;
		std::string	BoundaryLine;
		std::string	MimeBoundary;
		
		bool						IsMime_10 = false;
		size_t					Mime_10Len = strlen(MimeVersion_s);
		size_t					ContentTypeLen = strlen(ContentType_s);

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
						uint32_t	BodyLength = 0;
					
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
							Header * NewHeader;
							do {

								while (*DataPtr != '\r' && !Error) {
									NewHeader = _ParseHeader(DataPtr, DebugMessages);

									if (NewHeader != nullptr) {
										DataPtr = Data + (NewHeader->_ValueStart + NewHeader->_ValueLength);

										// Should be EOL
										//
										if (strncmp((char*)DataPtr, "\r\n", 2) != 0) {
											DebugMessages.push_back(std::string("unexpected \\r\\n. (0.1)"));
											Error = true;
											break;
										}
										DataPtr += 2;
										
										_MessageHeaders.Add(NewHeader);
										// We want to know if it is a MIME
										// message, or a pre MIME 822 message type.
										//
										if (strncasecmp((char*)Data + NewHeader->_HeaderStart,
																		MimeVersion_s,
																		Mime_10Len) == 0) {

											if (strncasecmp((char*)Data + NewHeader->_ValueStart,
																			MimeVersion_Value_1_s,
																			3) == 0) {
												IsMime_10 = true;
											}
										}

										// We also want the Content type
										// for MIME messages, so we can extract
										// and boundry signatures.
										//
										if (strncasecmp((char*)Data + NewHeader->_HeaderStart,
																		ContentType_s,
																		ContentTypeLen) == 0) {

											// Get the value.
											//
											uint8_t * CTValue = Data + NewHeader->_ValueStart;
													
											ContentTypeValue = std::string((char*)CTValue,
																										 NewHeader->_ValueLength);
											BoundaryLine = "\r\n--";
											BoundaryLine += ContentTypeValue;
											BoundaryLine += "\r\n";
										}

									} else {
										Error = true;
										DebugMessages.push_back(std::string("_ParseHeaders() failed. (0)"));
										break;										
									}
								}
								
							} while(*DataPtr != '\r' && !Error);

							//
							// At this point, we are at the start of the body
							//

							// Evertything left is the entire body.
							//
							_EntireBody.IsMMapped = false;
							_EntireBody.IsAllocated = false;
							_EntireBody.Data = DataPtr;
							_EntireBody.Length = (uint32_t)(Data + SBuf.st_size - DataPtr);
								
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
									// Skip the word 'boundary'
									//
									Boundary += 8;
									
									// Found the boundry marker.
									// Skip everything until '='.
									//
									while (*Boundary != '=') {
										Boundary++;
										if (*Boundary == '\r' || *Boundary == '\n') {
											Error = true;
											DebugMessages.push_back(std::string("EOL unexpected. (1)"));
											break;
										}
									}
									if (!Error) {
										// Point past '='
										//
										Boundary++;

										// Now check for single or double quote.
										//
										// Quoting seems to be optional.
										// Some message have it, others do not.
										//
										bool					QuoteUsed = true;
										char					Quote = 0;
										const char	* QPtr = Boundary;

										while (*QPtr != '\0') {
											if (*QPtr == '\'' || *QPtr == '"') {
												QuoteUsed = true;
												Quote = *QPtr;
												break;
											}
											QPtr++;
										}
										if (*QPtr == '\0') {
											QuoteUsed = false;
										}

										if (!Error) {
											if (QuoteUsed) {
												Boundary++;
											}
												
											// Now grab the boundary value.
											//
											if (QuoteUsed) {
												while (*Boundary != Quote) {
													MimeBoundary += *Boundary;
													Boundary++;
													if (*Boundary == '\r' || *Boundary == '\n') {
														DebugMessages.push_back(std::string("EOL unexpected. (3)"));
														Error = true;
														break;
													}
												}
											} else {
												while (*Boundary != '\r') {
													MimeBoundary += *Boundary;
													Boundary++;
													if (*Boundary == '\r' || *Boundary == '\n') {
														if (QuoteUsed) {
															DebugMessages.push_back(std::string("EOL unexpected. (4)"));
															Error = true;
														}
														break;
													}
												}
											}

											if (!Error) {
												//const char * Body = (char*)&Data[BodyStart];
												uint8_t * Body = DataPtr;
												
												// Now scan the full body and
												// chunk the indexes into body parts.
												//
												BodyLength = _ParseBody(Body,	MimeBoundary, DebugMessages);

												if (BodyLength > 0) {
												} else {
													DebugMessages.push_back("_ParseBody() returned 0.");
													Error = true;
												}
											}
										}
									}
								}
							} else {
								// Is a pre-MIME body.
								//
								/**@todo write code for pre-MIME body*/
							}
						}
					}
				}
			} else {
				// Could not open file
				//
				DebugMessages.push_back(std::string("EOL unexpected. (5)"));
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

}
