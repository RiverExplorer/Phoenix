/**
 * Project: Phoenix
 * Time-stamp: <2025-03-24 22:29:14 doug>
 * 
 * @file GenerateMethod.cpp
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
		: ParentStruct(Parent)
	{
	}
	
	Method::~Method()
	{
	}
	
	void
	Method::PrintCppHeader(ofstream & Stream)
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
	Method::PrintCppXDR(ofstream & /*Stream*/)
	{
		/*EMPTY*/

		return;
	}

	void
	Method::PrintCppStubs(ofstream & Stream)
	{
		std::vector<string>::const_iterator SIt;

		// Get a string set to the current indentation level.
		//
		string I = Indent();
		string I2 = Indent(IndentLevel + 1);
		bool PrintedOne = false;

		Stream << I << Type << " " << ParentStruct.Name << "::" << Name << "(";
		
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
		Stream << I2 << Type << " Results;" << endl;
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
	Method::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	Method::PrintAbnf(ofstream & Stream)
	{
	}
	
	void
	Method::PrintServer(ofstream & Stream)
	{
	}
		
}
