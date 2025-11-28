/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATECPP_HPP__
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATECPP_HPP__

#include "Globals.hpp"
#include "Generate.hpp"

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * This is a base class for generate a protocol as defined
	 * by the Protocol definition file.
	 *
	 * A native implementation uses TCP and/or UDP to transfer
	 * the data. And assumes the other endpoint can accept basic
	 * data types (int, float, ...) without interpretation.
	 */
	class GenerateCPP
		: public Generate
		
	{
	public:

		virtual ~GenerateCPP() {};
		
		/**
		 * Write the files.
		 *
		 * Start here, this calls all the needed methods
		 * to create the generated code.
		 *
		 * This method checks the target language(s).
		 *
		 * @note
		 * Any implementation may or may not support all
		 * build types.
		 */
		virtual bool Create();

		/**
		 * Compile and build the generated code.
		 *
		 * @note
		 * This implementation builds and compiles on:
		 *
		 * Linux (Tested on Fedora 43).
		 * Generates code, builds compilable code for Linux, Windows, and Android.
		 *
		 * Windows (Tested on Windows 10/11).
		 * Generated code runs on this platform.
		 *
		 * Android (Tested on Galaxy and Oculus)
		 * Generated code runs on this platform.
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
		virtual bool CreateClientSideCode() const;
		
		/**
		 * Generate server size code.
		 *
		 * @return true if the server size code was successfully created.
		 */
		virtual bool CreateServerSideCode() const;

		/**
		 * Declare variable type.
		 *
		 * @param S The symbol to create the variable type from.
		 *
		 * @note
		 * This is like DeclareVariable, except it does not
		 * print the variable name.
		 *
		 * @return The variable, appropriate for the current language.
		 */
		virtual std::string	DeclareVariablType(Symbol * S) const;
		
		/**
		 * Write the default comment.
		 *
		 * @param S The symbol.
		 */
		virtual std::string DefaultComment(const Symbol * S) const;
		
		/**
		 * Write the range comment.
		 *
		 * @param S The symbol.
		 */
		virtual std::string RangeComment(const Symbol * S) const;
		
		/**
		 * Declare variable.
		 *
		 * @param S The symbol to create the variable from.
		 *
		 * @note
		 * This prints the data type, and variable name.
		 * DeclareVariableType() just prints the type,
		 * and not the variable.
		 *
		 * @return The variable, appropriate for the current language.
		 */
		virtual std::string	DeclareVariable(Symbol * S) const;
		
		/**
		 * Generate headers files, or definition, API, ... files
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteHeaders() const;

		/**
		 * Generate the code files.
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteCode() const;
		
		/**
		 * Generate any document files.
		 *
		 * A .html file will be written providing a link
		 * to variables sorted by ...
		 *
		 * @return true on success.
		 * Returns false on failure.
		 */
		virtual bool	WriteDocuments() const;
		
		/**
		 * Print the data type of the value, as a string.
		 *
		 * @param Type The symbol value type.
		 *
		 * @param Bits The number of bits defined with the type.
		 *
		 * @return The type, as a string.
		 */
		virtual std::string		SymbolType(Symbol * S) const;

		/**
		 * Print a method definition to a string;
		 *
		 * @param Symbol The symbol.
		 *
		 * @return The method as a string for a header or
		 * definition file.
		 */
		virtual std::string SymbolMethod(Symbol * S) const;
		 
		/**
		 * Print if the array type is fixed, variable,
		 * Not an array, as a string.
		 *
		 * @param A The array type to check.
		 *
		 * @return The array type, as a string.
		 */
		virtual std::string		to_string(Symbol::Array_e A) const;

		/**
		 * Print the visibility as a printed string.
		 *
		 * @return The visibility as a string.
		 */
		virtual std::string		to_string(Symbol::Visibility_e V) const;
		
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
#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_GENERATECPP_HPP__
