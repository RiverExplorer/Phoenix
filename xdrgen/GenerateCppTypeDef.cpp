/**
 * Project: Phoenix
 * Time-stamp: <2025-03-24 22:20:48 doug>
 * 
 * @file GenerateTypeDef.cpp
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
	
	TypeDef::~TypeDef()
	{
	}
	
	void
	TypeDef::PrintCppHeader(ofstream & Stream)
	{
		string I = Indent();

		PrintCppNamespaceBegin(Stream);
		Stream << I << "typedef ";
		PrintCppDeclareVariable(Stream);
		Stream << endl << endl;
		PrintCppNamespaceEnd(Stream);
		
		return;
	}

	void
	TypeDef::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	TypeDef::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	TypeDef::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	TypeDef::PrintAbnf(ofstream & Stream)
	{
	}
	
	void
	TypeDef::PrintServer(ofstream & Stream)
	{
	}
	
}
