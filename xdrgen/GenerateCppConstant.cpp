/**
 * Project: Phoenix
 * Time-stamp: <2025-03-26 18:50:48 doug>
 * 
 * @file GenerateCppConstant.cpp
 * @author Douglas Mark Royer
 * @date 08-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "xdrgen.hpp"
#include <regex>
#include <cctype>

namespace RiverExplorer::xdrgen
{

	using namespace std;
	
	Constant::~Constant()
	{
		/*EMPTY*/

		return;
	}

	bool
	IsFloatingPoint(const std::string & Str)
	{
    const std::regex FloatRegex("^-?\\d+(\\.\\d+)?([eE][-+]?\\d+)?$");

    return std::regex_match(Str, FloatRegex);
	}
	
	void
	Constant::PrintCppHeader(ofstream & Stream) const
	{
		string I = Indent();

		PrintCppNamespaceBegin(Stream);
		// If is string...
		//
		if (Type[0] == '"') {
			Stream << I << "const char * "
						 << Name << " = " << Type << ";" << endl;

			
		} else if (Type[0] == '\'') { // Else is char...
			Stream << I << "const char "
						 << Name << " = " << Type << ";" << endl;
			
		} else if (std::all_of(Type.begin(), Type.end(), ::isdigit)) {
			// Is all digits.
			//
			Stream << I << "const int64_t "
						 << Name << " = " << Type << ";" << endl;

		} else if (IsFloatingPoint(Type)) {
			// If floating point.
			//
			Stream << I << "const float "
						 << Name << " = " << Type << ";" << endl;
		} else {
			// Assume it is a form read to use as an integer.
			Stream << I << "const int64_t "
						 << Name << " = " << Type << ";" << endl;
		}
		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Constant::PrintCppHeaderXdr(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Constant::PrintCppXDR(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Constant::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Constant::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Constant::PrintAbnf(ofstream & Stream) const
	{
		Stream << "const " << Name << " = " << Type << endl;
	}
	
	void
	Constant::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
}
