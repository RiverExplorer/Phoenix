/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer™ LLC
 */

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <format>
#include <string>

#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "Globals.hpp"
#include "RmDirRecursive.hpp"

#include "Driver.hpp"
#include "Parser.tab.hpp"

#include "GenerateCPP.hpp"
#include "GenerateCBOR.hpp"
#include "GenerateJSON.hpp"
#include "GenerateNATIVE.hpp"
#include "GenerateREST.hpp"
#include "GenerateXDR.hpp"
#include "GenerateXML.hpp"

using namespace RiverExplorer::Phoenix;
using namespace std;

void
Usage(int argc, char * const * argv)
{
	cout << endl
			 << basename(argv[0])
			 << " usage: " << endl
			 << endl;

	cout << "--c++" << endl
			 << "  Generate C++ languge code." << endl
			 << endl;
		
	cout << "--csharp" << endl
			 << "  Generate C# language code." << endl
			 << endl;
		
	cout << "--help" << endl
			 << "  This message." << endl
			 << endl;
	
	cout << "--no-predef" << endl
			 << "  (default) The data types do not need to be defined"
			 << endl
			 << "  before being used." << endl
			 << endl;

	cout << "--outdir=dir" << endl
			 << "  The default output directory is 'Generated'" << endl
			 << "  This sets the output directory to 'dir'." << endl
			 << "  'dir' will created if needed." << endl
			 << "  The contents of this directory are cleared" << endl
			 << "  and created each run of this program." << endl
			 << endl;
	
	cout << "--predef" << endl
			 << "  The data type must be defined before used." << endl
			 << "  The default is --no-predef" << endl
			 << endl;

	cout << "--stdin" << endl
			 << "  Read the definition from stdin and not the file" << endl
			 << "  name specified on the command line." << endl
			 << endl;

	cout << "NOTE:" << endl
			 << "  If no output language is specified. The program will" << endl
			 << "  run, and no output will be generated. It can be used" << endl
			 << "  to test and pre-run your input definition files." << endl
			 << endl;
	
	cout << "RiverExplorer is a trademark of RiverExplorer LLC."
			 << endl
			 << "This program is free to use as long as this notice is"
			 << " kept intact and visible." << endl
			 << "Copyright (C) 2025, RiverExplorer™ LLC" << endl
			 << endl;

	return;
}

int
main(const int argc, char * const * argv)
{
	Global::ProgramArgc = argc;
	Global::ProgramArgv = argv;
	
	Global::GScope.GenerateCPP = false;
	Global::GScope.GenerateCSharp = false;

	Global::GScope.GenerateCBOR_Client = false;
	Global::GScope.GenerateCBOR_Server = false;
	
	Global::GScope.GenerateJSON_Client = false;
	Global::GScope.GenerateJSON_Server = false;
	
	Global::GScope.GenerateNATIVE_Client = false;
	Global::GScope.GenerateNATIVE_Server = false;
	
	Global::GScope.GenerateREST_Client = false;
	Global::GScope.GenerateREST_Server = false;
	
	Global::GScope.GenerateXDR_Client = false;
	Global::GScope.GenerateXDR_Server = false;
	
	Global::GScope.GenerateXML_Client = false;
	Global::GScope.GenerateXML_Server = false;
	
	const int NoArg = 0;
	const int HasArg = 1;

	// One or more chosen is okay.
	//
	bool OutputLanguageChosen = false;
	
	int Opt;
	bool More = true;

	Global::OutputDirectory = "Generated";
	
	while (More) {
		int OptionIndex = 0;

		static struct option LongOptions[] = {
			{"stdin",			NoArg,	nullptr, 1},
			{"no-predef", NoArg,	nullptr, 2},
			{"predef",		NoArg,	nullptr, 3},
			{"help",			NoArg,	nullptr, 4},
			{"outdir",		HasArg,	nullptr, 5},
			{"c++",				NoArg,	nullptr, 6},
			{"csharp",		NoArg,	nullptr, 7},
			{nullptr,			NoArg,	nullptr, 0}
		};

		Opt = getopt_long(argc, argv, "",
											LongOptions,
											&OptionIndex);

		switch (Opt) {

		case 1:
			Global::ReadFromStdIn = true;
			break;

		case 2:
			Global::SymbolsMustBePredefined = false;
			break;
	
		case 3:
			Global::SymbolsMustBePredefined = true;
			break;

		case '?':
			/*FALLTHRU*/
		case 4:
			Usage(argc, argv);
			return(0);

		case 5:
			Global::OutputDirectory = optarg;
			break;

		case 6:
			Global::GScope.GenerateCPP = true;
			OutputLanguageChosen = true;
			break;

		case 7:
			Global::GScope.GenerateCSharp = true;
			OutputLanguageChosen = true;
			break;

		case -1:
			More = false;
			break;
		}
	}

	if (optind < argc) {
		Global::InputFileName = argv[optind];
		Global::CurrentFileName.push_front(Global::InputFileName);
	}

	// Check for the existence of, access,
	// and create if needed, the output directory.
	//
	RiverExplorer::rmdirr(Global::OutputDirectory);
	mkdir(Global::OutputDirectory.c_str(), 0700);
	if (access(Global::OutputDirectory.c_str(), R_OK|R_OK|X_OK) != F_OK) {
		cerr << "Error: Unable to create directory: '"
				 << Global::OutputDirectory
				 << "'. Or unable to use because of file permissions."
				 << endl;
		exit(1);
	}

	// Setup the name of the log file.
	//
	if (Global::ReadFromStdIn) {
		Global::LogFileName = Global::OutputDirectory + "/stdin.log";
	} else {
		Global::LogFileName = Global::OutputDirectory + "/" + Global::InputFileName + ".log";
	}
	
	Global::Log.open(Global::LogFileName);
	chmod(Global::LogFileName.c_str(), 0600);

	const auto Now = std::chrono::system_clock::now();

	std::string ZuluTimeStamp = std::format("{:%FT%TZ}", Now);

	Global::Log << "Time Stamp: " << ZuluTimeStamp << endl;

	if (Global::GScope.GenerateCPP) {
		Global::Log << "Generating C++ code in directory: "
			+ Global::OutputDirectory + "/" + Global::CppDirName << endl;
	}
	
	if (Global::GScope.GenerateCSharp) {
		Global::Log << "Generating C# code in directory: "
			+ Global::OutputDirectory + "/" + Global::CSharpDirName << endl;
	}
	
	Driver Compiler;
		
	if (Global::ReadFromStdIn) {
			Compiler.Parse(std::cin, true);

	} else {
		if (access(Global::InputFileName.c_str(), R_OK) == F_OK) {
			Compiler.Parse(Global::InputFileName, true);
		} else {
			Global::Log << "File not found: " << Global::InputFileName << endl;
		}
	}

	if (Global::GScope.GenerateCPP || Global::GScope.GenerateCSharp) {
		OutputLanguageChosen = true;
	}
	if (!OutputLanguageChosen) {
		std::stringstream SS;

		SS
			<< ">---------------------------------------------------------------<"
			<< endl
			<< "| No coding language was selected, so no code will be generated.|"
			<< endl
			<< "| run '" << basename(argv[0]) << " --help' for options."
			<< "                            |"
			<< endl
			<< ">---------------------------------------------------------------<"
			<< endl;

		cout << SS.str();
		Global::Log << SS.str();
	}
	
	// Print out the symbol tables.
	//
	Compiler.DumpSymbolTables(Global::Log);

	// Now generate any code.
	//
	if (Global::GScope.GenerateCBOR_Client
			|| Global::GScope.GenerateCBOR_Server) {
		
		GenerateCBOR * Generator = new GenerateCBOR();

		if (Generator->Create()) {
			Global::Log << "CBOR code generated without error." << endl;
			
		} else {
			Global::Log << "CBOR code generated with errors." << endl;
		}
	}
	
	if (Global::GScope.GenerateJSON_Client
			|| Global::GScope.GenerateJSON_Server) {
		
		GenerateJSON * Generator = new GenerateJSON();

		if (Generator->Create()) {
			Global::Log << "JSON code generated without error." << endl;

		} else {
			Global::Log << "JSON code generated with errors." << endl;
		}
	}
	
	if (Global::GScope.GenerateNATIVE_Client
			|| Global::GScope.GenerateNATIVE_Server) {
		
		GenerateCPP * Generator = new GenerateCPP();

		if (Generator->Create()) {
			Global::Log << "C++ code generated without error." << endl;

		} else {
			Global::Log << "C++ code generated with errors." << endl;
		}
	}
	
	if (Global::GScope.GenerateREST_Client
			|| Global::GScope.GenerateREST_Server) {
		
		GenerateREST * Generator = new GenerateREST();

		if (Generator->Create()) {
			Global::Log << "REST code generated without error." << endl;

		} else {
			Global::Log << "REST code generated with errors." << endl;
		}
	}
	
	if (Global::GScope.GenerateXDR_Client
			|| Global::GScope.GenerateXDR_Server) {
		
		GenerateXDR * Generator = new GenerateXDR();

		if (Generator->Create()) {
			Global::Log << "XDR code generated without error." << endl;

		} else {
			Global::Log << "XDR code generated with errors." << endl;
		}
	}
	
	if (Global::GScope.GenerateXML_Client
			|| Global::GScope.GenerateXML_Server) {
		
		GenerateXML * Generator = new GenerateXML();

		if (Generator->Create()) {
			Global::Log << "XML code generated without error." << endl;

		} else {
			Global::Log << "XML code generated with errors." << endl;
		}
	}
	
	return(0);
}
