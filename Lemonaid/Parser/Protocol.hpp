/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_HPP_
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_HPP_

#include "Scope.hpp"
#include "Pack.hpp"
#include "SymbolTable.hpp"
#include "Version.hpp"
#include "Enum.hpp"

#include <vector>

namespace RiverExplorer::Phoenix::Protocol
{
	class Control
	{
	public:

		/**
		 * Control - Constructor.
		 */
		Control();

		/**
		 * Control - Destructor.
		 */
		virtual ~Control();
		
		/**
		 * The global Scope.
		 */
		Scope	* GlobalScope;

		/**
		 * A protocol has 1 or more versions.
		 * The version data is not really ordered,
		 * But I used a vector, to preserve the
		 * other the author of the protocol wrote them,
		 * in oder to possibly aid the readability.
		 */
		std::vector<Version*> Versions;
		
	private:
		 
	};
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_HPP_
