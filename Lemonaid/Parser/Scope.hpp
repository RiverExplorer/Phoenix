/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */

#ifndef __RIVEREXPLORER_PHOENIX_SCOPE_HPP_
#define __RIVEREXPLORER_PHOENIX_SCOPE_HPP_

#include <cstdint>
#include <string>
#include "Pack.hpp"
#include "SymbolTable.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	class Scope
	{
	public:

		/**
		 * Scope - Constructor
		 */
		Scope();

		/**
		 * Scope - Destructor.
		 */
		virtual ~Scope();
		
		/**
		 * Check to see if this scope a symbol
		 * with the procided ID.
		 *
		 * @param Namespace The symbols namespace.
		 * If none, pass in an empty string.

		 *
		 * @param IdToCheck The ID to check for.
		 *
		 * @return true if it has IdToCheck, else
		 * returns false.
		 */
		bool Contains(const std::string & Namespace,
									const std::string & IdToCheck) const;
		 
		/**
		 * Check to see if any parent scope has a symbol
		 * with the procided ID.
		 *
		 * @param Namespace The symbols namespace.
		 * If none, pass in an empty string.
		 *
		 * @param IdToCheck The ID to check for.
		 *
		 * @return true if it has IdToCheck, else
		 * returns false.
		 */
		bool ParentsContain(const std::string & Namespace,
												const std::string & IdToCheck) const;
		 
		Pack	Packing;
		SymbolTable Symbols;

		bool GenerateCPP;
		bool GenerateCSharp;

		bool GenerateCBOR_Client;
		bool GenerateCBOR_Server;
		
		bool GenerateJSON_Client;
		bool GenerateJSON_Server;
		
		bool GenerateNATIVE_Client;
		bool GenerateNATIVE_Server;

		bool GenerateREST_Client;
		bool GenerateREST_Server;
		
		bool GenerateXDR_Client;
		bool GenerateXDR_Server;
		
		bool GenerateXML_Client;
		bool GenerateXML_Server;

		std::string Namespace;
		std::string InputFileName;

		bool Document;
		
		/**
		 * All scopes
		 */
 		static std::vector<Scope*> AllScopes;

	private:
		
	};
}

#endif // __RIVEREXPLORER_PHOENIX_SCOPE_HPP_
