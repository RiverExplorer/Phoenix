/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTCOL_GLOBALS_HPP__
#define __RIVEREXPLORER_PHOENIX_PROTCOL_GLOBALS_HPP__

#include "Scope.hpp"
#include "Range.hpp"

#include <string>
#include <fstream>
#include <deque>

namespace RiverExplorer::Phoenix::Protocol
{
	struct Global
	{
		/**
		 * -no-predef or -predef.
		 */
		static bool SymbolsMustBePredefined;

		/**
		 * --stdin
		 */
		static bool ReadFromStdIn;

		/**
		 * If !--stdin, then last command line argument
		 * that is not a option.
		 */
		static std::string InputFileName;

		/**
		 * The current file name being read from.
		 * Set at start, then updated at the start and end
		 * of each [Include].
		 */
		static std::deque<std::string> CurrentFileName;

		/**
		 * The default output directory is 'Generated'.
		 */
		static std::string OutputDirectory;

		/**
		 * The name of the parser generated log file.
		 * It is created in the OutputDirectory.
		 *
		 * The log file name is <OutputDirectory>/<InputFileName>.log
		 * This is not overridable from the command line.
		 */
		static std::string LogFileName;

		/**
		 * The log file stream.
		 */
		static std::ofstream Log;

		/**
		 * The gloabl scope.
		 */
		static Scope	GScope;

		/**
		 * The current scope being used.
		 * Set at start, then updated at the start and end
		 * of each scope change.
		 */
		static std::deque<Scope*> CurrentScope;

		/**
		 * The name of the directory where the C++ code wil be
		 * placed, under OutputDirectory.
		 */
		static std::string CppDirName;

		/**
		 * The name of the directory where the C# code wil be
		 * placed, under OutputDirectory.
		 */
		static std::string CSharpDirName;

	};
}
#endif // __RIVEREXPLORER_PHOENIX_PROTCOL_GLOBALS_HPP__
