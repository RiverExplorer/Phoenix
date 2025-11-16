/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "Globals.hpp"
#include "Driver.hpp"

#include <cctype>
#include <fstream>
#include <cassert>

namespace RiverExplorer::Phoenix::Protocol
{
	bool Driver::VerboseDebug = false;
	std::ofstream * Driver::_LogFile = nullptr;
	
	char * Driver::_FileName = nullptr;
	
	Driver::~Driver()
	{
		if (_Scanner != nullptr) {
			delete(_Scanner);
			_Scanner = nullptr;
		}

		if (_Parser != nullptr) {
			delete(_Parser);
			_Parser = nullptr;
		}

		return;
	}

	void
	Driver::Parse(const std::string & FileName, bool DebugOn)
	{
		std::ifstream In(FileName);

		_FileName = strdup(FileName.c_str());

		if (!In.good()) {
			exit(EXIT_FAILURE);
		}

		_ParseHelper(In, DebugOn);

		return;		
	}

	void
	Driver::Parse(std::istream & In, bool DebugOn)
	{
		if (!In.good() && In.eof()) {
			return;
		}

		_ParseHelper(In, DebugOn);

		return;
	}

	void
	Driver::_ParseHelper(std::istream & In, bool DebugOn)
	{
		if (_Scanner != nullptr) {
			delete _Scanner;
		}

		try {
			_Scanner = new Scanner(&In);

		} catch (std::bad_alloc & Ex) {

			std::cerr << "Failed to allocate scanner: ("
								<< Ex.what()
								<< "), exiting\n"
								<< std::endl;
			exit(EXIT_FAILURE);
		}

		if (_Parser != nullptr) {
			delete _Parser;
		}

		try {
			_Parser = new Parser(*_Scanner, *this);

			if (DebugOn) {
				_DebugOn();
			}

		} catch (std::bad_alloc & Ex) {

			std::cerr << "Failed to allocate parser: ("
								<< Ex.what()
								<< "), exiting\n"
								<< std::endl;
			exit(EXIT_FAILURE);
		}

		const int Accept(0);
		
		if (_Parser->parse() != Accept) {
			std::cerr << "Parse Failed." << std::endl;
		}

		return;
	}

	void
	Driver::_DebugOn()
	{
		_Parser->set_debug_level(1);
		_Parser->set_debug_stream(std::cerr);

		return;
	}

	std::ofstream *
	Driver::OpenLog()
	{
		_LogFile = new std::ofstream(Global::LogFileName);

		return(_LogFile);
	}
 
	void
	Driver::DumpSymbolTables(std::ostream & Out)
	{
		Scope * GlobalScope = CurrentProtocol.GlobalScope;

		Out << GlobalScope->Symbols;

		return;		
	}
	
}

