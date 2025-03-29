/**
 * Project: Phoenix
 * Time-stamp: <2025-03-26 18:58:30 doug>
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
#include "xdrgen.hpp"

namespace RiverExplorer::xdrgen
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
		PrintCppDeclareVariable(Stream);
		Stream << endl << endl;
		PrintCppNamespaceEnd(Stream);
		
		return;
	}

	void
	TypeDef::PrintCppHeaderXdr(ofstream & Stream) const
	{
		PrintCppNamespaceBegin(Stream);
		Stream << "bool xdr_" << Name
					 << "(XDR * xdrs, " << Name << " * obj);" << endl;
		PrintCppNamespaceEnd(Stream);
		Stream << "extern \"C\" bool xdr_" << Name
					 << "(XDR * xdrs, " << Name << " * obj) {" << endl
					 << "\treturn(" << CppNamespace << "::"
					 << Name << "(xdrs, obj));" << endl
					 << "}" << endl;
		
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
