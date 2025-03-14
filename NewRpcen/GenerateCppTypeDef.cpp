/**
 * Project: Phoenix
 * Time-stamp: <2025-03-12 16:06:15 doug>
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
#include "rpcgen.hpp"

namespace RiverExplorer::rpcgen
{
	using namespace std;
	
	TypeDef::~TypeDef()
	{
	}
	
	void
	TypeDef::PrintCppHeader(ofstream & Stream)
	{
		string I = Indent();

		Stream << I << "typedef ";
		PrintCppDeclareVariable(Stream);
		Stream << endl << endl;
		
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
