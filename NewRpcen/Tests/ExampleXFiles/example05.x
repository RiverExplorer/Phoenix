
// One line comment

namespace RiverExplorer:rpcgen:test ;

/**
 * Multi Line Comment.
 */
union CapabilityEntry switch (CMD_e Capability) {

	// One line comment before one.		
	 case one :
		 int int1;
	
	/**
	 * Multiline comment before ANONYMOUS.
	 */	
	 case ANONYMOUS :
		 float float1;

};

