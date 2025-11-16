/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATE_HPP__
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATE_HPP__

#include "Globals.hpp"
#include "SymbolTable.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * This is a base class for generate
	 *
	 */
	class Generate
	{
	public:

		virtual ~Generate() {};

		/**
		 * Write the files.
		 *
		 * Start here, this calls all the needed methods
		 * to create the generated code.
		 *
		 * This method checks the target languge(s) and builds
		 * the code.
		 *
		 * @note
		 * Any implemenetation may or may not support all
		 * build types.
		 */
		virtual bool Create() = 0;

		/**
		 * Compile and build the generated code.
		 *
		 * @note
		 * Cross compiling may or might not be availiable on
		 * all platforms for all other platforms.
		 */
		virtual bool Compile() = 0;
		
	protected:
		
		/**
		 * Create the put directory structure needed
		 * to support the protocol.
		 *
		 * The tree format is (example using XDR):
		 *
		 *                         XDR
		 *                          |
		 *                       ---------
		 *                     C++       C#
		 *                      |         |
		 *       ------ ------ ------   ------ ------ ------
		 *       Shared  Client Server  Shared Client Server
		 *          |       |      |       |      |      |
		 *    incude  ...  ...    ...     ...    ...    ...
		 *       .     .    .      .       .      .      .
		 *       .     .    .      .       .      .      .
		 *
		 * Then below this, the suggestion is to make each unique
		 * namespace path, a directory, much like the Java com/company/...
		 * directory structure. This is not for readability, it is
		 * to aid automated processes.
		 *
		 * Under C++/Shared is 
		 * @param BuildClient When true, build client side code.
		 *
		 * @param BuildServer When true, build server side code.
		 *
		 * @return true if the output tree was successfully crated.
		 *
		 * @note
		 * The shared code is always built.
		 */
		virtual bool CreateOutputTree(bool BuildClient, bool BuildServer) = 0;

		/**
		 * Generate client size code.
		 *
		 * @return true if the client size code was successfully created.
		 */
		virtual bool CreateClientSideCode() = 0;
		
		/**
		 * Generate server size code.
		 *
		 * @return true if the server size code was successfully created.
		 */
		virtual bool CreateServerSideCode() = 0;

		/**
		 * Declare variable type.
		 *
		 * @param S The symbol to create the variable type from.
		 */
		virtual bool	DeclareVariablType(Symbol * S) = 0;
		 
		/**
		 * Declare variable.
		 *
		 * @param S The symbol to create the variable from.
		 */
		virtual bool	DeclareVariable(Symbol * S) = 0;
		
		/**
		 * Generate headers files, or definition, API, ... files
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteHeaders() = 0;

		/**
		 * Generate the code files.
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteCode() = 0;
		
		/**
		 * Generate any document files.
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteDocuments() = 0;

		/**
		 * First, set _TopDir, _BuildingClientCode, and _BuildingServerCode.
		 * And one or both of BuildingCPP and BuildingCSharp must be set.
		 *
		 * Then call BuildDirs().
		 *
		 * Then it will create the directories and the generated
		 * tree structures.
		 *
		 * @return true of successful.
		 */
		virtual bool _BuildDirs() = 0;

		/**
		 * Only to be called from _BuildDirs().
		 */
		virtual bool _BuildCPPDirs() = 0;

		/**
		 * Only to be called from _BuildDirs().
		 */
		virtual bool _BuildCSharpDirs() = 0;
		
		/**
		 * Only to be called from _BuildDirs().
		 */
		bool _MkDir(std::string & Parent, std::string Child);

		/**
		 * Turn a file name into part of the #ifndef/#define/#endif
		 * name used in header files. Replace dots with underscore.
		 *
		 * @param FileName The file name to use.
		 */
		std::string ToIfDef(const std::string & FileName);

		/**
		 * Get the C++ namespace name for the symbol.
		 *
		 * @param TargetSymbol The Symbol to get the C++ namespace of.
		 *
		 * @return The C++ namespace of the symbol.
		 * An empty string means it is without a namespace.
		 */
		std::string CppNamespace(const Symbol & TargetSymbol);
		 
		/**
		 * Get the C# namespace name for the symbol.
		 *
		 * @param TargetSymbol The Symbol to get the C++ namespace of.
		 *
		 * @return The C# namespace of the symbol.
		 * An empty string means it is without a namespace.
		 */
		std::string CSharpNamespace(const Symbol & TargetSymbol);

		/**
		 * Output the specified number of tabs.
		 *
		 * @parm NumberTabs The number of tabs (>= 0).
		 *
		 * @retun NumberTabs tab (\t) characters.
		 */
		static std::string Tabs(uint8_t NumberTabs);
		
		/**
		 * When true, build the client side code.
		 */
		bool _BuildClientCode;

		/**
		 * When true, build the server side code.
		 */
		bool _BuildServerCode;

		/**
		 * The top of the generate tree.
		 */
		std::string	_TopDir;

	};
}
#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATE_HPP__
