
namespace RiverExplorer:rpcgen:test;

struct MethodTest1
{
	int Count;
	int64_t * Data;
	opaque Opaque1<>;
	string String1<32>;
	float Float1[16];

	void CountSet(int);
	int CountGet(void);

};

