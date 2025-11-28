[Namespace=RiverExplorer:test:testsint1]
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

		const sint VariableName = -5;
		const sint MinVarName = -30;
		const sint MaxVarName = -25;

		sint si1;
		sint si1r;
		sint:8 si2;
		sint sia1[2];
		sint:16 sia2[5];
		sint siv1<10,20>;
		sint:64 siv2<0,14>;
		sint siiv3<>;

		sint si1O1 Default(-3);
		sint:8 si1O1_1 Default(-4);
		sint:16 si1O1_2 Default(-5);
		sint:32 si1O1_3 Default(-6);
		sint:64 si1O1_4 Default(-7);
		sint:64 si1O1_5 Default(VariableName);

		sint si1O2 Range(>-4,<-10);
		sint si1O2_0 Range(>MinVarName,<MaxVarName);
		sint:8 si1O2_1 Range(>-5,<-11);
		sint:16 si1O2_2 Range(>-6,<-12);
		sint:32 si1O2_3 Range(>-7,<-13);
		sint:64 si1O2_4 Range(>-8,<-14);

		sint si1O3 Default(-7) Range(>-20,<-1);
		sint:8 si1O3_1 Default(-8) Range(>-21,<-2);
		sint:16 si1O3_2 Default(-9) Range(>-22,<-3);
		sint:32 si1O3_3 Default(-10) Range(>-23,<-4);
		sint:64 si1O3_4 Default(-11) Range(>-24,<-5);

		sint si1O4 Range(>-30,<-6) Default(-10);
		sint:8 si1O4_1 Range(>-31,<-7) Default(-11);
		sint:16 si1O4_2 Range(>-32,<-8) Default(-12);
		sint:32 si1O4_3 Range(>-33,<-9) Default(-13);
		sint:64 si1O4_4 Range(>-34,<-10) Default(-14);

		sint asi1O4<10,20>	Range(>-100,<-10) Default(-15);
		sint:8 asi1O4_1<11,21>	Range(>-101,<-11) Default(-16);
		sint:16 asi1O4_2<12,22> Range(>-102,<-12) Default(-17);
		sint:32 asi1O4_3<13,23> Range(>-103,<-13) Default(-18);
		sint:64 asi1O4_4<14,24> Range(>-104,<-14) Default(-19);
	};
};
