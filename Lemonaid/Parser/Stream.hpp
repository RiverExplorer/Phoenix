/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#ifndef __RIVEREXPLORER_PHOENIX_STREAM_HPP_
#define __RIVEREXPLORER_PHOENIX_STREAM_HPP_

#include <cstdint>
#include <string>
#include <iostream>
#include <ostream>
#include <istream>
#include <cstdio>	// For EOF

namespace RiverExplorer::Phoenix::Protocol
{

	class Stream
	{
	public:

		/**
		 * Stream - Default Constructor.
		 *
		 * @note
		 * Stream is a pure virtual class. So you will
		 * not be able to instianciate it directly.
		 */
		Stream();

		/**
		 * Stream - Destructor.
		 */
		virtual ~Stream();

		/**
		 * Open a file
		 *
		 * @param FileName The filename to open.
		 *
		 * @return True if success, else false.		 
		 */
		virtual bool OpenFile(std::string FileName);

		/**
		 * Open a TCP connection.
		 *
		 * @param IpOrHost The IP address or host name.
		 *
		 * @param Port the IP port number to use on the
		 * remote end.
		 *
		 * @return True if success, else false.		 
		 */
		virtual bool OpenTCP(std::string IpOrHost, uint16_t Port);
		
		/**
		 * Open a TCP connection using TLS.
		 * This implementation uses openssl.
		 *
		 * @param IpOrHost The IP address or host name.
		 *
		 * @param Port the IP port number to use on the
		 * remote end.
		 *
		 * @return True if success, else false.		 
		 */
		virtual bool OpenTCP_Tls(std::string IpOrHost, uint16_t Port);

		/**
		 * Setup for UDP data transfer.
		 *
		 * @param IpOrHost The IP address or host name.
		 *
		 * @param Port the IP port number to use on the
		 * remote end.
		 *
		 * @return True if success, else false.		 
		 */
		virtual bool OpenUDP(std::string IpOrHost, uint16_t Port);
		
		/**
		 * Setup for UDP data transfer, using DTLS
		 * This implementation uses openssl.
		 *
		 * @param IpOrHost The IP address or host name.
		 *
		 * @param Port the IP port number to use on the
		 * remote end.
		 *
		 * @return True if success, else false.		 
		 */
		virtual bool OpenUDP_DTls(std::string IpOrHost, uint16_t Port);

		/**
		 * Close the stream.
		 */
		virtual void Close();

		/**
		 * Get the byte order of the remote end.
		 * We send a 0x0102030405060708 and a long double in native byte order and the
		 * remote end sends a 0x0102030405060708 and a long double in native byte order.
		 *
		 * If what is received is the same, then we can skip the network byte
		 * order code, and just send/receive in host byte order.
		 *
		 * @return true if we can do I/O in host byte order.
		 */
		bool CheckRemoteByteOrder() const;
		
	protected:

		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_uint8(uint8_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_uint16(uint16_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_uint32(uint32_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_uint64(uint64_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_int8(int8_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_int16(int16_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_int32(int32_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_int64(int64_t Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_float(float Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_double(double Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_longdouble(long double Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to send the data (Value).
		 *
		 * @param Value The data to send.
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _write_string(std::string Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_uint8(uint8_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_uint16(uint16_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_uint32(uint32_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_uint64(uint64_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_int8(int8_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_int16(int16_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_int32(int32_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_int64(int64_t & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_float(float & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_double(double & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_longdouble(long double & Value) = 0;
		
		/**
		 * This is a pure virtual method.
		 * A derriving class implements its own version
		 * in order to receive the data (Value).
		 *
		 * @param Value Where to place the received data..
		 *
		 * @return The stream that was used.
		 */
		virtual Stream & _read_string(std::string & Value) = 0;

		/**
		 * The current Stream mode.
		 */
		enum OpenMode_e {
			NotOpen_t,
			OpenFile_t,
			OpenTCP_t,
			OpenUDP_t
		};

		/**
		 * The stream to write to.
		 */
		std::iostream	*	_IO;

		/**
		 * The current state of the Out stream.
		 */
		OpenMode_e			_Mode;
		
		friend Stream & operator<<(Stream & Out, uint8_t Value);
		friend Stream & operator<<(Stream & Out, uint16_t Value);
		friend Stream & operator<<(Stream & Out, uint32_t Value);
		friend Stream & operator<<(Stream & Out, uint64_t Value);

		friend Stream & operator<<(Stream & Out, int8_t Value);
		friend Stream & operator<<(Stream & Out, int16_t Valaue);
		friend Stream & operator<<(Stream & Out, int32_t Value);
		friend Stream & operator<<(Stream & Out, int64_t Value);

		friend Stream & operator<<(Stream & Out, float Value);
		friend Stream & operator<<(Stream & Out, double Value);
		friend Stream & operator<<(Stream & Out, long double Value);

		friend Stream & operator>>(Stream & In, uint8_t & Value);
		friend Stream & operator>>(Stream & In, uint16_t & Value);
		friend Stream & operator>>(Stream & In, uint32_t & Value);
		friend Stream & operator>>(Stream & In, uint64_t & Value);

		friend Stream & operator>>(Stream & In, int8_t & Value);
		friend Stream & operator>>(Stream & In, int16_t & Value);
		friend Stream & operator>>(Stream & In, int32_t & Value);
		friend Stream & operator>>(Stream & In, int64_t & Value);

		friend Stream & operator>>(Stream & In, float & Value);
		friend Stream & operator>>(Stream & In, double & Value);
		friend Stream & operator>>(Stream & In, long double & Value);
	};

	Stream & operator<<(Stream & Out, uint8_t Value);
	Stream & operator<<(Stream & Out, uint16_t Value);
	Stream & operator<<(Stream & Out, uint32_t Value);
	Stream & operator<<(Stream & Out, uint64_t Value);

	Stream & operator<<(Stream & Out, int8_t Value);
	Stream & operator<<(Stream & Out, int16_t Valaue);
	Stream & operator<<(Stream & Out, int32_t Value);
	Stream & operator<<(Stream & Out, int64_t Value);

	Stream & operator<<(Stream & Out, float Value);
	Stream & operator<<(Stream & Out, double Value);
	Stream & operator<<(Stream & Out, long double Value);

	Stream & operator>>(Stream & In, uint8_t & Value);
	Stream & operator>>(Stream & In, uint16_t & Value);
	Stream & operator>>(Stream & In, uint32_t & Value);
	Stream & operator>>(Stream & In, uint64_t & Value);

	Stream & operator>>(Stream & In, int8_t & Value);
	Stream & operator>>(Stream & In, int16_t & Value);
	Stream & operator>>(Stream & In, int32_t & Value);
	Stream & operator>>(Stream & In, int64_t & Value);

	Stream & operator>>(Stream & In, float & Value);
	Stream & operator>>(Stream & In, double & Value);
	Stream & operator>>(Stream & In, long double & Value);

}

#endif // __RIVEREXPLORER_PHOENIX_STREAM_HPP_
