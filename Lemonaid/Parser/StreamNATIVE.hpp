/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#ifndef __RIVEREXPLORER_PHOENIX_STREAMNATIVE_HPP_
#define __RIVEREXPLORER_PHOENIX_STREAMNATIVE_HPP_

#include <cstdint>
#include <string>
#include <iostream>
#include <ostream>
#include <cstdio>	// For EOF

#include "Stream.hpp"

namespace RiverExplorer::Phoenix::Protocol
{

	class StreamNATIVE
		: public Stream
	{

		StreamNATIVE();
		virtual ~StreamNATIVE();
		
private:

		Stream & _write_uint8(uint8_t Value);
		Stream & _write_uint16(uint16_t Value);
		Stream & _write_uint32(uint32_t Value);
		Stream & _write_uint64(uint64_t Value);

		Stream & _write_int8(int8_t Value);
		Stream & _write_int16(int16_t Value);
		Stream & _write_int32(int32_t Value);
		Stream & _write_int64(int64_t Value);

		Stream & _write_float(float Value);
		Stream & _write_double(double Value);
		Stream & _write_longdouble(long double Value);

		Stream & _write_string(std::string Value);


		Stream & _read_uint8(uint8_t & Value);
		Stream & _read_uint16(uint16_t & Value);
		Stream & _read_uint32(uint32_t & Value);
		Stream & _read_uint64(uint64_t & Value);

		Stream & _read_int8(int8_t & Value);
		Stream & _read_int16(int16_t & Value);
		Stream & _read_int32(int32_t & Value);
		Stream & _read_int64(int64_t & Value);

		Stream & _read_float(float & Value);
		Stream & _read_double(double & Value);
		Stream & _read_longdouble(long double & Value);

		Stream & _read_string(std::string & Value);
		
		friend Stream & operator<<(StreamNATIVE & Out, uint8_t Value);
		friend Stream & operator<<(StreamNATIVE & Out, uint16_t Value);
		friend Stream & operator<<(StreamNATIVE & Out, uint32_t Value);
		friend Stream & operator<<(StreamNATIVE & Out, uint64_t Value);

		friend Stream & operator<<(StreamNATIVE & Out, int8_t Value);
		friend Stream & operator<<(StreamNATIVE & Out, int16_t Valaue);
		friend Stream & operator<<(StreamNATIVE & Out, int32_t Value);
		friend Stream & operator<<(StreamNATIVE & Out, int64_t Value);

		friend Stream & operator<<(StreamNATIVE & Out, float Value);
		friend Stream & operator<<(StreamNATIVE & Out, double Value);
		friend Stream & operator<<(StreamNATIVE & Out, long double Value);

		friend Stream & operator>>(StreamNATIVE & In, uint8_t & Value);
		friend Stream & operator>>(StreamNATIVE & In, uint16_t & Value);
		friend Stream & operator>>(StreamNATIVE & In, uint32_t & Value);
		friend Stream & operator>>(StreamNATIVE & In, uint64_t & Value);

		friend Stream & operator>>(StreamNATIVE & In, int8_t & Value);
		friend Stream & operator>>(StreamNATIVE & In, int16_t & Value);
		friend Stream & operator>>(StreamNATIVE & In, int32_t & Value);
		friend Stream & operator>>(StreamNATIVE & In, int64_t & Value);

		friend Stream & operator>>(StreamNATIVE & In, float & Value);
		friend Stream & operator>>(StreamNATIVE & In, double & Value);
		friend Stream & operator>>(StreamNATIVE & In, long double & Value);
		
	};

	Stream & operator<<(StreamNATIVE & Out, uint8_t Value);
	Stream & operator<<(StreamNATIVE & Out, uint16_t Value);
	Stream & operator<<(StreamNATIVE & Out, uint32_t Value);
	Stream & operator<<(StreamNATIVE & Out, uint64_t Value);

	Stream & operator<<(StreamNATIVE & Out, int8_t Value);
	Stream & operator<<(StreamNATIVE & Out, int16_t Valaue);
	Stream & operator<<(StreamNATIVE & Out, int32_t Value);
	Stream & operator<<(StreamNATIVE & Out, int64_t Value);

	Stream & operator<<(StreamNATIVE & Out, float Value);
	Stream & operator<<(StreamNATIVE & Out, double Value);
	Stream & operator<<(StreamNATIVE & Out, long double Value);

	Stream & operator>>(StreamNATIVE & In, uint8_t & Value);
	Stream & operator>>(StreamNATIVE & In, uint16_t & Value);
	Stream & operator>>(StreamNATIVE & In, uint32_t & Value);
	Stream & operator>>(StreamNATIVE & In, uint64_t & Value);

	Stream & operator>>(StreamNATIVE & In, int8_t & Value);
	Stream & operator>>(StreamNATIVE & In, int16_t & Value);
	Stream & operator>>(StreamNATIVE & In, int32_t & Value);
	Stream & operator>>(StreamNATIVE & In, int64_t & Value);

	Stream & operator>>(StreamNATIVE & In, float & Value);
	Stream & operator>>(StreamNATIVE & In, double & Value);
	Stream & operator>>(StreamNATIVE & In, long double & Value);
}

#endif // __RIVEREXPLORER_PHOENIX_STREAMNATIVE_HPP_
