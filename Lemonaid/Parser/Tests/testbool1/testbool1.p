[Namespace=RiverExplorer:test:testbool1]
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

		const bool LightsOn = true;
		const bool LightOff = false;

		bool bi1;
		bool bi1r;
		bool bia1[2];
		bool biv1<10,20>;
		bool biiv3<>;

		bool bi1O1 Default(true);
		bool bi1O2<10,30> Default(false);
		bool bi1O3<> Default(true);

	};
};
