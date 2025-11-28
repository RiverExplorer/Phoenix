/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#include "Stream.hpp"

#include <fstream>

namespace RiverExplorer::Phoenix::Protocol
{

	Stream::Stream()
	{
		_Mode = NotOpen_t;
		_IO = nullptr;
			
		return;
	}

	Stream::~Stream()
	{
		switch (_Mode) {

		case NotOpen_t:
			/*EMPTY*/
			break;

		case OpenFile_t:
			Close();
			break;

		case OpenTCP_t:
			Close();
			break;

		case OpenUDP_t:
			Close();
			break;
		}
		
		return;
	}

	void
	Stream::Close()
	{
		switch (_Mode) {

		case NotOpen_t:
			/*EMPTY*/
			break;

		case OpenFile_t:
			{
				std::ofstream * FOut = dynamic_cast<std::ofstream*>(_IO);

				if (FOut != nullptr) {
					if (FOut->is_open()) {
						FOut->close();
						delete _IO;
						_IO = nullptr;
					}
				}
			}
			break;

		case OpenTCP_t:
			/**@todo*/
			break;
			
		case OpenUDP_t:
			/**@todo*/
			break;
		}

		_Mode = NotOpen_t;
		if (_IO != nullptr) {
			delete _IO;
			_IO = nullptr;
		}

		return;
	}
	
	bool
	Stream::OpenFile(std::string FileName)
	{
		bool	Results = false;

		std::fstream * FOut = new std::fstream(FileName);
		
		if (FOut->is_open()) {
			_Mode = OpenFile_t;
			_IO = FOut;
			Results = true;
		} else {
			_Mode = NotOpen_t;
		}
		
		return(Results);
	}
		
	bool
	Stream::OpenTCP(std::string IpOrHost, uint16_t Port)
	{
		bool	Results = false;

		/** @todo */
		
		return(Results);
	}
	
	bool
	Stream::OpenTCP_Tls(std::string IpOrHost, uint16_t Port)
	{
		bool	Results = false;

		/** @todo */
		
		return(Results);
	}
	
	bool
	Stream::OpenUDP(std::string IpOrHost, uint16_t Port)
	{
		bool	Results = false;

		/** @todo */
		
		return(Results);
	}

	bool
	Stream::OpenUDP_DTls(std::string IpOrHost, uint16_t Port)
	{
		bool	Results = false;

		/** @todo */
		
		return(Results);
	}

	Stream &
	operator<<(Stream & Out, uint8_t Value)
	{
		Out._write_uint8(Value);
		
		return(Out);
	}
	
	Stream &
	operator<<(Stream & Out, uint16_t Value)
	{
		Out._write_uint16(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, uint32_t Value)
	{
		Out._write_uint32(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, uint64_t Value)
	{
		Out._write_uint64(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, int8_t Value)
	{
		Out._write_int8(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, int16_t Value)
	{
		Out._write_int16(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, int32_t Value)
	{
		Out._write_int32(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, int64_t Value)
	{
		Out._write_int64(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, float Value)
	{
		Out._write_float(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, double Value)
	{
		Out._write_double(Value);

		return(Out);
	}

	Stream &
	operator<<(Stream & Out, long double Value)
	{
		Out._write_longdouble(Value);

		return(Out);
	}
}
