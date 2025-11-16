/**
 * Project: Phoenix
 * Time-stamp: <2025-03-26 19:07:25 doug>
 * 
 * @file GenerateCpp.hpp
 * @author Douglas Mark Royer
 * @date 24-FEB-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#ifndef _RIVEREXPLORER_XDRGEN_GENERATECPP_HPP_
#define _RIVEREXPLORER_XDRGEN_GENERATECPP_HPP_

#include <string>
#include "Generate.hpp"

namespace RiverExplorer::protocolgen
{
	extern std::string Namespace;
	extern std::string CppNamespace;
	
	/**
	 * Convert the XDR type, to a C++ type.
	 *
	 * @param The XDR type.
	 *
	 * @return The C++ type.
	 */
	extern std::string ToCppType(const std::string & In);

	/**
	 * Convert an XDR namespace to a C++ namespace.
	 */
	extern std::string NamespaceToCppNamespace();

	extern void	GenerateSharedHpp(const std::string & InDirectory);

	extern void	PrintCppNamespaceBegin(std::ofstream & Stream);

	extern void	PrintCppNamespaceBegin(std::ofstream & Stream,
																		 const std::string & NsName);
	
	extern void	PrintCppNamespaceEnd(std::ofstream & Stream);

	extern void	PrintCppNamespaceEnd(std::ofstream & Stream,
																	 const std::string & NsName);
	
}
#endif // _RIVEREXPLORER_XDRGEN_GENERATECPP_HPP_
