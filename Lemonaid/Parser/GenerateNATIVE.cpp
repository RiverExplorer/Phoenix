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
#include <sstream>
#include <string>

using namespace std;

namespace RiverExplorer::Phoenix::Protocol
{

	/**
	 * The current namespace being printed.
	 */
	static std::string CurrentNamespace;

	/**
	 * The current indentation level.
	 */
	uint8_t TabLevel = 0;
		
	
	const char * const NATIVE_s = "NATIVE";

	const char * const HeaderText1_s =
		"/*\n"
		" * DO NOT EDIT - THIS FILE WAS GENERATED.\n"
		" * FROM:";

	const char * const HeaderText2_s =
		"\n"
		" *\n"
		" * Using RiverExplorer Protocol Generator:\n"
    " *\n"
		" * ";
	
	const char * const HeaderText3_s =
		"\n */\n";
		
	bool
	GenerateNATIVE::Create()
	{
		bool		Results = false;

		if (CreateOutputTree(Global::GScope.GenerateNATIVE_Client,
												 Global::GScope.GenerateNATIVE_Server)) {

			// Start generating code ...
			Global::Log << "Generated NATIVE/C++ tree." << EolTabs(TabLevel);

			bool Headers = WriteHeaders();

			if (Headers) {
				Global::Log << "Wrote NATIVE/C++ header files." << EolTabs(TabLevel);
			} else {
				Global::Log << "ERROR: Did not Write NATIVE/C++ header files."
										<< EolTabs(TabLevel);
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
	GenerateNATIVE::CreateClientSideCode() const
	{
		/**@todo*/
		return(false);
	}

	bool
	GenerateNATIVE::CreateServerSideCode() const
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

	std::string
	GenerateNATIVE::SymbolType(Symbol * S) const
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
				Results = "uint32_t";

			} else if (S->Bits <= 64) {
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
				Results = "int32_t";

			} else if (S->Bits <= 64) {
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
	GenerateNATIVE::to_string(Symbol::Array_e A) const
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
	GenerateNATIVE::to_string(Symbol::Visibility_e V) const
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
	GenerateNATIVE::DeclareVariablType(Symbol * S) const
	{
		std::string Results = SymbolType(S);

		return(Results);
	}

	std::string
	GenerateNATIVE::RangeComment(const Symbol * S) const
	{
		std::string	Results;

		if (S->HasRange()) {
			Results += " *" + EolTabs(TabLevel);
			
			// The range is uint, sint, or float - only.
			//

			if (S->Array == Symbol::NotArray_t) {
				Results += " * With its value ";
			} else {
				Results += " * With each entry, with a value ";
			}
			switch (S->Type) {

			case Symbol::uint_t:
				{
					const RangeT<uint64_t> * UintRange
						= dynamic_cast<const RangeT<uint64_t>*>(S->ValidRange);

					uint64_t Max = UintRange->Max;
					uint64_t Min = UintRange->Min;

					Results += UintRange->ToString(UintRange->LowCmp);
					Results += " ";
					Results += std::to_string(Min);
					Results += " and ";
					Results += UintRange->ToString(UintRange->HighCmp);
					Results += " ";
					Results += std::to_string(Max);
					Results += ".";
					Results += EolTabs(TabLevel);
				}
				break;
				
			case Symbol::sint_t:
				{
				}
				break;
				
			case Symbol::float_t:
				{
				}
				break;
				
			default:
				/*EMPTY*/
				break;
			}
		}

		return(Results);
	}
	
	std::string
	GenerateNATIVE::DefaultComment(const Symbol * S) const
	{
		std::string Results;

		if (S->HasDefault()) {
			Results = " * With a default value of: ";
			
			switch (S->Type) {

			case Symbol::uint_t:
				{
					DefaultValueT<uint64_t> * Default
						= dynamic_cast<DefaultValueT<uint64_t>*>(S->Default);

					Results += std::to_string(Default->Value);
					Results += ".";
					Results += EolTabs(TabLevel);
				}
				break;
			
			case Symbol::sint_t:
				{
					DefaultValueT<int64_t> * Default
						= dynamic_cast<DefaultValueT<int64_t>*>(S->Default);

					Results += std::to_string(Default->Value);
					Results += ".";
					Results += EolTabs(TabLevel);
				}
				break;
			
			case Symbol::float_t:
				{
					DefaultValueT<long double> * Default
						= dynamic_cast<DefaultValueT<long double>*>(S->Default);

					Results += std::to_string(Default->Value);
					Results += ".";
					Results += EolTabs(TabLevel);
				}
				break;
			
			case Symbol::string_t:
				{
					DefaultValueT<std::string> * Default
						= dynamic_cast<DefaultValueT<std::string>*>(S->Default);

						Results += Default->Value;
						Results += ".";
						Results += EolTabs(TabLevel);
				}
				break;
			
			case Symbol::class_t:
				/*EMPTY*/
				break;
			
			case Symbol::void_t:
				/*EMPTY*/
				break;
			
			case Symbol::method_t:
				/*EMPTY*/
			break;

			case Symbol::opaque_t:
				/*EMPTY*/
				break;		
			}
			}

		return(Results);
	}
	
	std::string
	GenerateNATIVE::DeclareVariable(Symbol * S) const
	{
		std::string Results;

		std::stringstream Code;
		std::stringstream Comment;

		Comment << EolTabs(TabLevel)
						<< "/**" << EolTabs(TabLevel)
						<< " * Declared in: " << S->InputFileName
						<< ", at line: " << S->LineNumber << EolTabs(TabLevel)
						<< DefaultComment(S)
						<< RangeComment(S)
						<< " *" << EolTabs(TabLevel);
		
			if (S->IsConstant) {
				Comment << " * '" << S->ID
								<< "' is declared as constant with a value of: ";

				switch (S->Type) {

				case Symbol::uint_t:
					{
						SymbolValueT<uint64_t> * UintValue
							= dynamic_cast<SymbolValueT<uint64_t>*>(S->Value);

						Comment << std::to_string(UintValue->Value)
										<< EolTabs(TabLevel);
					}
					break;
					
				case Symbol::sint_t:
					{
						SymbolValueT<int64_t> * SintValue
							= dynamic_cast<SymbolValueT<int64_t>*>(S->Value);

						Comment << std::to_string(SintValue->Value)
										<< EolTabs(TabLevel);
					}
					break;
					
				case Symbol::float_t:
					{
						SymbolValueT<long double> * SintValue
							= dynamic_cast<SymbolValueT<long double>*>(S->Value);

						Comment << std::to_string(SintValue->Value)
										<< EolTabs(TabLevel);
					}
					break;

				default:
					/*EMPTY*/
					break;
				}
			}
		if (S->Array == Symbol::VariableArray_t) {
			if (S->MinSize == 0 && S->MaxSize == 0) {
				Comment << " * A variable length array, with an unspecifed"
								<< " number of "
								<< SymbolType(S)
								<< " entries."
								<< EolTabs(TabLevel)
								<< " */"
								<< EolTabs(TabLevel);
					
			} else {
				Comment << " * A variable length array, " 
								<< "of between: "
								<< std::to_string(S->MinSize)
								<< " and "
								<< std::to_string(S->MaxSize)
								<< " "
								<< SymbolType(S)
								<< " entries."
								<< EolTabs(TabLevel)
								<< " */"
								<< EolTabs(TabLevel);
			}
			Code << "extern ";

			if (S->IsConstant) {
				Code << "const ";
			}
			Code << "std::vector<"
					 << SymbolType(S)
					 << "> "
					 << " ";

		} else {
			if (S->Array != Symbol::FixedArray_t) {
				Comment << " */" << EolTabs(TabLevel);
			}
			Code << "extern ";
			if (S->IsConstant) {
				Code << "const ";
			}
			Code << SymbolType(S)
					 << " "
					 << S->ID;
		}

		switch (S->Array) {

		case Symbol::NotArray_t:
			Code << ";" << EolTabs(TabLevel);
			break;

		case Symbol::FixedArray_t:
			Comment << " * A fixed sized array, " 
							<< "of " << std::to_string(S->MinSize)
							<< " " << SymbolType(S)
							<< "."
							<< EolTabs(TabLevel)
							<< " */"
							<< EolTabs(TabLevel);
			
			Code << "["
					 << std::to_string(S->MinSize)
					 << "];" << EolTabs(TabLevel);
			break;
			
		case Symbol::VariableArray_t:
			Code << S->ID
					 << ";"
					 << EolTabs(TabLevel);
			break;
			
		}

		Results = Comment.str();
		Results += Code.str();

		return(Results);
	}

	bool
	GenerateNATIVE::WriteHeaders() const
	{
		bool Results = false;

		if (Global::GScope.GenerateCPP) {
			
			// Open header file.
			//
			string InputFile = Global::CurrentFileName.front();
			string HeaderFile = InputFile + ".hpp";

			string HeaderPath = "Generated/NATIVE/C++/include/"
				+ HeaderFile;
			
			ofstream Header(HeaderPath);

			if (Header.is_open()) {

				/**
				 * Print the comments on the top of the generated
				 * header file.
				 *
				 * Inlcude the command line args that were provided
				 * to generate the file.
				 */
				Header << HeaderText1_s
							 << InputFile
							 << HeaderText2_s;

				for (int i = 0; i < Global::ProgramArgc; i++) {
					Header << Global::ProgramArgv[i] << " ";
				}
				Header << HeaderText3_s;

				string If = ToIfDef(InputFile);
				string IfDef = "__RIVEREXPLORER__"
					+ If
				  + "_HPP__";

				Header << "#ifndef " << IfDef << EolTabs(TabLevel)
							 << "#define " << IfDef << EolTabs(TabLevel)
							 << EolTabs(TabLevel);

				Header << EolTabs(TabLevel)
							 << "//" << EolTabs(TabLevel)
							 << "// These are included in all C++ header files"
							 << EolTabs(TabLevel)
							 << "// that are generated with Protocol."
							 << EolTabs(TabLevel)
							 << "//"
							 << EolTabs(TabLevel)
							 << "#include <string>" << EolTabs(TabLevel)
							 << "#include <map>" << EolTabs(TabLevel)
							 << "#include <vector>" << EolTabs(TabLevel)
							 << "#include <cstdint>" << EolTabs(TabLevel)
							 << EolTabs(TabLevel);
				//
				const std::vector<Symbol*> & AllSymbols = SymbolTable::Symbols();
				
				std::vector<Symbol*>::const_iterator SIt;
				Symbol * S;

				for (SIt = AllSymbols.cbegin(); SIt != AllSymbols.cend(); SIt++) {
					S = *SIt;

					if (S->Namespace != CurrentNamespace) {
						if (CurrentNamespace != "") {
							Header << "} // End "
										 << CurrentNamespace
										 << EolTabs(--TabLevel);
						}
						CurrentNamespace = S->Namespace;
						Header << "namespace "
									 << CppNamespace(*S)
									 << " {"
									 << EolTabs(++TabLevel);
					}

					Header << DeclareVariable(S);
					Header << EolTabs(TabLevel);
				}

				while (TabLevel > 0) {
					Header << EolTabs(--TabLevel) << "}" << endl;
				}
				Header << "#endif // " << IfDef << EolTabs(TabLevel);
				Header.close();
				Results = true;
			}
		}

		return(Results);
	}

	bool
	GenerateNATIVE::WriteCode() const
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
	bool
	GenerateNATIVE::WriteDocuments() const
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}

	std::string
	GenerateNATIVE::SymbolMethod(Symbol * S) const
	{
		std::string Results;

		/** @todo */

		return(Results);
	}
	
}
