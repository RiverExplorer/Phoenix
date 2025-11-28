/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#include "GenerateCPP.hpp"
#include <sys/stat.h>
#include <string.h>
#include <sstream>
#include <string>
#include <deque>

using namespace std;

namespace RiverExplorer::Phoenix::Protocol
{

	/**
	 * The current namespace being processed.
	 */
	static std::string	ActiveNamespace;
	static uint8_t			NamespaceDepth = 1;

	static std::deque<std::string> NamespaceStack;
	
	/**
	 * Comments in the protocol definition file
	 * must proceed the item it is commenting about.
	 *
	 * As any comments are accumulated, they are appended
	 * to this string stream.
	 *
	 * Then applied to the next item generated.
	 */
	std::stringstream	StoredComments;
	
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
	GenerateCPP::Create()
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
	GenerateCPP::Compile()
	{
		bool		Results = false;

		/**@todo */

		return(Results);
	}
	
	bool
	GenerateCPP::CreateOutputTree(bool BuildClient, bool BuildServer)
	{
		bool	Results = true;

		_BuildClientCode = BuildClient;
		_BuildServerCode = BuildServer;
 		
		_TopDir = Global::OutputDirectory + "/" + NATIVE_s;

		Results = _BuildDirs();
		
		return(Results);
	}

	bool
	GenerateCPP::CreateClientSideCode() const
	{
		/**@todo*/
		return(false);
	}

	bool
	GenerateCPP::CreateServerSideCode() const
	{
		/**@todo*/
		return(false);
	}


	bool
	GenerateCPP::_BuildCPPDirs()
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
	GenerateCPP::_BuildCSharpDirs()
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
	GenerateCPP::_BuildDirs()
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
	GenerateCPP::SymbolType(Symbol * S) const
	{
		std::string Results = "Unknown";

		switch (S->Type) {

		case Symbol::comment_t:
			/*EMPTY*/
			break;
			
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

		case Symbol::urange_t:
			Results = "uint64_t";
			break;
			
		case Symbol::srange_t:
			Results = "int64_t";
			break;
			
		case Symbol::frange_t:
			Results = "long double";
			break;

		case Symbol::bool_t:
			Results = "bool";
			break;
		}

		return(Results);
	}
	
	std::string
	GenerateCPP::to_string(Symbol::Array_e A) const
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
	GenerateCPP::to_string(Symbol::Visibility_e V) const
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
	GenerateCPP::DeclareVariablType(Symbol * S) const
	{
		std::stringstream R;
		std::string Results;

		std::string SType = SymbolType(S);
		
		if (S->IsConstant) {
			R << "const ";
		}
		
		if (S->Array == Symbol::VariableArray_t) {
			R << "std::vector<" << SType << "> ";

		} else {
			R << SType;

			if (S->Array == Symbol::FixedArray_t) {
				R << "[" << S->MinSize << "] ";
			}
		}
		
		Results = R.str();
		return(Results);
	}

	std::string
	GenerateCPP::RangeComment(const Symbol * S) const
	{
		std::stringstream R;
		
		std::string	Results;

		if (S->HasRange()) {

			// Only one of these 3 will have a value.
			// The other two will be nullptr.
			//
			const RangeT<uint64_t> * UintRange
				= dynamic_cast<const RangeT<uint64_t>*>(S->ValidRange);
			
			const RangeT<int64_t> * SintRange
				= dynamic_cast<const RangeT<int64_t>*>(S->ValidRange);
			
			const RangeT<long double> * FloatRange
				= dynamic_cast<const RangeT<long double>*>(S->ValidRange);

			R << " *" << EolTabs(TabLevel);
			
			// The range is uint, sint, or float - only.
			//

			if (S->Array == Symbol::NotArray_t
					&& (S->Type != Symbol::urange_t
							&& S->Type != Symbol::srange_t
							&& S->Type != Symbol::frange_t)) {
				//R << " * With its value ";

			} else if (S->Type == Symbol::urange_t && UintRange != nullptr) {
				R << " * This is predefined RANGE data:"
					<< EolTabs(TabLevel)
					<< " *"
					<< EolTabs(TabLevel)

					<< " * The minimum value is '"
					<< UintRange->ToString(UintRange->LowCmp)
					<< "' "
					<< std::to_string(UintRange->Min)
					<< "."
					<< EolTabs(TabLevel)
					<< " *"
					<< EolTabs(TabLevel)

					<<  " * The maximum value is '"
					<< UintRange->ToString(UintRange->HighCmp)
					<< "' "
					<< std::to_string(UintRange->Max)
					<< "."
					<< EolTabs(TabLevel);
				
			} else if (S->Type == Symbol::srange_t && SintRange != nullptr) {
				R << " * This is predefined RANGE data:"
					<< EolTabs(TabLevel)
					<< " *"
					<< EolTabs(TabLevel)

					<< " * The minimum value is '"
					<< SintRange->ToString(SintRange->LowCmp)
					<< "' "
					<< std::to_string(SintRange->Min)
					<<  "."
					<< EolTabs(TabLevel)
					<< " *"
					<< EolTabs(TabLevel)

					<< " * The maximum value is '"
					<< SintRange->ToString(SintRange->HighCmp)
					<< "' "
					<< std::to_string(SintRange->Max)
					<< "."
					<< EolTabs(TabLevel);
				
			} else if (S->Type == Symbol::frange_t && FloatRange != nullptr) {
				R << " * This is predefined RANGE data:"
					<< EolTabs(TabLevel)
					<< " *"
					<< EolTabs(TabLevel)

					<< " * The minimum value is '"
					<< FloatRange->ToString(FloatRange->LowCmp)
					<< "' "
					<< std::to_string(FloatRange->Min)
					<< "."
					<< EolTabs(TabLevel)
					<< " *"
					<< EolTabs(TabLevel)

					<< " * The maximum value is '"
					<< FloatRange->ToString(FloatRange->HighCmp)
					<< "' "
					<< std::to_string(FloatRange->Max)
					<< "."
					<< EolTabs(TabLevel);
			}
			
			switch (S->Type) {

			case Symbol::uint_t:
				{
					uint64_t Max = UintRange->Max;
					uint64_t Min = UintRange->Min;

					R << " * Value range: "
						<< UintRange->ToString(UintRange->LowCmp)
						<< " "
						<< std::to_string(Min)
						<< " and "
						<< UintRange->ToString(UintRange->HighCmp)
						<< " "
						<< std::to_string(Max)
						<< "."
						<< EolTabs(TabLevel);
				}
				break;
				
			case Symbol::sint_t:
				{
					int64_t Max = SintRange->Max;
					int64_t Min = SintRange->Min;

					R << " * Value range: "
						<< SintRange->ToString(SintRange->LowCmp)
						<< " "
						<< std::to_string(Min)
						<< " and "
						<< SintRange->ToString(SintRange->HighCmp)
						<< " "
						<< std::to_string(Max)
						<< "."
						<< EolTabs(TabLevel);
				}
				break;
				
			case Symbol::float_t:
				{
					long double Max = FloatRange->Max;
					long double Min = FloatRange->Min;

					R << " * Value range: "
						<< FloatRange->ToString(FloatRange->LowCmp)
						<< " "
						<< std::to_string(Min)
						<< " and "
						<< FloatRange->ToString(FloatRange->HighCmp)
						<< " "
						<< std::to_string(Max)
						<< "."
						<< EolTabs(TabLevel);
				}
				break;

			case Symbol::urange_t:
				/*EMPTY*/
				break;
				
			case Symbol::srange_t:
				/*EMPTY*/
				break;
				
			case Symbol::frange_t:
				/*EMPTY*/
				break;
				
			default:
				/*EMPTY*/
				break;
			}
		}

		Results = R.str();
		return(Results);
	}
	
	std::string
	GenerateCPP::DefaultComment(const Symbol * S) const
	{
		std::stringstream R;
		std::string Results;

		if (S->HasDefault()) {
			R <<  " *" + EolTabs(TabLevel);
			
			if (S->Array == Symbol::NotArray_t) {
				R << " * With a default value of: ";
			} else {
				R << " * With each entry in the array, with a default value of: ";
			}
			
			switch (S->Type) {

			case Symbol::comment_t:
				/*EMPTY*/
				break;
				
			case Symbol::uint_t:
				{
					DefaultValueT<uint64_t> * Default
						= dynamic_cast<DefaultValueT<uint64_t>*>(S->Default);

					R << Default->Value
						<< "."
						<< EolTabs(TabLevel);
				}
				break;
			
			case Symbol::sint_t:
				{
					DefaultValueT<int64_t> * Default
						= dynamic_cast<DefaultValueT<int64_t>*>(S->Default);

					R << Default->Value
						<< "."
						<< EolTabs(TabLevel);
				}
				break;
			
			case Symbol::float_t:
				{
					DefaultValueT<long double> * Default
						= dynamic_cast<DefaultValueT<long double>*>(S->Default);

					R << Default->Value
						<< "."
						<< EolTabs(TabLevel);
				}
				break;
			
			case Symbol::string_t:
				{
					DefaultValueT<std::string> * Default
						= dynamic_cast<DefaultValueT<std::string>*>(S->Default);

					R << Default->Value
						<< "."
						<< EolTabs(TabLevel);
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

			case Symbol::urange_t:
				/**
				 * A range is a protocol definition, not code.
				 */
				/*EMPTY*/
				break;
				
			case Symbol::srange_t:
				/**
				 * A range is a protocol definition, not code.
				 */
				/*EMPTY*/
				break;
				
			case Symbol::frange_t:
				/**
				 * A range is a protocol definition, not code.
				 */
				/*EMPTY*/
				break;		

			case Symbol::bool_t:
				{
					DefaultValueT<bool> * Default
						= dynamic_cast<DefaultValueT<bool>*>(S->Default);

					bool V = Default->Value;
					std::string TF;

					if (V) {
						TF = "true";
					} else {
						TF = "false";
					}
					R << TF
						<< "."
						<< EolTabs(TabLevel);
				}
				break;		
			}
		}

		Results = R.str();
		return(Results);
	}
	
	std::string
	GenerateCPP::DeclareVariable(Symbol * S) const
	{
		std::string Results;

		std::stringstream Code;
		std::stringstream Comment;

		if (S->Type != Symbol::comment_t) {
			Comment << "/**" << EolTabs(TabLevel)
							<< " * Declared in: " << S->InputFileName
							<< ", at line: " << S->LineNumber << EolTabs(TabLevel)
							<< DefaultComment(S)
							<< RangeComment(S)
							<< " *" << EolTabs(TabLevel);
		}
		
		if (S->IsConstant
				&& S->Type != Symbol::urange_t
				&& S->Type != Symbol::srange_t
				&& S->Type != Symbol::frange_t) {

			if (S->Type == Symbol::method_t) {
				Comment << " * '" << S->ID
								<< "' is a const method."
								<< EolTabs(TabLevel);
			} else {
				Comment << " * '" << S->ID
								<< "' is declared as constant with a value of: ";
			}

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
				
			case Symbol::bool_t:
				{
					SymbolValueT<bool> * BoolValue
						= dynamic_cast<SymbolValueT<bool>*>(S->Value);

					std::string TF;
					
					if (BoolValue->Value) {
						TF = "true";
					} else {
						TF = "false";
					}
				
					Comment << TF
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
								<< StoredComments.str()
								<< " */"
								<< EolTabs(TabLevel);
				StoredComments.str("");
				StoredComments.clear();
					
			} else {
				Comment << " * A variable length array, " 
								<< "of between: "
								<< std::to_string(S->MinSize)
								<< " and "
								<< std::to_string(S->MaxSize)
								<< " "
								<< SymbolType(S)
								<< " entries."
								<< StoredComments.str()
								<< " */"
								<< EolTabs(TabLevel);
				StoredComments.str("");
				StoredComments.clear();
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
				if (S->Type != Symbol::comment_t) {
					Comment << StoredComments.str()
									<< " */" << EolTabs(TabLevel);
					StoredComments.str("");
					StoredComments.clear();
				}
			}
			if (S->Type != Symbol::comment_t
					&& S->Type != Symbol::protocol_t
					&& S->Type != Symbol::version_t) {
				Code << "extern ";
			}

			// Methods have their 'const' after the parameters.
			//
			if (S->IsConstant && S->Type != Symbol::method_t) {
				Code << "const ";
			}

			switch (S->Type) {

			case Symbol::urange_t:
				Code << "RangeT<uint64_t> " << S->ID;
				break;
				
			case Symbol::srange_t:
				Code << "RangeT<int64_t> " << S->ID;
				break;
				
			case Symbol::frange_t:
				Code << "RangeT<long double> " << S->ID;
				break;

			case Symbol::method_t:
				{
					Code << SymbolType(S->MethodReturnType)
							 << " " << SymbolType(S)
							 << " "
							 << S->ID
							 << "(";

					if (S->MethodParameters != nullptr) {
						std::vector<Symbol*>::const_iterator PIt;
						Symbol * P;
						bool OnePrinted = false;
						
						for (PIt = S->MethodParameters->cbegin()
									 ; PIt != S->MethodParameters->cend()
									 ; PIt++) {
							P = *PIt;
							if (OnePrinted) {
								Code << ", ";
							}
							Code << SymbolType(P)
									 << " "
									 << P->ID;
							OnePrinted = true;
						}
					}
					Code << ")";
					if (S->IsConstant) {
						Code << " const";
					}
				}
				break;

			case Symbol::comment_t:
				{
					SymbolValueT<std::string> * StrValue
						= dynamic_cast<SymbolValueT<std::string>*>(S->Value);

					if (StrValue->Value.length() > 0) {
						StoredComments << " *" << StrValue->Value
													 << EolTabs(TabLevel);
					}
				}
				break;

			case Symbol::protocol_t:
				ActiveNamespace += "::";
				ActiveNamespace += S->ID;

				Code << "namespace " << S->ID << " {";
				NamespaceDepth++;
				NamespaceStack.push_front(ActiveNamespace);
				
				if (StoredComments.str().length() > 0) {
					StoredComments << "/**"
												 << StoredComments.str()
												 << EolTabs(TabLevel);
					StoredComments.str("");
					StoredComments.clear();
				}
				Comment << "// Begin namespace "
								<< ActiveNamespace
								<< EolTabs(TabLevel);
				TabLevel++;
				break;
				
			case Symbol::version_t:
				ActiveNamespace += "::";
				ActiveNamespace += S->ID;
				NamespaceStack.push_front(ActiveNamespace);

				StoredComments << "// Begin namespace "
											 << ActiveNamespace
											 << EolTabs(TabLevel);
				
				Code << "namespace " << S->ID << " {";
				NamespaceDepth++;
				
				if (StoredComments.str().length() > 0) {
					Comment << StoredComments.str()
									<< EolTabs(TabLevel);
					
					StoredComments.str("");
					StoredComments.clear();
				}
				break;
				
			default:
				Code << SymbolType(S)
						 << " "
						 << S->ID;
			}
		}

		switch (S->Array) {

		case Symbol::NotArray_t:
			if (S->Type != Symbol::comment_t
					&& S->Type != Symbol::protocol_t
					&& S->Type != Symbol::version_t) {
				Code << ";" << EolTabs(TabLevel);
			}
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
	GenerateCPP::WriteHeaders() const
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

				ActiveNamespace = Global::GScope.Namespace;
				if (ActiveNamespace.length() > 0) {
					Header << "namespace " << ActiveNamespace << " {"
								 << EolTabs(++TabLevel);
					NamespaceStack.push_front(ActiveNamespace);
				}
				
				const std::vector<Symbol*> & AllSymbols = SymbolTable::Symbols();
				
				std::vector<Symbol*>::const_iterator SIt;
				Symbol * S;
				std::string SNamespace;

				for (SIt = AllSymbols.cbegin()
							 ; SIt != AllSymbols.cend()
							 ; SIt++) {
					
					S = *SIt;

					if (ActiveNamespace.length() == 0) {
						ActiveNamespace = CppNamespace(*S);
						Header << "namespace " << ActiveNamespace << " {"
									 << EolTabs(++TabLevel);
						NamespaceStack.push_front(ActiveNamespace);
					}
					Header << DeclareVariable(S);
					Header << EolTabs(TabLevel);
				}
				// End of symbols, end the file.
				//
				while (NamespaceDepth-- > 0) {
					Header << EolTabs(TabLevel)
								 << "} // End namespace "
								 << NamespaceStack.front()
								 << EolTabs(TabLevel);
					--TabLevel;
					NamespaceStack.pop_front();
				}
				ActiveNamespace = "";

				Header << "#endif // " << IfDef << EolTabs(TabLevel);
				Header.close();
				Results = true;
			}
		}

		return(Results);
	}

	bool
	GenerateCPP::WriteCode() const
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}
	
	bool
	GenerateCPP::WriteDocuments() const
	{
		bool Results = false;

		/**@todo*/

		return(Results);
	}

	std::string
	GenerateCPP::SymbolMethod(Symbol * S) const
	{
		std::string Results;

		/** @todo */

		return(Results);
	}
	
}
