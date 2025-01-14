/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#ifndef _RIVEREXPLORER_PHOENIX_IOV_HPP_
#define _RIVEREXPLORER_PHOENIX_IOV_HPP_

#include <deque>
#include <cstdint>

namespace RiverExplorer::Phoenix
{

	/**
	 * @class Iov
	 * Similar in idea to a (struct iov), with some control information.
	 */
	class Iov
	{
	public:

		/**
		 * Iov - Default Constructor.
		 */
		Iov();

		/**
		 * Iov - Destructor.
		 */
		~Iov();

		/**
		 * A blob of data.
		 */
		struct Blob
		{
			/**
			 * When true, this data is mmapped.
			 * When false, this data is allocated.
			 */
			bool IsMMapped;

			/**@todo Check W64, make sure we can close before munmap()*/
			
			/**
			 * The data.
			 */
			uint8_t	*	Data;

			/**
			 * As we take data, Data is updated.
			 * OriginalData is kept so we know what to delete or unmap.
			 */
			uint8_t * OriginalData;

			/**
			 * How much data.
			 */
			uint64_t	Length;

			/**
			 * Blob - Constructor.
			 *
			 * @param Data The data.
			 *
			 * @param Len The length of data.
			 *
			 * @param IsMMapped When true, Data is from mmap().
			 * When false, Data was allocated by us.
			 */
			Blob(uint8_t * Data, uint64_t Len, bool IsMMapped);

			/**
			 * Blob - Destructor.
			 */
			~Blob();

		};
		
		/**
		 * Total size of incomming or outgoing data.
		 */
		uint64_t	TotalLength;
			
		/**
		 * Total size of processed data incomming or outgoing data so far.
		 */
		uint64_t	ProcessedCount;
			
		/**
		 * Add a blob of data
		 *
		 * @param Data The data to add.
		 *
		 * @param Len The length of Data.
		 *
		 * @param IsMMapped When true, the data is mmaped.
		 * When false, it is no mmapped.
		 */
		void	Add(uint8_t * Data, uint64_t Len, bool IsMMapped);

		/**
		 * A special add for a uint64_t host byte order value
		 * Converts to network byte order and stores in the Q.
		 *
		 * @param Host64 A uint64_t value in host byte order.
		 */
		void Add(uint64_t Host64);

		/**
		 * During reading, we grab some, but the network may take
		 * less, so we Take() then later Return() the unused.
		 * And this code manages it for us.
		 *
		 * We Take() by Blob. So we don't have to
		 * allocate/free memory.
		 *
		 * @param[out] Len A reference to a uint64_t that tells
		 * us how much we got.
		 *
		 * @return A blobs data.
		 * Returns nullptr with Len == 0, when there is no more data.
		 */
		uint8_t	*	Take(uint64_t & Len);

		/**
		 * Return what we could not use yet by telling us
		 * how much use used.
		 *
		 * @param HowMuchWeUsed How many octets we used in the last Blob.
		 * When HowMuchWeUsed == Len of the blob, then all of it
		 * was used. Pop off the blob as it was used.
		 */
		void			WeUsed(uint64_t HowMuchWeUsed);

		/**
		 * Clear out all of the data.
		 */
		void			Clear();
		
	private:

		/**
		 * The queue of things to process.
		 */
		std::deque<Blob*>	_Q;
		
	};
}
	
#endif // _RIVEREXPLORER_PHOENIX_IOV_HPP_
