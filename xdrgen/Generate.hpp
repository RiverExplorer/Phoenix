/**
 * Project: Phoenix
 * Time-stamp: <2025-03-24 22:22:46 doug>
 * 
 * @file Generate.hpp
 * @author Douglas Mark Royer
 * @date 10-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

#include <iostream>
#include <fstream>

namespace RiverExplorer::xdrgen
{
	/**
	 * Make the path, or print error if it can not
	 * be made or does not exist.
	 *
	 * @param PathToMake The path to make.
	 * Does nothing if the path already exists.
	 */
	extern bool	MakePath(const std::string PathToMake);
	
	/**
	 * Convert an XDR namespace, to a include path.
	 */
	extern std::string NamespaceToIncludePath();
	
	extern std::string RemoveFileExtension(std::string FileName);
	
	extern bool IsBuiltInXdrType(const std::string & Type);
	
	extern std::string	Indent();
	extern std::string	Indent(int Level);
	extern std::string	ToUpper(const std::string & In);
	
	extern void	GenerateThisFileWasGenerated(std::string Prefix,
																					 std::ofstream & Stream);

	extern void	GenerateEditThisFile(std::string Prefix,
																	 std::ofstream & Stream);
	
}
