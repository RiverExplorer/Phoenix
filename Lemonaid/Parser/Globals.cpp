/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "Globals.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	int							Global::ProgramArgc = 0;
	char * const *	Global::ProgramArgv = nullptr;
	
	std::ofstream Global::Log;

	bool Global::SymbolsMustBePredefined = false;
	bool Global::ReadFromStdIn = false;
	
	std::string Global::InputFileName;
	std::string Global::OutputDirectory;
	std::string Global::LogFileName;
	std::string Global::CppDirName = "C++";
	std::string Global::CSharpDirName = "C#";

	Scope Global::GScope;
	std::deque<std::string> Global::CurrentFileName;
	std::deque<Scope*> Global::CurrentScope;
}
