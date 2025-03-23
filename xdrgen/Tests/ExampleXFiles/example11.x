
namespace RiverExplorer:rpcgen:test;

const DOZEN = 12;

typedef int SimpleInt;
typedef int IntFixedArray[10];
typedef int IntVarArray<>;
typedef int IntVarArray2<2>;

typedef struct tm TheTimes1;
typedef struct tm TheTimes2[10];
typedef struct tm TheTimes3<10>;
typedef struct tm TheTimes4<>;

typedef struct tm * TheTimes5;
typedef struct tm * TheTimes6[10];
typedef struct tm * TheTimes7<10>;
typedef struct tm * TheTimes8<>;
	
};

