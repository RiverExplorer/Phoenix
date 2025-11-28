/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#include "StreamNATIVE.hpp"

#include <fstream>

namespace RiverExplorer::Phoenix::Protocol
{

	StreamNATIVE::StreamNATIVE()
	{
		
		return;
	}

	StreamNATIVE::~StreamNATIVE()
	{
		
		return;
	}
	
	Stream &
	StreamNATIVE::_write_uint8(uint8_t Value)
	{
		*_IO << Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_uint16(uint16_t Value)
	{
		*_IO << Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_uint32(uint32_t Value)
	{
		*_IO << Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_uint64(uint64_t Value)
	{
		*_IO << Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_int8(int8_t Value)
	{
		*_IO << Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_int16(int16_t Value)
	{
		*_IO << Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_int32(int32_t Value)
	{
		*_IO << Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_int64(int64_t Value)
	{
		*_IO << Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_float(float Value)
	{
		*_IO << Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_double(double Value)
	{
		*_IO << Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_write_longdouble(long double Value)
	{
		*_IO << Value;

		return(*this);
	}

	Stream &
	StreamNATIVE::_write_string(std::string Value)
	{
		*_IO << Value;

		return(*this);
	}

	Stream &
	StreamNATIVE::_read_uint8(uint8_t & Value)
	{
		*_IO >> Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_uint16(uint16_t & Value)
	{
		*_IO >> Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_uint32(uint32_t & Value)
	{
		*_IO >> Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_uint64(uint64_t & Value)
	{
		*_IO >> Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_int8(int8_t & Value)
	{
		*_IO >> Value;
		
		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_int16(int16_t & Value)
	{
		*_IO >> Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_int32(int32_t & Value)
	{
		*_IO >> Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_int64(int64_t & Value)
	{
		*_IO >> Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_float(float & Value)
	{
		*_IO >> Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_double(double & Value)
	{
		*_IO >> Value;

		return(*this);
	}
	
	Stream &
	StreamNATIVE::_read_longdouble(long double & Value)
	{
		*_IO >> Value;

		return(*this);
	}

	Stream &
	StreamNATIVE::_read_string(std::string & Value)
	{
		*_IO << Value;

		return(*this);
	}

	Stream &
	operator<<(StreamNATIVE & Out, uint8_t Value)
	{
		Out._write_uint8(Value);
		
		return(Out);
	}
	
	Stream &
	operator<<(StreamNATIVE & Out, uint16_t Value)
	{
		Out._write_uint16(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, uint32_t Value)
	{
		Out._write_uint32(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, uint64_t Value)
	{
		Out._write_uint64(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, int8_t Value)
	{
		Out._write_int8(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, int16_t Value)
	{
		Out._write_int16(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, int32_t Value)
	{
		Out._write_int32(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, int64_t Value)
	{
		Out._write_int64(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, float Value)
	{
		Out._write_float(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, double Value)
	{
		Out._write_double(Value);

		return(Out);
	}

	Stream &
	operator<<(StreamNATIVE & Out, long double Value)
	{
		Out._write_longdouble(Value);

		return(Out);
	}
}
