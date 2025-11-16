/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "Globals.hpp"
#include "Generate.hpp"

#include <sys/stat.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace RiverExplorer::Phoenix::Protocol
{

	bool
	Generate::_MkDir(std::string & Parent, std::string Child)
	{
		bool Results = true;

		std::string NewPath = Parent + "/" + Child;
		
		if (mkdir(NewPath.c_str(), 0700) != 0) {
			Global::Log << "ERROR: Can not create directory:"
					<< Child << " as child in " << Parent << " directory." << std::endl
					<< "Errno: " << strerror(errno)
					<< "\n";
			Results = false;
		}

		return(Results);
	}

	std::string
	Generate::ToIfDef(const std::string & FileName)
	{
		std::string Results = FileName;

		std::transform(Results.begin(), Results.end(), Results.begin(),
                   [](unsigned char c){ return std::toupper(c); });

		std::replace(Results.begin(), Results.end(), '.', '_');

		return(Results);
	}
	

	std::string
	Generate::CppNamespace(const Symbol & TargetSymbol)
	{
		std::string Results;

		if (TargetSymbol.Namespace.length() > 0) {
			std::stringstream SS(TargetSymbol.Namespace);
			std::string Token;

			while (std::getline(SS, Token, ':')) {
				Results += Token;
				Results += "::";
			}
		}

		return(Results);
	}
		 
	std::string
	Generate::CSharpNamespace(const Symbol & TargetSymbol)
	{
		std::string Results;

		if (TargetSymbol.Namespace.length() > 0) {
			std::stringstream SS(TargetSymbol.Namespace);
			std::string Token;

			while (std::getline(SS, Token, ':')) {
				Results += Token;
				Results += ".";
			}
		}

		return(Results);
	}
	
	std::string
	Generate::Tabs(uint8_t NumberTabs)
	{
		std::string Results;

		while (NumberTabs-- > 0) {
			Results += '\t';
		}

		return(Results);
	}
	
}
