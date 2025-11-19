/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "GenerateXDR.hpp"
#include <sys/stat.h>
#include <string.h>

namespace RiverExplorer::Phoenix::Protocol
{
	const char * const XDR_s = "XDR";
	
	bool
	GenerateXDR::Create()
	{
		bool		Results = false;

		/**@todo */

		return(Results);
	}

	bool
	GenerateXDR::Compile()
	{
		bool		Results = false;

		/**@todo */

		return(Results);
	}
	
	bool
	GenerateXDR::CreateOutputTree(bool BuildClient, bool BuildServer)
	{
		bool	Results = true;

		_BuildClientCode = BuildClient;
		_BuildServerCode = BuildServer;
		
		_TopDir = Global::OutputDirectory + "/" + XDR_s;

		_BuildDirs();
		
		return(Results);
	}

	bool
	GenerateXDR::CreateClientSideCode() const
	{
		/**@todo*/
		return(false);
	}

	bool
	GenerateXDR::CreateServerSideCode() const
	{
		/**@todo*/
		return(false);
	}


	bool
	GenerateXDR::_BuildCPPDirs()
	{
		bool Results;

		bool GotError = false;
		
		if (Global::GScope.GenerateCPP) {

			if (!_MkDir(_TopDir, Global::CppDirName)) {
				GotError = true;

			} else {
				_CppTopDir = _TopDir + "/" + Global::CppDirName;

				// Create C++/Client directory.
				//
				if (!_MkDir(_CppTopDir, "Client")) {
					GotError = true;

				} else {
					_CppClientSideDir = _CppTopDir + "/" + "Client";

					// Create C++/Server directory.
					//
					if (!_MkDir(_CppTopDir, "Server")) {
						GotError = true;

					} else {
						_CppServerSideDir = _CppTopDir + "/" + "Server";

						// Create C++/include directory.
						//
						if (!_MkDir(_CppTopDir, "include")) {
							GotError = true;

						} else {
							_CppIncludeDir = _CppTopDir + "/" + "include";

							// Create C++/Shared directory.
							//
							if (!_MkDir(_CppTopDir, "Shared") != 0) {
								GotError = true;
							} else {
								_CppSharedDir = _CppTopDir + "Shared";
							}
						}
					}
				}
			}
		}

		if (GotError) {
			Results = false;
		} else {
			Results = true;
		}
		return(Results);
	}
	
	bool
	GenerateXDR::_BuildCSharpDirs()
	{
		bool Results;

		bool GotError = false;
		
		if (Global::GScope.GenerateCSharp) {

			if (!_MkDir(_TopDir, Global::CSharpDirName)) {
				GotError = true;

			} else {
				_CSharpTopDir = _TopDir + "/" + Global::CSharpDirName;

				// Create C#/Client directory.
				//
				if (!_MkDir(_CSharpTopDir, "Client")) {
					GotError = true;

				} else {
					_CSharpClientSideDir = _CppTopDir + "/" + "Client";

					// Create C#/Server directory.
					//
					if (!_MkDir(_CSharpTopDir, "Server")) {
						GotError = true;

					} else {
						_CSharpServerSideDir = _CppTopDir + "/" + "Server";
					}
				}
			}
		}

		if (GotError) {
			Results = false;
		} else {
			Results = true;
		}
		return(Results);
	}
	
	bool
	GenerateXDR::_BuildDirs()
	{
		bool Results;

		bool GotError = false;
			
		if (_TopDir.length() > 0) {
			if (_BuildClientCode || _BuildServerCode) {
				if (Global::GScope.GenerateCPP || Global::GScope.GenerateCSharp) {

					// Make the top most directory used
					// by all languages and log files.
					//
					if (!mkdir(_TopDir.c_str(), 0700) == 0) {
						Global::Log << "ERROR: Can not create top level output directory:"
								<< _TopDir
								<< "Errno: " << strerror(errno)
								<< "\n";
						GotError = true;

					} else {
						// Build babby build, C++
						//
						if (!_BuildCPPDirs()) {
							GotError = true;
						}

						// Build babby build, C#
						//
						if (!_BuildCSharpDirs()) {
							GotError = true;
						}
					}
				}
			}
		}

		if (GotError) {
			Results = false;
		} else {
			Results = true;
		}
		return(Results);
	}

	std::string
	GenerateXDR::DeclareVariablType(Symbol * S) const
	{
		std::string Results;

		/**@todo */

		return(Results);
	}
		
	std::string
	GenerateXDR::DeclareVariable(Symbol * S) const
	{
		std::string Results;

		/**@todo */

		return(Results);
	}

	bool
	GenerateXDR::WriteHeaders() const
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}

	bool
	GenerateXDR::WriteCode() const
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
	bool
	GenerateXDR::WriteDocuments() const
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
	std::string
	GenerateXDR::to_string(Symbol::Array_e A) const
	{
		std::string Results;
		
		switch (A) {

		case Symbol::NotArray_t:
			Results = "Not an array";
			break;

		case Symbol::FixedArray_t:
			Results = "Fixed Array";
			break;

			
		case Symbol::VariableArray_t:
			Results = "Variable Array";
			break;
		}
		
		return(Results);
	}
	
	std::string
	GenerateXDR::to_string(Symbol::Visibility_e V) const
	{
		std::string Results;
		
		switch (V) {

		case Symbol::Public_t:
			Results = "public:";
			break;

		case Symbol::Protected_t:
			Results = "protected:";
			break;

		case Symbol::Internal_t:
			/**
			 * C++ does not have an 'internal' visibility.
			 * So we enforce it in the generated code.
			 */
			Results = "";
			break;

		case Symbol::Private_t:
			Results = "private:";
			break;
		}

		return(Results);
	}
	
	std::string
	GenerateXDR::SymbolType(Symbol * S) const
	{
		std::string Results = "Unknown";

		switch (S->Type) {

		case Symbol::uint_t:
			if (S->Bits == 0) {
				Results = "uint64_t";
				
			} else if (S->Bits == 1) {
				Results = "bool";

			} else if (S->Bits <=8) {
				Results = "uint8_t";
				
			} else if (S->Bits <= 16) {
				Results = "uint16_t";

			} else if (S->Bits <= 32) {
				Results = "uint64_t";

			} else {
				Results = "ERROR - up to 64 bits supported!";
			}
			break;
			
		case Symbol::sint_t:
			if (S->Bits == 0) {
				Results = "int64_t";
				
			} else if (S->Bits == 1) {
				Results = "bool";

			} else if (S->Bits <=8) {
				Results = "int8_t";
				
			} else if (S->Bits <= 16) {
				Results = "int16_t";

			} else if (S->Bits <= 32) {
				Results = "int64_t";

			} else {
				Results = "ERROR - up to 64 bits supported!";
			}
			break;
			
		case Symbol::float_t:
			if (S->Bits == 0) {
				Results = "long double";

			} else if (S->Bits <= 32) {
				Results = "float";

			} else if (S->Bits <= 64) {
				Results = "double";

			} else {
				/**
				 * @todo - check at runtime = length??
				 */
				Results = "long double";
			}
			break;
			
		case Symbol::string_t:
			Results = "std::string";
			break;
			
		case Symbol::class_t:
			Results = S->ID;
			break;
			
		case Symbol::void_t:
			Results = "void";
			break;
			
		case Symbol::method_t:
			Results = SymbolMethod(S);
			break;

		case Symbol::opaque_t:
			/**
			 * It will be a fixed or variable array.
			 */
			Results = "uint8_t";
			break;		
		}

		return(Results);
	}
	
	std::string
	GenerateXDR::SymbolMethod(Symbol * S) const
	{
		std::string Results;

		/** @todo */

		return(Results);
	}
	
	std::string
	GenerateXDR::RangeComment(const Symbol * S) const
	{
		std::string Results;

		/** @todo */

		return(Results);
	}

	std::string
	GenerateXDR::DefaultComment(const Symbol * S) const
	{
		std::string Results;

		/** @todo */

		return(Results);
	}
}
