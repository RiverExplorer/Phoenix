
namespace RiverExplorer:rpcgen:test;

struct VendorCapability
{
	int Count;

	int64_t * Data;

	opaque Opaque1<>;


	string String1<32>;

	float Float1[16];

	CustomName CustomName1[CUSTOM_MAX_VALUE];
};

