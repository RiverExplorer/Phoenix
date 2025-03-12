
// Pre comment

namespace RiverExplorer:rpcgen:test;

struct VendorCapability
{
	/**
	 * Count Comment.
	 */
	int Count;

	// Date Comment.
	//
	int64_t * Data;

	/**
	 * Opaque<> comment.
	 */
	opaque Opaque1<>;

	/**
	 * Opaque1<2> comment.
	 */
	opaque Opaque2<2>;

	/**
	 * String1<32> comment.
	 */
	string String1<>;

	/**
	 * String1<32> comment.
	 */
	string String1<32>;

	// Float1[16] comment.

	float Float1[16];

	/*
 	 * CustomName1[CUSTOM_MAX_VALUE] comment.
	 */

	CustomName CustomName1[CUSTOM_MAX_VALUE];
};

