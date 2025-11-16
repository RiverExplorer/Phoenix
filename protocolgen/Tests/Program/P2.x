
struct S1 {
	int iOne;
	float fTwo;
};

program Program200 {
	version Version_100 {
		int foo_1(S1) = 1;
	} = 100;
} = 200;

