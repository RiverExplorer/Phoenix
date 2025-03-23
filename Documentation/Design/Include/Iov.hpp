namespace RiverExplorer::Phoenix
{
	/**
	 * @class Iov
	 * Similar in idea to a (struct iov), with some control information.
	 */
	class Iov
	{
		/* ... helper methods removed for brevity ... */

	public:
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

			/**
			 * The data.
			 */
			uint8_t	*	Data;

			/**
			 * How much data.
			 */
			uint32_t	Length;
		};

	private:
		/**
		 * The queue of things to process.
		 */
		std::deque<Blob*>	_Q;
	};

	/**
	 * @class IovCommand "Iov.hpp" <RiverExplorer/Phoenix/Iov.hpp>
	 * A set of Iov objects that represent a command.
	 *
	 * This is how a command packet is represented
	 * inside the implementation.
	 */
	class IovCommand
		: public Iov
	{
	public:
		/* ... methods removed for brevity... */

		/**
		 * The command.
		 */
		CMD_e Command;

		/**
		 * The sequence number.
		 */
		SEQ_t Sequence;
	}:

}
