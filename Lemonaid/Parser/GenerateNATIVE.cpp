/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "GenerateNATIVE.hpp"
#include <sys/stat.h>
#include <string.h>

using namespace std;

namespace RiverExplorer::Phoenix::Protocol
{
	const char * const NATIVE_s = "NATIVE";

	const char * const HeaderText1_s =
		"/*\n"
		" * DO NOT EDIT - THIS FILE WAS GENERATED.\n"
		" * FROM:";

	const char * const HeaderText2_s =
		"\n"
		" *\n"
		" * Using RiverExplorer Protocol Generator.\n"
		" */\n";
		
	bool
	GenerateNATIVE::Create()
	{
		bool		Results = false;

		if (CreateOutputTree(Global::GScope.GenerateNATIVE_Client,
												 Global::GScope.GenerateNATIVE_Server)) {

			// Start generating code ...
			Global::Log << "Generated NATIVE/C++ tree." << endl;

			bool Headers = WriteHeaders();

			if (Headers) {
				Global::Log << "Wrote NATIVE/C++ header files." << endl;
			} else {
				Global::Log << "ERROR: Did not Write NATIVE/C++ header files."
										<< endl;
			}
		}
		
		return(Results);
	}

	bool
	GenerateNATIVE::Compile()
	{
		bool		Results = false;

		/**@todo */

		return(Results);
	}
	
	bool
	GenerateNATIVE::CreateOutputTree(bool BuildClient, bool BuildServer)
	{
		bool	Results = true;

		_BuildClientCode = BuildClient;
		_BuildServerCode = BuildServer;
 		
		_TopDir = Global::OutputDirectory + "/" + NATIVE_s;

		Results = _BuildDirs();
		
		return(Results);
	}

	bool
	GenerateNATIVE::CreateClientSideCode()
	{
		/**@todo*/
		return(false);
	}

	bool
	GenerateNATIVE::CreateServerSideCode()
	{
		/**@todo*/
		return(false);
	}


	bool
	GenerateNATIVE::_BuildCPPDirs()
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
				if (Global::GScope.GenerateNATIVE_Client) {
					const char * const C = "Client";
					
					if (!_MkDir(_CppTopDir, C)) {
						GotError = true;
					} else {
						_CppClientSideDir = _CppTopDir + "/" + C;
					}
				}

				if (!GotError) {
					// Create C++/Server directory.
					//
					if (Global::GScope.GenerateNATIVE_Server) {
						const char * const S = "Server";

						if (!_MkDir(_CppTopDir, S)) {
							GotError = true;
						} else {
							_CppServerSideDir = _CppTopDir + "/" + S;
						}							
					}
				}

				if (!GotError) {
					const char * const I = "include";
					
					// Create C++/include directory.
					//
					if (!_MkDir(_CppTopDir, I)) {
						GotError = true;
						
					} else {
						_CppIncludeDir = _CppTopDir + "/" + I;

						// Create C++/Shared directory.
						//
						const char * const S = "Shared";
						
						if (!_MkDir(_CppTopDir, S) != 0) {
							GotError = true;
						} else {
							_CppSharedDir = _CppTopDir + S;
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
	GenerateNATIVE::_BuildCSharpDirs()
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
				if (Global::GScope.GenerateNATIVE_Client) {
					if (!_MkDir(_CSharpTopDir, "Client")) {
						GotError = true;
					} else {
						_CSharpClientSideDir = _CppTopDir + "/" + "Client";
					}						
				}

				if (!GotError
						&& Global::GScope.GenerateNATIVE_Server) {

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
	GenerateNATIVE::_BuildDirs()
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
						if (Global::GScope.GenerateCPP
								&& (Global::GScope.GenerateNATIVE_Client
										|| Global::GScope.GenerateNATIVE_Server)) {
							if (!_BuildCPPDirs()) {
								GotError = true;
							}
						}

						// Build babby build, C#
						//
						if (Global::GScope.GenerateCSharp
								&& (Global::GScope.GenerateNATIVE_Client
										|| Global::GScope.GenerateNATIVE_Server)) {

							if (!_BuildCSharpDirs()) {
								GotError = true;
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
	GenerateNATIVE::DeclareVariablType(Symbol * S)
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}

	bool
	GenerateNATIVE::DeclareVariable(Symbol * S)
	{
		bool Results = false;


		return(Results);
	}

	bool
	GenerateNATIVE::WriteHeaders()
	{
		bool Results = false;

		if (Global::GScope.GenerateCPP) {
			uint8_t	TabLevel = 0;
			
			// Open header file.
			//
			string InputFile = Global::CurrentFileName.front();
			string HeaderFile = InputFile + ".hpp";

			string HeaderPath = "Generated/NATIVE/C++/include/"
				+ HeaderFile;
			
			ofstream Header(HeaderPath);

			if (Header.is_open()) {

				Header << HeaderText1_s
							 << InputFile
							 << HeaderText2_s;

				string If = ToIfDef(InputFile);
				string IfDef = "__RIVEREXPLORER__"
					+ If
				  + "_HPP__";

				Header << "#ifndef " << IfDef << endl;
				Header << "#define " << IfDef << endl
							 << endl;

				// Create a list, ordered by namespace.
				//
				
				//
				Symbol * S;

				//
				//for (auto const & Pair : Table._Symbols) {
				//S = Pair.second;
				//}

				Header << "#endif // " << IfDef << endl;
				Header.close();
				Results = true;
			}
		}

		return(Results);
	}

	bool
	GenerateNATIVE::WriteCode()
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
	bool
	GenerateNATIVE::WriteDocuments()
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
}
