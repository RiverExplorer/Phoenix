[Namespace=RiverExplorer:test:test3]
[PackNBO,DefaultPackSize:8]
[C++]
[C#]
[DOCUMENT]
[NoGenCBOR]
[NoGenJSON]
[GenNATIVE]
[NoGenREST]
[NoGenXDR]
[NoGenXML]
protocol simple {
	version version1 {

	public:

		const uint VariableName = 5;
		const uint MinVarName = 3;
		const uint MaxVarName = 25;

		sint si1;
		sint si1r;
		sint:8 si2;
		sint sia1[2];
		sint:16 sia2[5];
		sint siv1<10,20>;
		sint:64 siv2<0,14>;
		sint siiv3<>;

		uint ui1;
		uint:8 ui2;
		uint uia1[2];
		uint:16 uia2[5];
		uint uiv1<10,20>;
		uint:64 uiv2<0,14>;
		uint uiiv3<>;
		uint:12 uiv4<>;

		uint ui1O1 Default(3);
		uint:8 ui1O1_1 Default(3);
		uint:16 ui1O1_2 Default(3);
		uint:32 ui1O1_3 Default(3);
		uint:64 ui1O1_4 Default(3);
		uint:64 ui1O1_5 Default(VariableName);

		uint ui1O2 Range(>4,<10);
		uint ui1O2_0 Range(>MinVarName,<MaxVarName);
		uint:8 ui1O2_1 Range(>4,<10);
		uint:16 ui1O2_2 Range(>4,<11);
		uint:32 ui1O2_3 Range(>4,<12);
		uint:64 ui1O2_4 Range(>4,<13);

		uint ui1O3 Default(7) Range(>8,<101);
		uint:8 ui1O3_1 Default(7) Range(>8,<102);
		uint:16 ui1O3_2 Default(7) Range(>8,<103);
		uint:32 ui1O3_3 Default(7) Range(>8,<104);
		uint:64 ui1O3_4 Default(7) Range(>8,<105);

		uint ui1O4 Range(>8,<100) Default(45);
		uint:8 ui1O4_1 Range(>8,<100) Default(46);
		uint:16 ui1O4_2 Range(>8,<100) Default(47);
		uint:32 ui1O4_3 Range(>8,<100) Default(48);
		uint:64 ui1O4_4 Range(>8,<100) Default(49);

		uint aui1O4<10,20>	Range(>8,<100)	Default(45);
		uint:8 aui1O4_1<11,21>	Range(>9,<101)	Default(46);
		uint:16 aui1O4_2<12,22> Range(>10,<102) Default(47);
		uint:32 aui1O4_3<13,23> Range(>11,<103) Default(48);
		uint:64 aui1O4_4<14,24> Range(>12,<104) Default(49);


	};
};
