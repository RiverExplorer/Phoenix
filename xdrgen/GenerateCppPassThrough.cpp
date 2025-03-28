/**
 * Project: Phoenix
 * Time-stamp: <2025-03-26 18:53:31 doug>
 * 
 * @file GenerateCppPassThrough.cpp
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
	extern std::string Indent(int Level);
	
	PassThrough::~PassThrough()
	{
		/*EMPTY*/
		return;
	}
	
	void
	PassThrough::PrintCppHeader(ofstream & Stream) const
	{
		Stream << Name << std::endl;

		return;
	}

	void
	PassThrough::PrintCppHeaderXdr(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintCppXDR(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	PassThrough::PrintCppStubs(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}

	void
	PassThrough::PrintXSD(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintAbnf(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
	void
	PassThrough::PrintServer(ofstream & /*Stream*/) const
	{
		/**@todo*/
		return;
	}
	
}
