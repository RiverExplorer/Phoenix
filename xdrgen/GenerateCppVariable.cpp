/**
 * Project: Phoenix
 * Time-stamp: <2025-03-28 23:58:22 doug>
 * 
 * @file GenerateCppVariable.cpp
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
	
	Variable::~Variable()
	{
		/*EMPTY*/

		return;
	}

	void
	Variable::PrintCppHeader(ofstream & Stream) const
	{
		PrintCppNamespaceBegin(Stream);
		string I = Indent();

		if (IsFixedArray || IsVariableArray) {
			Stream << I << "extern std::vector<" << Type;
			if (IsPointer) {
				Stream << "*>";
			} else if (IsReference) {
				Stream << "&>";
			} else {
				Stream << ">";
			}
		} else {
			Stream << I << "extern " << Type;

			if (IsPointer) {
				Stream << " * ";
			} else if (IsReference) {
				Stream << " & ";
			}
		}
		Stream << " " << Name << ";";
		if (ArraySize != "") {
			if (IsFixedArray) {
				Stream << "> // With a fixed size of: " << ArraySize << ".";

			} else if (IsVariableArray) {
				Stream << "> //With a max size of: " << ArraySize << ".";
			}
		} else {
			Stream << " // With no size limit.";
		}
		Stream << endl << endl;
		PrintCppNamespaceEnd(Stream);

		return;
	}

	void
	Variable::PrintCppHeaderXdr(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Variable::PrintCppXDR(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Variable::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Variable::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
	void
	Variable::PrintAbnf(ofstream & Stream) const
	{
		std::string Pad = std::string(Name.length()  + 3, ' ');

		if (IsVariableArray) {
			Stream << Pad << "; Variable size, up to: " << ArraySize << endl;
			
		} else if (IsFixedArray) {
			Stream << Pad << "; Fixed size, of: " << ArraySize << endl;
		}
		
		Stream << Name << " = ";

		if (IsVariableArray) {
			Stream << "*" << ArraySize;

		} else if (IsFixedArray) {
			Stream << ArraySize;
		}
		Stream << Type << endl << endl;
	}
	
	void
	Variable::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}
	
}
