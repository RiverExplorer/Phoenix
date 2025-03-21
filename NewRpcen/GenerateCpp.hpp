/**
 * Project: Phoenix
 * Time-stamp: <2025-03-21 09:47:40 doug>
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
#ifndef _RIVEREXPLORER_RPCGEN_GENERATECPP_HPP_
#define _RIVEREXPLORER_RPCGEN_GENERATECPP_HPP_

#include <string>
#include "Generate.hpp"

namespace RiverExplorer::rpcgen
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
	extern void	PrintCppNamespaceEnd(std::ofstream & Stream);
	
}
#endif // _RIVEREXPLORER_RPCGEN_GENERATECPP_HPP_
