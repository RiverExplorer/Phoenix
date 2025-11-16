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

		uint ui1O1 {Default=3};
		uint ui1O2 {Range:>4,<10};

	};
};
