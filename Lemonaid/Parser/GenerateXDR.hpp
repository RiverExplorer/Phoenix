/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATEXDR_HPP__
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATEXDR_HPP__

#include "Globals.hpp"
#include "Generate.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * This is a base class for generate
	 *
	 */
	class GenerateXDR
		: public Generate
		
	{
	public:

		virtual ~GenerateXDR() {};

		/**
		 * Generate the xdr code.
		 *
		 */

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
		virtual bool Create();

		/**
		 * Compile and build the generated code.
		 *
		 * @note
		 * Cross compiling may or might not be availiable on
		 * all platforms for all other platforms.
		 */
		virtual bool Compile();
		
		/**
		 * Create the put directory structure needed
		 * to support the protocol.
		 *
		 * @param BuildClient When true, build client side code.
		 *
		 * @param BuildServer When true, build server side code.
		 *
		 * @return true if the output tree was successfully crated.
		 *
		 * @note
		 * The shared code is always built.
		 */
		virtual bool CreateOutputTree(bool BuildClient, bool BuildServer);

		/**
		 * Generate client size code.
		 *
		 * @return true if the client size code was successfully created.
		 */
		virtual bool CreateClientSideCode();
		
		/**
		 * Generate server size code.
		 *
		 * @return true if the server size code was successfully created.
		 */
		virtual bool CreateServerSideCode();

		/**
		 * Declare variable type.
		 *
		 * @param S The symbol to create the variable type from.
		 */
		virtual bool	DeclareVariablType(Symbol * S);
		
		/**
		 * Declare variable.
		 *
		 * @param S The symbol to create the variable from.
		 */
		virtual bool	DeclareVariable(Symbol * S);
		
		/**
		 * Generate headers files, or definition, API, ... files
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteHeaders();

		/**
		 * Generate the code files.
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteCode();
		
		/**
		 * Generate any document files.
		 *
		 * A .html file will be written providing a link
		 * to variables sorted by ...
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteDocuments();
		
	private:
		
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
		virtual bool _BuildDirs();

		/**
		 * Only to be called from _BuildDirs().
		 */
		virtual bool _BuildCPPDirs();

		/**
		 * Only to be called from _BuildDirs().
		 */
		virtual bool _BuildCSharpDirs();
		
		std::string	_CppTopDir;
		std::string	_CppClientSideDir;
		std::string	_CppServerSideDir;
		std::string	_CppIncludeDir;
		std::string	_CppSharedDir;

		std::string	_CSharpTopDir;
		std::string	_CSharpClientSideDir;
		std::string	_CSharpServerSideDir;

	};
}
#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATEXDR_HPP__
