
namespace RiverExplorer:rpcgen:test;

struct MethodTest1
{
	int Count;
	int64_t * Data;
	opaque Opaque1<>;
	string String1<32>;
	float Float1[16];

	/**
	 * @fn CountSet()
	 * Description of CountSet...
	 *
	 * @param NameOfParam1 Info about NameOfParam1 ...
	 *
	 * @param NameOfParam2 Info about NameOfParam2 ...
	 */ 
	void CountSet(int NameOfParam1, float NameOfParam2);

	/**
	 * @fn CountGet()
	 * Description of CountGet...
	 *
	 * @return The ...
	 */
	int CountGet(void);

};

