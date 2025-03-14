/**
 * Project: Phoenix
 * Time-stamp: <2025-03-12 11:03:44 doug>
 * 
 * @file GeneratePassThrough.cpp
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
	
	PassThrough::~PassThrough()
	{
		/*EMPTY*/

	}
	
	void
	PassThrough::PrintCppHeader(ofstream & Stream)
	{
		Stream << Name << std::endl;

		return;
	}

	void
	PassThrough::PrintCppXDR(ofstream & Stream)
	{
	}

	void
	PassThrough::PrintCppStubs(ofstream & Stream)
	{
	}

	void
	PassThrough::PrintXSD(ofstream & Stream)
	{
	}
	
	void
	PassThrough::PrintAbnf(ofstream & Stream)
	{
	}
	
	void
	PassThrough::PrintServer(ofstream & Stream)
	{
	}
	
}
