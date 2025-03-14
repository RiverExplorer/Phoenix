/**
 * Project: Phoenix
 * Time-stamp: <2025-03-12 14:18:21 doug>
 * 
 * @file GenerateComment.cpp
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
	extern std::string Indent(int Level);
	
	Comment::~Comment()
	{
		/*EMPTY*/

	}
	
	void
	Comment::PrintCppHeader(ofstream & Stream)
	{
		std::string::const_iterator SIt;

		// I do not know why (-1) works.
		//
		std::string I = Indent(IndentLevel);

		Stream << I;
		for (SIt = Name.cbegin(); SIt != Name.cend(); SIt++) {
			if (*SIt == '\t' || *SIt == '\r') {
				continue;
			}
			Stream << *SIt;
			if (*SIt == '\n') {
				Stream << I;
			}
		}
		Stream << std::endl;
		//Stream << Indent() << Name << std::endl;
	}

	void
	Comment::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	Comment::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	Comment::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	Comment::PrintAbnf(ofstream & Stream)
	{
	}
	
	void
	Comment::PrintServer(ofstream & Stream)
	{
	}
	
}
