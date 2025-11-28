[Namespace=RiverExplorer:testmethod1]
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

// This is comment is about protocol simple.
//
protocol simple {
	// This is comment is about version version1.
	//
	version version1 {

	public:

		// VoidMethod().
		// returns void and has no parameters.
		//
		Function void VoidMethod();

		// VoidMethod2().
		// returns void and has no parameters.
		// the method is const;
		//
		Function void VoidMethod2() const;
	
		// Uint32Method1().
		// returns uint:32 and has no parameters.
		// the method is const;
		//
		Function uint:32 Uint32Method1() const;

		// Uint32Method1(uint:8 foo8).
		// returns uint:32 and has no parameters.
		// the method is const;
		//
		Function uint:32 Uint32MethodU8(uint:8 foo8) const;

		// Uint32MethodU8(uint:8 foo8, uint:16 foo16).
		// returns uint:32 and has no parameters.
		// the method is const;
		//
		Function uint:32 Uint32MethodU8(uint:8 foo8, uint:16 foo16) const;

		// Uint16Method1(uint:16 foo16, uint:64 foo64).
		// returns uint:32 and has no parameters.
		// the method is const;
		//
		Function const uint:16 Uint16MethodU1(uint:16 foo16, uint:64 foo64) const;
	};
};
