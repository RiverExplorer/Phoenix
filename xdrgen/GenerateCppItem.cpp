/**
 * Project: Phoenix
 * Time-stamp: <2025-03-27 10:35:06 doug>
 * 
 * @file GenerateCppItem.cpp
 * @author Douglas Mark Royer
 * @date 10-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "xdrgen.hpp"
#include "Generate.hpp"
#include "GenerateCpp.hpp"
#include <unistd.h>
#include <fcntl.h>

using namespace std;

namespace RiverExplorer::xdrgen
{
	void
	Item::PrintCppDeclareVariable(ofstream & Stream) const
	{
		string I = Indent();

		if (IsFixedArray || IsVariableArray) {
			if (Type == "opaque") {
				if (IsPointer) {
					Stream << I << "std::vector<uint8_t*> ";
				} else {
					Stream << I << "std::vector<uint8_t> ";
				}
					
			} else if (Type == "string") {
				Stream << I << "std::string ";

				if (IsPointer) {
					Stream << "* ";
				}
			} else {
				Stream << I << "std::vector<" << ToCppType(Type);
				if (IsPointer) {
					Stream << "*";
				}
				Stream << "> ";
			}
			Stream << Name << ";";

			if (ArraySize != "") {
				Stream << I;
				if (IsFixedArray) {
					Stream << endl << I << "// With a fixed size of: " << ArraySize;
				} else if (IsVariableArray) {
					Stream << endl << I << "// With a max size of: " << ArraySize;
				}
			} else {
				if (IsVariableArray) {
					Stream << endl << I << "// With no size limit.";
				}
			}
		} else {
			std::string CppType = ToCppType(Type);

			if (IsFixedArray || IsVariableArray) {
				Stream << I << "std::vector<" << CppType;
				if (IsPointer) {
					Stream << "*";
				}
				Stream << ">";
			} else {
				Stream << I << CppType;
			}
			if (IsPointer) {
				Stream << " * ";

			} else	if (IsReference) {
				Stream << " & ";

			} else {
				Stream << " ";
			}
			Stream << Name << ";";
		}

		return;
	}
	
}
