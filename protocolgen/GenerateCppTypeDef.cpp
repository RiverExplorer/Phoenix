/**
 * Project: Phoenix
 * Time-stamp: <2025-03-30 11:35:45 doug>
 * 
 * @file GenerateCppTypeDef.cpp
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
#include "protocolgen.hpp"

namespace RiverExplorer::protocolgen
{
	using namespace std;
	
	TypeDef::~TypeDef()
	{
		/*EMPTY*/
		return;
	}
	
	void
	TypeDef::PrintCppHeader(ofstream & Stream) const
	{
		string I = Indent();

		PrintCppNamespaceBegin(Stream);
		Stream << I << "typedef ";
		Declaration->PrintCppHeader(Stream, false);
		Stream << endl << endl;
		PrintCppNamespaceEnd(Stream);
		
		return;
	}

	void
	TypeDef::PrintCppHeaderXdr(ofstream & Stream) const
	{
		/*EMPTY*/
		return;
	}
	
	void
	TypeDef::PrintCppXDR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	TypeDef::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	TypeDef::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	TypeDef::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	TypeDef::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
