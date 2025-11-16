/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "GenerateREST.hpp"
#include <sys/stat.h>
#include <string.h>

namespace RiverExplorer::Phoenix::Protocol
{
	const char * const REST_s = "REST";
	
	bool
	GenerateREST::Create()
	{
		bool		Results = false;

		/**@todo */

		return(Results);
	}

	bool
	GenerateREST::Compile()
	{
		bool		Results = false;

		/**@todo */

		return(Results);
	}
	
	bool
	GenerateREST::CreateOutputTree(bool BuildClient, bool BuildServer)
	{
		bool	Results = true;

		_BuildClientCode = BuildClient;
		_BuildServerCode = BuildServer;
		
		_TopDir = Global::OutputDirectory + "/" + REST_s;

		_BuildDirs();
		
		return(Results);
	}

	bool
	GenerateREST::CreateClientSideCode()
	{
		/**@todo*/
		return(false);
	}

	bool
	GenerateREST::CreateServerSideCode()
	{
		/**@todo*/
		return(false);
	}


	bool
	GenerateREST::_BuildCPPDirs()
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
	GenerateREST::_BuildCSharpDirs()
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
	GenerateREST::_BuildDirs()
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

	bool
	GenerateREST::DeclareVariablType(Symbol * S)
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
		
	bool
	GenerateREST::DeclareVariable(Symbol * S)
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}

	bool
	GenerateREST::WriteHeaders()
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}

	bool
	GenerateREST::WriteCode()
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
	bool
	GenerateREST::WriteDocuments()
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
}
