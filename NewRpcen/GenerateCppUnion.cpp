/**
 * Project: Phoenix
 * Time-stamp: <2025-03-21 09:53:13 doug>
 * 
 * @file GenerateUnion.cpp
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
#include "rpcgen.hpp"
#include "GenerateCpp.hpp"

using namespace std;

namespace RiverExplorer::rpcgen
{

	Union::Union()
	{
		Default = nullptr;

		return;
	}

	Union::~Union()
	{
		if (Default != nullptr) {
			delete Default;
			Default = nullptr;
		}

		return;
	}
	
	void
	Union::PrintCppHeader(ofstream & Stream)
	{
		PrintCppNamespaceBegin(Stream);
		Stream << endl;
		
		string I = Indent();

		Stream << I << "/**" << endl;
		Stream << I << " * @class " << Name
					 << " \"" << Name << ".hpp\" <";

		if (Namespace != "") {
			Stream << NamespaceToIncludePath() << "/";
		}
		Stream << Name << ".hpp>"
					 << endl;
		Stream << I << " * Generated from: union " << Name << endl;
		Stream << I << " */" << endl;
		
		Stream << I << "class " << Name << endl;
		Stream << I << "{" << endl;
		Stream << I << "public:" << endl;
		IndentLevel++;

		string I2 = Indent();
		Stream << I2 << Name << "();" << endl;
		Stream << endl;
		Stream << I2 << "virtual ~" << Name << "();" << endl;
		Stream << endl;
		Stream << I2 << "/**" << endl;
		Stream << I2 << " * Serialize or deserialize a " << Name << " object." << endl;
		Stream << I2 << " * " << endl;
		Stream << I2 << " * @param Xdr An XDR object, initalzied with" << endl;
		Stream << I2 << " * XDR_ENCODE, XDR_DECODE or XDR_FREE." << endl;
		Stream << I2 << " * " << endl;
		Stream << I2 << " * @return true on no errors." << endl;
		Stream << I2 << " */" << endl;
		Stream << I2 << "bool Xdr(XDR & Xdr);" << endl;
		Stream << endl;

		Stream << I2 << "/**" << endl;
		Stream << I2 << " * " << SwitchVariable
					 << " tells you the which union content to use." << endl;
		Stream << I2 << " */" << endl;
		Stream << I2 << SwitchType << " " << SwitchVariable << ";" << endl;
		
		vector<Item*>::const_iterator CIt;
		UnionCase * Case;

		IndentLevel++;
		string I3 = Indent();

		Stream << endl;
		Stream << I2 << "union {" << endl;
		IndentLevel++;
		string I4 = Indent();
		
		for (CIt = Cases.begin(); CIt != Cases.end(); CIt++) {

			if (*CIt != nullptr) {
				Case = dynamic_cast<UnionCase*>(*CIt);

				if (Case != nullptr) {
					if (Case->Type == "void") {
						Stream << endl << I3
								 << "// " << Case->CaseValue << " has void data." << endl;
					} else {
						Stream << endl;
						Case->PrintCppDeclareVariable(Stream);
						if (Case->CaseValue == "default") {
							Stream << endl << I3
										 << "// This is the DEFAULT variable, use this when"
										 << endl << I3
										 << "// " << SwitchVariable
										 << " (" << SwitchType << ")"
										 << " does not equal any other value."
										 << endl << I3
										 << "// listed here."
										 << endl;
							Stream << endl;
						} else {
							Stream << endl << I3 << "// Use when " << SwitchVariable
										 << " (" << SwitchType << ")"
										 << " == "
										 << Case->CaseValue
										 << "."
										 << endl;
						}
						Stream << endl;
					}
				} else {
					(*CIt)->PrintCppHeader(Stream);
				}
			}
		}
		if (Default != nullptr) {
			Default->PrintCppDeclareVariable(Stream);
			Stream << endl;
		}
		IndentLevel--;
		Stream << I2 << "};" << endl;

		Stream << endl;
		Stream << "}; // End class " << Name << endl;

		PrintCppNamespaceEnd(Stream);
		return;
	}
	
	void
	Union::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	Union::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	Union::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	Union::PrintAbnf(ofstream & Stream)
	{
	}
	
	void
	Union::PrintServer(ofstream & Stream)
	{
	}

	UnionCase::~UnionCase()
	{
	}
	
	void
	UnionCase::PrintCppHeader(ofstream & Stream)
	{
	}
	
	void
	UnionCase::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	UnionCase::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	UnionCase::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	UnionCase::PrintAbnf(ofstream & Stream)
	{
	}
	
	void
	UnionCase::PrintServer(ofstream & Stream)
	{
	}
	
}
