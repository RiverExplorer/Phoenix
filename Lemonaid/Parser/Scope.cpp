/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "Scope.hpp"

namespace RiverExplorer::Phoenix::Protocol
{

	std::vector<Scope*> Scope::AllScopes;
	
	Scope::Scope()
	{
		AllScopes.push_back(this);

		return;
	}

	Scope::~Scope()
	{
		/*EMPTY*/
	}
	
	bool
	Scope::Contains(const std::string & ToCheckNamespace,
									const std::string & IdToCheck) const
	{
		bool Results = false;

		if (Symbols.Contains(ToCheckNamespace, IdToCheck)) {
			Results = true;
		}
		
		return(Results);
	}
		 
	bool
	Scope::ParentsContain(const std::string & ToCheckNamespace,
												const std::string & IdToCheck) const
	{
		bool Results = false;

		return(Results);
	}

}
