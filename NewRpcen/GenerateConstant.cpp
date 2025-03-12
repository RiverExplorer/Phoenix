/**
 * Project: Phoenix
 * Time-stamp: <2025-03-12 14:10:31 doug>
 * 
 * @file GenerateConstant.cpp
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
	
	Constant::~Constant()
	{
	}
	
	void
	Constant::PrintCppHeader(ofstream & Stream)
	{
		string I = Indent();

		string  TypeToUse = Type;
		bool FixedString = false;
		
		if (TypeToUse.starts_with("'")) {
			if (TypeToUse.length() > 3) {
				TypeToUse[0] = '"';
				TypeToUse[TypeToUse.length() - 1] = '"';
				FixedString = true;
			}
		}
		Stream << "#define " << Name << " " << TypeToUse;

		if (FixedString) {
			Stream << "  ; Had single quotes ('), changed to (\").";
		}
		Stream << endl;
		
	}

	void
	Constant::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	Constant::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	Constant::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	Constant::PrintAbnf(ofstream & Stream)
	{
		Stream << Name << " = " << Type << endl;
	}
	
	void
	Constant::PrintServer(ofstream & Stream)
	{
	}
	
}
