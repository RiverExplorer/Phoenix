/**
 * Project: Phoenix
 * Time-stamp: <2025-03-27 18:55:00 doug>
 * 
 * @file GenerateCppMethod.cpp
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

namespace RiverExplorer::xdrgen
{
	using namespace std;

	Method::Method(Struct & Parent)
		: Parent(Parent)
	{
		/*EMPTY*/
		return;
	}
	
	Method::~Method()
	{
		/*EMPTY*/
		return;
	}
	
	void
	Method::PrintCppHeader(ofstream & Stream) const
	{
		std::vector<string>::const_iterator SIt;

		// Get a string set to the current indentation level.
		//
		string I = Indent();

		Stream << I << Type << " " << Name << "(";

		bool PrintedOne = false;
		bool PrintingType = true;

		for (string S : Parameters) {
			if (S == ",") {
				Stream << ", ";
			} else if (S == "*" || S == "&") {
				Stream << " " << S << " ";
			} else {
				if (!PrintedOne) {
					Stream << S;
					PrintedOne = true;
				} else {
					Stream << " " << S;
				}
			}
		}
		Stream << ");" << endl << endl;

		return;
	}

	void
	Method::PrintCppHeaderXdr(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Method::PrintCppXDR(ofstream & /*Stream*/) const
	{
		/**@todo*/

		return;
	}

	void
	Method::PrintCppStubs(ofstream & Stream) const
	{
		std::vector<string>::const_iterator SIt;

		// Get a string set to the current indentation level.
		//
		string I = Indent();
		string I2 = Indent(IndentLevel + 1);
		bool PrintedOne = false;

		Stream << I << Type;

		if (IsPointer) {
			Stream << " * ";
		} else if (IsReference) {
			Stream << " & ";
		} else {
			Stream << " ";
		}

		Stream << Parent.Name << "::" << Name << "(";
		
		for (string S : Parameters) {
			if (S == ",") {
				Stream << ", ";
			} else if (S == "*" || S == "&") {
				Stream << " " << S << " ";
			} else {
				if (!PrintedOne) {
					Stream << S;
					PrintedOne = true;
				} else {
					Stream << " " << S;
				}
			}
		}
		Stream << ")" << endl;
		Stream << I << "{" << endl;
		if (Type != "void") {
			Stream << I2 << "'" << Type << "' Results;" << endl;
		}
		Stream << endl;
		Stream << I2 << "/**@todo write this code */" << endl;
		Stream << endl;
		if (Type != "void") {
			Stream << I2 << "return(Results);" << endl;
		} else {
			Stream << I2 << "return;" << endl;
		}
		Stream << I  << "}" << endl << endl;
	}

	void
	Method::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Method::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	Method::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
		
}
