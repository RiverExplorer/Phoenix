
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_PACK_HPP_
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_PACK_HPP_

#include <cstdint>

namespace RiverExplorer::Phoenix
{
	class Pack
	{
	public:

		Pack();

		/**
		 * How to pack the bits that are sent.
		 * How to unpack them when received.
		 */
		enum PackingOrder_t {
			/** Network Byte Order */
			PackNBO,

			/** Pack hightest bit to left */
			PackLR,

			/** Pack hightest bit) to right */
			PackRL
		};

		/**
		 * Default to PackNBO
		 */
		PackingOrder_t PackingOrder;

		/**
		 * When true, then PackingSize is ignored.
		 * All bits are packed as needed without any padding.
		 *

		 * Default to false.
		 */
		bool					BitStream;
		
		/**
		 * Default to 8-bit. Defaults to 8.
		 */
		uint8_t PackSize;
	};
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_PACK_HPP_
