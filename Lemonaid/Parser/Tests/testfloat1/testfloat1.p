[Namespace=RiverExplorer:test:testfloat1]
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

		const URange 	R1(>1,<2);
		const SRange 	R2(>-3,<4);
		const FRange	R3(>-4.5,<12.6);

		const FRange	CompassDegrees(>=0,<360.0);
		const URange	HourOfDay(>=0,<=23);
		const URange	MinuteOfHour(>=0,<=59);
		const URange	SecondOfMinute(>=0,<=59);

		const float VariableName = -5.2;
		const float MinVarName = -30.3;
		const float MaxVarName = -25.4;

		float si1;
		float si1r;
		float:8 si2;
		float sia1[2];
		float:16 sia2[5];
		float siv1<10,20>;
		float:64 siv2<0,14>;
		float siiv3<>;

		float si1O1 Default(-3.1);
		float:8 si1O1_1 Default(-4.2);
		float:16 si1O1_2 Default(-5.3);
		float:32 si1O1_3 Default(-6.4);
		float:64 si1O1_4 Default(-7.5);
		float:64 si1O1_5 Default(VariableName);

		float si1O2 Range(>-4.6,<-10.7);
		float si1O2_0 Range(>MinVarName,<MaxVarName);
		float:8 si1O2_1 Range(>-5.8,<-11.9);
		float:16 si1O2_2 Range(>-6.11,<-12.12);
		float:32 si1O2_3 Range(>-7.13,<-13.14);
		float:64 si1O2_4 Range(>-8.15,<-14.16);

		float si1O3 Default(-7.17) Range(>-20.18,<-1.19);
		float:8 si1O3_1 Default(-8.21) Range(>-21.22,<-2.23);
		float:16 si1O3_2 Default(-9.24) Range(>-22.25,<-3.26);
		float:32 si1O3_3 Default(-10.27) Range(>-23.28,<-4.29);
		float:64 si1O3_4 Default(-11.31) Range(>-24.32,<-5.33);

		float si1O4 Range(>-30.34,<-6.36) Default(-10.37);
		float:8 si1O4_1 Range(>-31.38,<-7.39) Default(-11.41);
		float:16 si1O4_2 Range(>-32.42,<-8.43) Default(-12.44);
		float:32 si1O4_3 Range(>-33.45,<-9.46) Default(-13.47);
		float:64 si1O4_4 Range(>-34.48,<-10.49) Default(-14.51);

		float asi1O4<10,20>	Range(>-100.54,<-10.55) Default(-15.56);
		float:8 asi1O4_1<11,21>	Range(>-101.59,<-11) Default(-16.61);
		float:16 asi1O4_2<12,22> Range(>-102.64,<-12.65) Default(-17.66);
		float:32 asi1O4_3<13,23> Range(>-103.69,<-13.71) Default(-18.72);
		float:64 asi1O4_4<14,24> Range(>-104.75,<-14.76) Default(-19.77);

		uint:8 Hour Range(HourOfDay);
		uint:8 Minute Range(MinuteOfHour);
		uint:7 Second Range(SecondOfMinute);
	};
};
