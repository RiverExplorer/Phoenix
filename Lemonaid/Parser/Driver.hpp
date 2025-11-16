#ifndef __RIVEREXPLORER_PROTOCOL_DRIVER_HPP_
#define __RIVEREXPLORER_PROTOCOL_DRIVER_HPP_

#include <string>
#include <cstddef>
#include <istream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "Scanner.hpp"
#include "Parser.tab.hpp"

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

namespace RiverExplorer::Phoenix::Protocol
{
	class Driver
	{
	public:

		Driver() = default;

		virtual ~Driver();

		/**
		 * Parse from a file.
		 *
		 * @param FileName The input file name.
		 */
		void Parse(const std::string & FileName, bool WithDebug = false);

		/**
		 * Parse from a C++ input stream.
		 *
		 * @param In The input stream.
		 */
		void Parse(std::istream & In, bool WithDebug = false);

		/**
		 * Log documents and debuging information.
		 *
		 * Create a documentation file as an outputfile.
		 * Name it the input file name, with a .doc added.
		 *
		 * If no input file name was provided, (stdin),
		 * then it will be named stdin.doc.
		 */
		static std::ofstream * OpenLog();

		static bool VerboseDebug;
		
		/**
		 * Dump the symbol tables.
		 *
		 * @param Out Where to send the symbol desciptions.
		 */
		void DumpSymbolTables(std::ostream & Out);

	private:

		void _DebugOn();
		
		void _ParseHelper(std::istream & Stream, bool WithDebug);

		RiverExplorer::Phoenix::Protocol::Parser * _Parser = nullptr;
		RiverExplorer::Phoenix::Protocol::Scanner * _Scanner = nullptr;

		static char * _FileName;
		static std::ofstream * _LogFile;
		
	};
}

#endif // __RIVEREXPLORER_PROTOCOL_DRIVER_HPP_

