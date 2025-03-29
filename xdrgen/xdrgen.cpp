/**
 * Project: Phoenix
 * Time-stamp: <2025-03-28 23:12:19 doug>
 * 
 * @file xdrgen.cpp
 * @author Douglas Mark Royer
 * @date 08-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "xdrgen.hpp"
#include "GenerateCpp.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <format>

#include "antlr4-runtime.h"
#include "xdrLexer.h"
#include "xdrParser.h"
#include "xdrListener.h"
#include "xdrBaseListener.h"

#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>

using namespace antlr4;
using namespace std;

namespace RiverExplorer::xdrgen
{
	extern std::string ToUpper(const std::string & In);
	extern void GenerateSharedHpp();
	extern void GenerateThisFileWasGenerated(std::string Prefix,
																					 ofstream & Stream);
	extern std::string Indent();
	extern bool IsBuiltInXdrType(const std::string & Type);
	extern std::string RemoveFileExtension(std::string FileName);

	std::string						PostCppFileName;
	
	int										IndentLevel = 0;
	
	bool									RunQuiet = false;
	std::string						InputFileName;
	std::string						InputNoExtension;
	std::string						OutputDirectory;
	std::string						CppOutputDirectory;
	std::string						AbnfOutputDirectory;
	std::string						ThisFileWasGenerated;
	
	std::string						Namespace;
	std::string						CppNamespace;
	std::vector<Item*>		OrderedItems;
	State									CurrentState = Unknown;
	bool									InArray = false;
	Constant						*	CurrentConstant = nullptr;
	StructMember				*	CurrentStructMember = nullptr;
	Struct							*	CurrentStruct = nullptr;
	Enum								*	CurrentEnum = nullptr;
	EnumValue						*	CurrentEnumValue = nullptr;
	Union								*	CurrentUnion = nullptr;
	UnionCase						*	CurrentUnionCase = nullptr;
	TypeDef							*	CurrentTypeDef = nullptr;
	Method							* CurrentMethod = nullptr;
	Program							* CurrentProgram = nullptr;
	Version							* CurrentVersion = nullptr;
	VersionMethod				* CurrentVersionMethod = nullptr;
	Variable						* CurrentVariable = nullptr;

	std::string						CurrentTypeSpecifier;

	bool									WaitingValue = false;
	bool									DoingParams = false;
	bool									NextIsValue = false;
	
	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::ConstantContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::TypeSpecifierContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentTypeSpecifier = Text;
			std::cout << From << Text << std::endl;
		}
		
		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::EnumTypeSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();
		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::EnumBodyContext* Ctx)
	{
		if (Enter) {
			CurrentState = InEnumBody;
			
		} else {

		}
		std::string Text = Ctx->getText();
		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::StructTypeSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();


		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::StructBodyContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InStructBody;

		} else {

		}
		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::UnionTypeSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InUnion;
			CurrentUnion = new Union();

			std::cout << "BEGIN union " << std::endl;
			
		} else {
			OrderedItems.push_back(CurrentUnion);
			std::cout << "END union " << CurrentUnion->Name << std::endl;
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::UnionBodyContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InUnion;
		} else {
			CurrentState = Unknown;
		}

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::CaseSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InUnionCase;
		}

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::TypeDefContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InTypeDef;
		}
		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::ConstantDefContext* Ctx)
	{
		std::string Text = Ctx->getText();


		if (Enter) {
			CurrentState = InConstant;
			CurrentConstant = new Constant();
			if (Ctx->IDENTIFIER() != nullptr) {
				CurrentConstant->Name = Ctx->IDENTIFIER()->getSymbol()->getText();
			}
		} else {
			OrderedItems.push_back(CurrentConstant);
		}
		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::ValueContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::XdrSpecificationContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 tree::TerminalNode* Ctx)
	{

		std::string Text = Ctx->getText();
		std::cout << From << Text << std::endl;

		switch (CurrentState) {

		case InDeclaration:

			if (WaitingValue && Text != ">") {
				CurrentVariable->ArraySize = Text;
				WaitingValue = false;
				
			} else {
				if (CurrentVariable == nullptr) {
					CurrentVariable = new Variable();
				}

				if (CurrentVariable->Type == "unsigned") {
					if (Text == "short") {
						CurrentVariable->Type = "uint16_t";
						
					} else if (Text == "int") {
						CurrentVariable->Type = "uint32_t";
						
					} else if (Text == "hyper") {
						CurrentVariable->Type = "uint64_t";

					} else if (Text == "char") {
						CurrentVariable->Type = "uint8_t";

					} else if (Text == "quadruple") {
						CurrentVariable->Type = "uint64_t";

					} else {
						std::cerr
							<< "Error:Have unsigned, got: " << Text << std::endl;
					}

				} else if (CurrentVariable->Type == "long" && Text == "double") {
					CurrentVariable->Type == "long double";

				}	else if (CurrentVariable->Type == "") {
					if (Text == "unsigned") {
						CurrentVariable->Type = Text;

					} else if (Text == "short") {
						CurrentVariable->Type = "int16_t";
					
					} else if (Text == "int") {
						CurrentVariable->Type = "int32_t";
					
					} else if (Text == "hyper") {
						CurrentVariable->Type = "int64_t";
					
					} else if (Text == "quadruple") {
						CurrentVariable->Type = "int64_t";
					
					} else if (Text == "char") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "float") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "double") {
						CurrentVariable->Type = Text;

					} else if (Text == "bool") {
						CurrentVariable->Type = Text;

					} else if (Text == "bool_t") {
						CurrentVariable->Type = "bool";
					
					} else if (Text == "uint8_t") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "int8_t") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "uint16_t") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "int16_t") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "uint32_t") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "int32_t") {
						CurrentVariable->Type = Text;
					
					} else if (Text == "uint64_t") {
						CurrentVariable->Type = Text;

					} else if (Text == "int64_t") {
						CurrentVariable->Type = Text;

					} else if (Text == "enum") {
						CurrentVariable->Type = Text;
						//CurrentState = InEnumTypeDef;

					} else if (Text == "struct") {
						CurrentVariable->Type = Text;
						//CurrentState = InStructTypeDef;
					
					} else if (Text == "union") {
						CurrentVariable->Type = Text;
						//CurrentState = InUnionTypeDef;

					} else {
						CurrentVariable->Type = Text;
					}
					
				} else if (CurrentVariable->Name == "") {
					CurrentVariable->Name = Text;

				}	else {
					if (Text == ";") {
						OrderedItems.push_back(CurrentVariable);
						CurrentVariable = nullptr;

					} else if (Text == "[") {
						CurrentVariable->IsFixedArray = true;
						WaitingValue = true;

					} else if (Text == "]") {
						WaitingValue = false;

					} else if (Text == "<>") {
						CurrentVariable->IsVariableArray = true;

					} else if (Text == "<") {
						CurrentVariable->IsVariableArray = true;
						WaitingValue = true;

					} else if (Text == ">") {
						WaitingValue = false;
					}
				}					
			}
			break;
			
		case InVar:
			std::cout << "Process Node: InVar" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InVarPtr:
			std::cout << "Process Node: InVarPtr" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InVarFixed:
			std::cout << "Process Node: InVarFixed" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InVarFixedPtr:
			std::cout << "Process Node: InVarFixedPtr" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InVarVariable:
			std::cout << "Process Node: InVarVariable" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InVarVariablePtr:
			std::cout << "Process Node: InVarVariablePtr" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InOpaqueFixed:
			std::cout << "Process Node: InOpaqueFixed" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InOpaqueFixedPtr:
			std::cout << "Process Node: InOpaqueFixedPtr" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InOpaqueVariable:
			std::cout << "Process Node: InOpaqueVariable" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InOpaqueVariablePtr:
			std::cout << "Process Node: InOpaqueVariablePtr" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InString:
			std::cout << "Process Node: InString" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;
			
		case InStringPtr:
			std::cout << "Process Node: InStringPtr" << std::endl;
			std::cout << "Text = " << Text << endl;
			break;

		case InProgram:
			std::cout << "Process Node: Program" << std::endl;
			std::cout << "Text = " << Text << endl;

			if (Text == "program") {
				CurrentProgram = new Program();

			} else {
				if (Text == "{") {
					// Ignore
				} else if (Text == "}") {
					// Ignore
				} else if (Text == "=") {
					// Ignore

				} else if (Text == ";") {
					if (CurrentProgram != nullptr) {
						OrderedItems.push_back(CurrentProgram);
						CurrentProgram = nullptr;
						CurrentState = Unknown;
					}
				} else {
					if (CurrentProgram->Name == "") {
						CurrentProgram->Name = Text; // The identifier.
					
					} else if (CurrentProgram->Type == "") {
						CurrentProgram->Type = Text; // The value
					}
				}
			}
			break;

		case InVersion:
			std::cout << "Process Node Version" << std::endl;
			std::cout << "Text = " << Text << endl;

			if (Text == "version") {
				CurrentVersion = new Version(*CurrentProgram);

			} else if (Text == "{") {
				// Ignore

			} else if (Text == "}") {
				// Ignore

			} else if (Text == "=") {
				// Ignore

			} else if (Text == ";") {
				if (CurrentProgram != nullptr && CurrentVersion != nullptr) {
					CurrentProgram->Versions.push_back(CurrentVersion);
					CurrentVersion = nullptr;
					CurrentState = InProgram;
				}
			} else {
				if (CurrentVersion->Name == "") {
					CurrentVersion->Name = Text;
				} else {
					CurrentVersion->Type = Text;
				}
			}
			break;

		case  InVersionMethod:
			std::cout << "Process Node VersionMethod" << std::endl;
			std::cout << "Text = " << Text << endl;

			if (WaitingValue) {
				if (Text != ">" && Text != "]") {
					CurrentVersion->ArraySize = Text;
				}
				WaitingValue = false;

			} else {
				if (CurrentVersionMethod == nullptr) {
					CurrentVersionMethod = new VersionMethod(*CurrentVersion);
					NextIsValue = false;
				}
				
				if (Text == "<>") {
					CurrentVersion->IsVariableArray = true;
					WaitingValue = false;
				
				} else if (Text == "<") {
					CurrentVersion->IsVariableArray = true;
					WaitingValue = true;
				
				} else if (Text == "[") {
					CurrentVersion->IsFixedArray = true;
					WaitingValue = true;
				
				} else if (Text == "]") {
					WaitingValue = false;
				
				} else if (Text == ">") {
					WaitingValue = false;
				
				} else if (Text == "(") {
					DoingParams = true;

				} else if (Text == ")") {
					DoingParams = false;

				} else if  (Text == "=") {
					// Next is method value
					//
					NextIsValue = true;

				} else if (Text == "*" ) {
					if (DoingParams) {
						CurrentVersionMethod->Parameters.push_back(Text);
					} else {
						CurrentVersionMethod->IsPointer = true;
					}
				
				} else if (Text == "&") {
					if (DoingParams) {
						CurrentVersionMethod->Parameters.push_back(Text);
					} else {
						CurrentVersionMethod->IsReference = true;
					}
				
				} else if (Text == ";") {
					CurrentVersion->Procedures.push_back(CurrentVersionMethod);
					CurrentVersionMethod = nullptr;
					CurrentState = InVersion;

				} else if (CurrentVersionMethod->Type == "") {
					CurrentVersionMethod->Type = Text;

				} else if (CurrentVersionMethod->Name == "") {
					CurrentVersionMethod->Name = Text;

				} else {
					if (NextIsValue) {
						CurrentVersionMethod->MethodValue = Text;
					} else {
						CurrentVersionMethod->Parameters.push_back(Text);
					}
				}
			}
			break;
			
		case InStruct:
			/*EMPTY*/
			break;

		case InProcedureDef:
			std::cout << "Text = " << Text << endl;
			break;

		case InMethod:
			std::cout << "Method Text = " << Text << endl;
			if (CurrentMethod == nullptr) {
				CurrentMethod = new Method(*CurrentStruct);
			}
			if (Text == "(" || Text == ")") {
				/*Ignore*/

			} else if (Text == ";") {
				if (CurrentStruct != nullptr) {
					CurrentStruct->Members.push_back(CurrentMethod);
					CurrentMethod = nullptr;
					CurrentState = Unknown;
				}

			} else if (CurrentMethod->Type == "") {
				CurrentMethod->Type = Text;

			} else if (CurrentMethod->Name == "") {
				CurrentMethod->Name = Text;

			} else {
				CurrentMethod->Parameters.push_back(Text);
			}
				
			break;
			
		case InNamespaceDef:
			if (Text != "namespace" && Text != ";" && Text != ":") {
				if (Namespace == "") {
					Namespace = Text;
				} else {
					Namespace += ":";
					Namespace += Text;
				}
				if (!RunQuiet) {
					std::cout << "Using namespace: " << Text << std::endl;
				}
			}
			break;

		case InComment:
			break;

		case InConstant:
			if (Enter) {
				if (CurrentConstant == nullptr) {
					CurrentConstant == new Constant();
				}
				if (Text == "=") {
					/*Ignore*/

				} else if (Text == "const") {
					/*Ignore*/

				} else if (Text == ";") {
					CurrentState = Unknown;
					
				} else {
					CurrentConstant->Type = Text;
				}
			}
			break;

		case InTypeDef:
			if (Enter) {
				if (CurrentTypeDef == nullptr) {
					CurrentTypeDef = new TypeDef();
				}
				std::cout << "--IN TYPEDEF: " << Text << std::endl;

				if (Text == "typedef") {
					/*Ignore*/
					
				} else if (Text == ";") {
					OrderedItems.push_back(CurrentTypeDef);
					CurrentTypeDef = nullptr;
					CurrentState = Unknown;
					
				} else if (CurrentTypeDef->Name == "") {
					if (Text == "*") {
						CurrentTypeDef->IsPointer = true;
						break;
					}
					if (CurrentTypeDef->Type == "") {
						CurrentTypeDef->Type = Text;

					} else if (CurrentTypeDef->Type == "struct") {
						CurrentTypeDef->Type += " ";
						CurrentTypeDef->Type += Text;
					} else {
						CurrentTypeDef->Name = Text;
					}
				} else {
					// Type and Name are set, if there is more
					// it is array information.
					//
					if (Text == "[") {
						CurrentTypeDef->IsFixedArray = true;
						
					} else if (Text == "<>") {
						CurrentTypeDef->IsVariableArray = true;

					} else if (Text == "<") {
						CurrentTypeDef->IsVariableArray = true;

					} else if (Text == "]") {
						/*Ignore*/

					} else if (Text == ">") {
						/*Ignore*/

					} else if (Text == "<>") {
						CurrentTypeDef->IsVariableArray = true;
						
					} else if (CurrentTypeDef->IsFixedArray
									|| CurrentTypeDef->IsVariableArray) {
							CurrentTypeDef->ArraySize = Text;
					}
				}
			}
			break;
			
		case InEnum:
			/*EMPTY*/
			break;

		case InEnumBody:
			if (Enter) {
				if (CurrentEnumValue == nullptr) {
					CurrentEnumValue = new EnumValue();
				}
				if (Text.compare(0, 2, "//") == 0) {
						/*Ignore*/
				} else if (Text.compare(0, 2, "/*") == 0) {
						/*Ignore*/
					
				} else if (Text == "{" ) {
					/*Ignore*/
					
				} else if (Text == "=" ) {
					/*Ignore*/
					
				} else if (Text == "}" ) {
					/*Ignore*/
					
				} else if (Text == "," ) {
					/*ignore*/

				} else if (CurrentEnumValue->Name == "") {
					CurrentEnumValue->Name = Text;

				} else {
					CurrentEnumValue->Type = Text;
					CurrentEnum->Enums.push_back(CurrentEnumValue);
					CurrentEnumValue = nullptr;
				}
				 
			}
			break;
			
		case InStructBody:
			if (Enter) {
				if (CurrentStructMember == nullptr) {
					CurrentStructMember = new StructMember();
					std::cout << "------------------------------------------" << std::endl;
				}
				if (Text.compare(0, 2, "//") == 0) {
						/*Ignore*/
				} else if (Text.compare(0, 2, "/*") == 0) {
						/*Ignore*/
					
				} else if (Text == "{" ) {
					/*Ignore*/
					
				} else if (Text == "}" ) {
					/*Ignore*/

				} else if (Text == "struct") {
					/*Ignore*/
					
				//} else if (Text == CurrentStruct->Name) {
					/*Ignore*/
					
				} else if (Text == ";" ) {
					// This is a hack fix for a bug.
					// Something is generating an empty one that is not needed.
					//
					if (CurrentStructMember->Type != "" && CurrentStructMember->Name != "") {
						CurrentStruct->Members.push_back(CurrentStructMember);
					}
					CurrentStructMember = nullptr;
					
				} else if (CurrentStructMember->Type == "") {
					CurrentStructMember->Type = Text;
					std::cout << "Got Struct " << CurrentStruct->Name << " Member Type: " << Text << std::endl;
				} else if (Text == "*") {
					CurrentStructMember->IsPointer = true;
					std::cout << "Got Struct " << CurrentStruct->Name << " Pointer" << std::endl;

				} else if (Text == "<>") {
					CurrentStructMember->IsVariableArray = true;
					
				} else if (Text == "<") {
					CurrentStructMember->IsVariableArray = true;
					InArray = true;
					//std::cout << "Got Struct " << CurrentStruct->Name << " Start Variable Array " << std::endl;
					
				} else if (Text == ">") {
					InArray = false;
					std::cout << "Got Struct " << CurrentStruct->Name
										<< " Variable Array ";

					if (CurrentStructMember->ArraySize == "") {
						std::cout << " No Max Size " << std::endl;
					} else {
						std::cout << " Max Size " << CurrentStructMember->ArraySize << std::endl;
					}
					
				} else if (Text == "[") {
					CurrentStructMember->IsFixedArray = true;
					InArray = true;
					//std::cout << "Got Struct " << CurrentStruct->Name << " Start Fixed Array " << std::endl;
					
				} else if (Text == "]") {
					InArray = false;

					std::cout << "Got Struct " << CurrentStruct->Name << " Fixed Array ";
					if (CurrentStructMember->ArraySize == "") {
						std::cout << " ERROR !! No Max Size" << std::endl;
					} else {
						std::cout << " Max Size " << CurrentStructMember->ArraySize << std::endl;
					}

				} else if (!InArray) {
					CurrentStructMember->Name = Text;

					std::cout << "Got Struct " << CurrentStruct->Name << " Named: " << Text << std::endl;

				} else if (InArray) {
					CurrentStructMember->ArraySize = Text;
					//std::cout << "Got Struct " << CurrentStruct->Name << " Array Size: " << Text << std::endl;
					
				} else {
					std::cout << "Got Struct " << CurrentStruct->Name << " WHAT? GOT!!: " << Text << std::endl;
				}
			
			} else {
				// This is a hack fix for a bug.
				// Something is generating an empty one that is not needed.
				//
				if (CurrentStructMember->Type != "" && CurrentStructMember->Name != "") {
					CurrentStruct->Members.push_back(CurrentStructMember);
				}
				CurrentStructMember = nullptr;
			}
			break;

		case InUnion:
			std::cout << "In Union: " << Text << std::endl;

			if (Text == "default") {
				if (CurrentUnionCase != nullptr) {
					CurrentUnion->Cases.push_back(CurrentUnionCase);
					CurrentUnionCase = nullptr;
				}
				CurrentUnionCase = new UnionCase();
				CurrentUnionCase->CaseValue = Text;
				CurrentState = InUnionCase;

			} else if (Text == "union") {
				/*ignore*/

			} else if (Text == "switch") {
				/*ignore*/
				
			} else if (Text == ";") {
				/*ignore*/
				
			} else if (Text == "(") {
				/*ignore*/
				
			} else if (Text == ")") {
				/*ignore*/
				
			} else if (Text == "{") {
				/*ignore*/
				
			} else if (Text == "}") {
				/*ignore*/
				
			} else {
				if (CurrentUnion->Default == nullptr) {
					if (CurrentUnion->Name == "") {
						CurrentUnion->Name = Text;

					} else if (CurrentUnion->SwitchType == "") {
						CurrentUnion->SwitchType = Text;

					} else if (CurrentUnion->SwitchVariable == "") {
						CurrentUnion->SwitchVariable = Text;

					} else if (Text == "default") {
						CurrentUnion->Default = new UnionCase();
						CurrentUnion->Default->CaseValue = "void";
					}
				} else {
					if (CurrentUnion->Default->Type == "") {
						CurrentUnion->Default->Type = Text;
					} else {
						CurrentUnion->Name = Text;
					}
				}
			}
			
			break;

		case InUnionCase:
			std::cout << "In Union Case: " << Text << std::endl;

			if (WaitingValue) {
				if (Text != ">") { // Was/is variable with no predefined size.
					CurrentUnionCase->ArraySize = Text;
				}
				WaitingValue = false;
			} else {
					
				if (Text == "case") {
					if (CurrentUnionCase != nullptr) {
						CurrentUnion->Cases.push_back(CurrentUnionCase);
						CurrentUnion = nullptr;
					}
					CurrentUnionCase = new UnionCase();

				} else if (Text == ":") {
					/*EMPTY*/

				} else if (Text == ";") {
					CurrentUnion->Cases.push_back(CurrentUnionCase);
					CurrentUnionCase = nullptr;
					CurrentState = InUnion;
				
				} else if (Text == "<>") {
					CurrentUnionCase->IsVariableArray = true;
					WaitingValue = false;
					
				} else if (Text == "<") {
					CurrentUnionCase->IsVariableArray = true;
					WaitingValue = true;
					
				} else if (Text == ">") {
					// They are optional with <>.
					//
					WaitingValue = false;
				
				} else if (Text == "[") {
					CurrentUnionCase->IsFixedArray = true;
					WaitingValue = true;
				
				} else if (Text == "]") {
					/*EMPTY*/
				
				} else if (Text == "*") {
					CurrentUnionCase->IsPointer = true;
				
				} else {
					if (CurrentUnionCase != nullptr) {
						if (CurrentUnionCase->CaseValue == "") {
							CurrentUnionCase->CaseValue = Text;

						} else {
							if (Text == "void" ) {
								// Void has no data.
								//
								CurrentUnionCase->Type = Text;
						
							} else {
								// Not void, so it has a type and data.
								//
								if (CurrentUnionCase->Type == "") {
									// Type not set yet, first value is type.
									//
									CurrentUnionCase->Type = Text;
							
								} else {
									if (Text == "*") {
										CurrentUnionCase->IsPointer = true;
									} else {
										if (CurrentUnionCase->Name == "") {
											CurrentUnionCase->Name = Text;
										}
									}
								}
							}
						}
					}
				}
			}
			
			break;
			
		default:
			std::cout << From << Text << std::endl;
			break;
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::DefinitionContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 ParserRuleContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::NamespaceDefContext  * Ctx)
	{
		if (Enter) {
			CurrentState = InNamespaceDef;
		} else {
			CurrentState = Unknown;
		}

		return;
	}
	
	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::SpecsContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::CommentContext  * Ctx)
	{
		std::string Text = Ctx->getText();
		std::cout << From << Text << std::endl;

		if (Enter) {
			Comment * NewOne = new Comment();
			NewOne->Type = "comment";
			NewOne->Name = Text;

			if (CurrentState == InUnion || CurrentState == InUnionCase) {
				CurrentUnion->Cases.push_back(NewOne);
				
			} else  if (CurrentState == InStructBody) {
				CurrentStruct->Members.push_back(NewOne);
				
			} else  if (CurrentState == InEnum) {
				CurrentEnum->Enums.push_back(NewOne);
				
			} else {
				OrderedItems.push_back(NewOne);
			}
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::PassThroughContext  * Ctx)
	{
		std::string Text = Ctx->getText();
		std::cout << From << Text << std::endl;
	
		if (Enter) {
			PassThrough * NewOne = new PassThrough();
			NewOne->Type = "PassThrough";
			NewOne->Name = Text.substr(1);

			if (CurrentState == InUnion || CurrentState == InUnionCase) {
				CurrentUnion->Cases.push_back(NewOne);
				
			} else  if (CurrentState == InStructBody) {
				CurrentStruct->Members.push_back(NewOne);
				
			} else  if (CurrentState == InEnum) {
				CurrentEnum->Enums.push_back(NewOne);
				
			} else {
				OrderedItems.push_back(NewOne);
			}
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::MethodContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InMethod;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::ProcFirstArgContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			//CurrentState = InVoid;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::ProgramContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			//CurrentState = InVoid;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::VersionContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVoid;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::VersionMethodContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVoid;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}


	///////////////////////////////////////////////////////////////////////

	void
	MyXdrListener::enterDeclaration(xdrParser::DeclarationContext * Ctx)
	{
		CurrentState = InDeclaration;
		ProcessNode(true, "Declaration Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitDeclaration(xdrParser::DeclarationContext * Ctx)
	{
		ProcessNode(false, "Declaration Exit : ", Ctx);
	}
	
	void
	MyXdrListener::enterValue(xdrParser::ValueContext *Ctx)
	{
		ProcessNode(true, "Enter Value: ", Ctx);
	}
	
	void
	MyXdrListener::exitValue(xdrParser::ValueContext *Ctx)
	{
		ProcessNode(false, "Exit Value: ", Ctx);
	}

	void
	MyXdrListener::enterConstant(xdrParser::ConstantContext *Ctx)
	{
		ProcessNode(true, "Enter Const: ", Ctx);
	}

	void
	MyXdrListener::exitConstant(xdrParser::ConstantContext *Ctx)
	{
		ProcessNode(false, "Exit Const: ", Ctx);
	}

	void
	MyXdrListener::enterTypeSpecifier(xdrParser::TypeSpecifierContext *Ctx)
	{
		std::cout << "In: enterTypeSpecifier" << std::endl;
		ProcessNode(true, "Enter TypeSpecifier: ", Ctx);
	}

	void
	MyXdrListener::exitTypeSpecifier(xdrParser::TypeSpecifierContext *Ctx)
	{
		ProcessNode(false, "Exit TypeSpecifier: ", Ctx);
	}

	void
	MyXdrListener::enterEnumTypeSpec(xdrParser::EnumTypeSpecContext *Ctx)
	{
		ProcessNode(true,"Enter Enum: ", Ctx);
	}

	void
	MyXdrListener::exitEnumTypeSpec(xdrParser::EnumTypeSpecContext *Ctx)
	{
		ProcessNode(false, "Exit TypeDef: ", Ctx);
	}

	void
	MyXdrListener::enterEnumBody(xdrParser::EnumBodyContext *Ctx)
	{
		ProcessNode(true,"Enter EnumBody: ", Ctx);
	}

	void
	MyXdrListener::exitEnumBody(xdrParser::EnumBodyContext *Ctx)
	{
		ProcessNode(false, "Exit EnumBody: ", Ctx);
	}

	void
	MyXdrListener::enterStructTypeSpec(xdrParser::StructTypeSpecContext *Ctx)
	{
		ProcessNode(true,"Enter Struct: ", Ctx);
	}

	void
	MyXdrListener::exitStructTypeSpec(xdrParser::StructTypeSpecContext *Ctx)
	{
		ProcessNode(false, "Exit Struct: ", Ctx);
	}

	void
	MyXdrListener::enterStructBody(xdrParser::StructBodyContext *Ctx)
	{
		ProcessNode(true,"Enter StructBody: ", Ctx);
	}

	void
	MyXdrListener::exitStructBody(xdrParser::StructBodyContext *Ctx)
	{
		ProcessNode(false, "Exit StructBody: ", Ctx);
	}

	void
	MyXdrListener::enterUnionTypeSpec(xdrParser::UnionTypeSpecContext *Ctx)
	{
		ProcessNode(true,"Enter Union: ", Ctx);
	}

	void
	MyXdrListener::exitUnionTypeSpec(xdrParser::UnionTypeSpecContext *Ctx)
	{
		ProcessNode(false, "Exit Union : ", Ctx);
	}

	void
	MyXdrListener::enterUnionBody(xdrParser::UnionBodyContext *Ctx)
	{
		ProcessNode(true,"Enter UnionBody: ", Ctx);
	}

	void
	MyXdrListener::exitUnionBody(xdrParser::UnionBodyContext *Ctx)
	{
		ProcessNode(false, "Exit UnionBody: ", Ctx);
	}

	void
	MyXdrListener::enterCaseSpec(xdrParser::CaseSpecContext *Ctx)
	{
		ProcessNode(true,"Enter Case: ", Ctx);
	}

	void
	MyXdrListener::exitCaseSpec(xdrParser::CaseSpecContext *Ctx)
	{
		ProcessNode(false, "Exit Case: ", Ctx);
	}

	void
	MyXdrListener::enterConstantDef(xdrParser::ConstantDefContext *Ctx)
	{
		ProcessNode(true,"Enter ConstDef: ", Ctx);
	}

	void
	MyXdrListener::exitConstantDef(xdrParser::ConstantDefContext *Ctx)
	{
		ProcessNode(false, "Exit ConstDef: ", Ctx);
	}

	void
	MyXdrListener::enterTypeDef(xdrParser::TypeDefContext *Ctx)
	{
		ProcessNode(true,"Enter TypeDef: ", Ctx);
	}

	void
	MyXdrListener::exitTypeDef(xdrParser::TypeDefContext *Ctx)
	{
		ProcessNode(false, "Exit TypeDef: ", Ctx);
	}

	void
	MyXdrListener::enterDefinition(xdrParser::DefinitionContext *Ctx)
	{
		ProcessNode(true,"Enter Definition: ", Ctx);
	}

	void
	MyXdrListener::exitDefinition(xdrParser::DefinitionContext *Ctx)
	{
		ProcessNode(false, "Exit Definition: ", Ctx);
	}

	void
	MyXdrListener::enterXdrSpecification(xdrParser::XdrSpecificationContext *Ctx)
	{
		ProcessNode(true,"Enter Specification: ", Ctx);
	}
	
	void
	MyXdrListener::exitXdrSpecification(xdrParser::XdrSpecificationContext *Ctx)
	{
		ProcessNode(false, "Exit Specification: ", Ctx);
	}

	void
	MyXdrListener::visitTerminal(tree::TerminalNode * Node)
	{
		ProcessNode(true,"Visit Terminal: ", Node);
	}
	
	void
	MyXdrListener::visitErrorNode(tree::ErrorNode * Node)
	{
		ProcessNode(true, "Visit Error : ", Node);
	}

	void
	MyXdrListener::enterEveryRule(ParserRuleContext * Ctx)
	{
		ProcessNode(true,"Enter Every Rule: ", Ctx);
	}
	
	void
	MyXdrListener::exitEveryRule(ParserRuleContext * Ctx)
	{
		ProcessNode(false, "Exit Every Rule: ", Ctx);
	}
	
	void
	MyXdrListener::enterNamespaceDef(xdrParser::NamespaceDefContext * Ctx)
	{
		ProcessNode(true, "Namespace Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitNamespaceDef(xdrParser::NamespaceDefContext * Ctx)
	{
		ProcessNode(false, "Namespce Exit : ", Ctx);
	}
	
	void
	MyXdrListener::enterPassThrough(xdrParser::PassThroughContext * Ctx)
	{
		ProcessNode(true, "PassThrough Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitPassThrough(xdrParser::PassThroughContext * Ctx)
	{
		ProcessNode(false, "PassThrough Exit : ", Ctx);
	}
	
	void
	MyXdrListener::enterSpecs(xdrParser::SpecsContext * Ctx)
	{
		ProcessNode(true, "SpecsExit : ", Ctx);
	}
	
	void
	MyXdrListener::exitSpecs(xdrParser::SpecsContext * Ctx)
	{
		ProcessNode(false, "Specs Exit : ", Ctx);
	}
	
	void
	MyXdrListener::enterComment(xdrParser::CommentContext * Ctx)
	{
		ProcessNode(true, "Comment Exit : ", Ctx);
	}
	
	void
	MyXdrListener::exitComment(xdrParser::CommentContext * Ctx)
	{
		ProcessNode(false, "Comment Exit : ", Ctx);
	}

	void
	MyXdrListener::enterMethod(xdrParser::MethodContext * Ctx)
	{
		ProcessNode(true, "Method Exit : ", Ctx);
	}
	
	void
	MyXdrListener::exitMethod(xdrParser::MethodContext * Ctx)
	{
		ProcessNode(false, "Method Exit : ", Ctx);
	}

	void
	MyXdrListener::enterProcFirstArg(xdrParser::ProcFirstArgContext * Ctx)
	{
		ProcessNode(true, "ProcFirstArg Exit : ", Ctx);
	}
	
	void
	MyXdrListener::exitProcFirstArg(xdrParser::ProcFirstArgContext * Ctx)
	{
		ProcessNode(false, "ProcFirstArg Exit : ", Ctx);
	}

	void
	MyXdrListener::enterProgram(xdrParser::ProgramContext * Ctx)
	{
		CurrentState = InProgram;
		ProcessNode(true, "Program Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitProgram(xdrParser::ProgramContext * Ctx)
	{
		CurrentState = Unknown;
		ProcessNode(false, "Program Exit : ", Ctx);
	}

	void
	MyXdrListener::enterVersion(xdrParser::VersionContext * Ctx)
	{
		CurrentState = InVersion;
		ProcessNode(true, "Version Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitVersion(xdrParser::VersionContext * Ctx)
	{
		CurrentState = InProgram;
		ProcessNode(false, "Version Exit : ", Ctx);
	}

	void
	MyXdrListener::enterVersionMethod(xdrParser::VersionMethodContext * Ctx)
	{
		CurrentState = InVersionMethod;
		ProcessNode(true, "VersionMethod Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitVersionMethod(xdrParser::VersionMethodContext * Ctx)
	{
		//CurrentState = Unknown;
		ProcessNode(false, "VersionMethod Exit : ", Ctx);
	}

	void
	MyXdrListener::enterDataType(xdrParser::DataTypeContext * Ctx)
	{
		CurrentState = InVersionMethod;
		ProcessNode(true, "DataType Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitDataType(xdrParser::DataTypeContext * Ctx)
	{
		//CurrentState = Unknown;
		ProcessNode(false, "DataType Exit : ", Ctx);
	}


} // End namespace RiverExplorer::xdrgen

void
Usage(const char * Pgm)
{
	std::cout << std::endl
						<< "Usage: "
						<< std::endl
						<< basename(strdup(Pgm))
						<< " [--quiet]"
						<< " --input <filename.x>"
						<< " (--outdir Dir | -out OutFile"
						<< " --lang (CPP|C#)"
						<< " -D[cpp-value]"
						<< " [--namespace=part1:part2:...]"
						<< " [--xdr | --xds | --headers | --abnf | --stubs | --server]"
						<< std::endl;

	return;
}

using namespace RiverExplorer::xdrgen;

/**
 * Open CPP and send the input file to it.
 * Then read the result file name.
 */
void
Preprocess(const std::string & InFile,
						const std::vector<std::string> & Args)
{
	if (access(InFile.c_str(), R_OK) == F_OK) {
		PostCppFileName = "/tmp/xdrgen_";
		PostCppFileName += std::to_string(getpid());

		std::string Cmd = "/bin/cpp -nostdinc -P -E -C ";

		std::vector<std::string>::const_iterator AIt;

		for (AIt = Args.begin(); AIt != Args.end(); AIt++) {
			Cmd += " ";
			Cmd += *AIt;
		}

		Cmd += " < ";
		Cmd += InFile;
		Cmd += " > ";
		Cmd += PostCppFileName;

		std::cout << std::endl << Cmd << std::endl;
		system(Cmd.c_str());
	}

	return;
}

void
OnExit()
{
	unlink(PostCppFileName.c_str());

	return;
}

int
main(int argc, char *argv[])
{
	string line;

	int Opt;

	std::vector<std::string> CppArgs;
	std::string Input;
	std::string OutDir;
	std::string OutFile;
	std::string Language;
	
	bool GenerateXdr = false;
	bool GenerateHeaders = false;
	bool GenerateStubs = false;
	bool GenerateXsd = false;
	bool GenerateAbnf = false;
	bool GenerateServer = false;
	bool NoBanner = false;
	
	bool Error = false;

	const int NoArgument = 0;
	const int RequiredArgument = 1;
	const int OptionalArgument = 2;

	atexit(OnExit);
	
	while (1) {
		int ThisOpt = optind ? optind : 1;
		int OptIndex = 0;

		static struct option LongOpts[] = {
			{"input",			RequiredArgument,	0, 0},
			{"outdir",		RequiredArgument,	0, 1},
			{"lang",			RequiredArgument,	0, 3},
			{"xdr",				NoArgument,				0, 4},
			{"xsd",				NoArgument,				0, 5},
			{"headers",		NoArgument,				0, 6},
			{"abnf",			NoArgument,				0, 7},
			{"stubs",			NoArgument,				0, 8},
			{"quiet",			NoArgument,				0, 9},
			{"D",					OptionalArgument,	0, 10},
			{"server",		NoArgument,				0, 11},
			{"nobanner",	NoArgument,				0, 12},
			{"namespace",	RequiredArgument,	0, 13},
			{"out",				RequiredArgument,	0, 14},
			{nullptr,			NoArgument,				0, 0}
		};
		
		Opt = getopt_long(argc, argv, "D::achlo:m",
											LongOpts, &OptIndex);

		if (Opt == -1) {
			break;
		}

		switch (Opt) {

		case 0: // Input
			if (optarg) {
				Input = optarg;
				InputFileName = basename(strdup(Input.c_str()));
				InputNoExtension = basename(strdup(RemoveFileExtension(Input).c_str()));
				
				if (!RunQuiet) {
					std::cout << "Reading from input: " << Input << std::endl;
				}
			}
			break;

		case 'o':
		/*FALLTHRU*/
		case 1: // OutDir
			if (optarg) {
				OutDir = optarg;
				OutputDirectory = OutDir;
				if (!RunQuiet) {
					std::cout << "Writing to directory: " << OutDir << std::endl;
				}
			}
			break;

		case 14: // Out (file)
			if (optarg) {
				OutFile = optarg;
				OutputDirectory = dirname(strdup(OutFile.c_str()));
				if (OutputDirectory == "/") {
					OutputDirectory = ".";
				}
				if (!RunQuiet) {
					std::cout << "Writing to file: " << OutFile << std::endl;
				}
			}
			break;
			
		case 3: // Lang
			if (Language != "") {
				std::cout << "ERROR: Only 1 language at a time." << std::endl;
				Error = true;
				
			} else if (optarg) {
				Language = optarg;
				if (Language == "CPP" || Language == "C#") {
					if (!RunQuiet) {
						std::cout << "For output languge: " << Language << std::endl;
					}
				} else {
					std::cout << "ERROR: Only CPP and C# supported." << std::endl;
					Error = true;
				}
			}
			break;

		case 4: // XDR
			GenerateXdr = true;
			CppArgs.push_back(std::string("-DRPC_XDR"));
			break;

		case 5: // XSD
			GenerateXsd = true;
			break;

		case 'h':
			/*FALLTHRU*/
		case 6: // Headers.
			GenerateHeaders = true;
			CppArgs.push_back(std::string("-DRPC_HDR"));
			break;

		case 7: // ABNF
			GenerateAbnf = true;
			break;

		case 'l':
			/*FALLTHRU*/
		case 8: // Stubs
			GenerateStubs = true;
			CppArgs.push_back(std::string("-DRPC_CLNT"));
			break;

		case 9: // Quiet
			RunQuiet = true;
			break;

		case 'D':
			/*FALLTHRU*/
		case 10: // DEFINE
			{
				std::string Tmp = "-D";

				Tmp += optarg;
				CppArgs.push_back(Tmp);
			}
			break;

		case 'm':
			/*FALLTHRU*/
		case 11: // Server Side - no main.
			GenerateServer = true;
			break;

		case 12:
			NoBanner = true;
			break;
			
		case 13:
			Namespace = optarg;
			break;
			
		case '?':
			Usage(argv[0]);
			break;

		default:
			std::cout << "Unknown option: " << optarg << std::endl;
			Error = true;
			break;
		}
	}

	if (Input == "") {
		Usage(argv[0]);
		exit(1);
	}

	if (OutDir != "" && OutFile != "") {
		std::cout << "Error: You can specify --outdir OR --out, but not both"
							<< std::endl;
		Usage(argv[0]);
		exit(1);
	}
	if (!GenerateXdr
			&& !GenerateXsd
			&& !GenerateHeaders
			&& !GenerateAbnf
			&& !GenerateStubs
			&& !GenerateServer) {
		std::cout << std::endl
							<< "Error: Must have one or more of: --xdr, --xds, --headers, --abnf, --stubs, --server"
							<< std::endl;
		Error = true;

	}

	if (OutDir == "" && OutFile == "") {
		std::cout << std::endl
							<< "Error: Must specific output directory --outdir <dir> or -out File"
							<< std::endl;
		Error = true;
	}

	if (Language == "") {
		std::cout << std::endl
							<< "Error: Must specific a target language."
							<< "'CPP', and 'C#' supported."
							<< std::endl;
		Error = true;
	}

	if (Error) {
		Usage(argv[0]);
		exit(1);
	}

	if (access(OutputDirectory.c_str(), W_OK|R_OK|X_OK) != 0) {
		if (!RunQuiet) {
			std::cout << "Output directory: '"
								<< OutputDirectory
								<< " 'does not exist, or can't access it."
								<< " Attepting to fix."
								<< std::endl;
		}
		if (OutputDirectory != ".") {
			chmod(OutputDirectory.c_str(), 0700);
		}
		if (access(OutputDirectory.c_str(), W_OK|R_OK|X_OK) != 0) {
			MakePath(OutputDirectory);

			if (access(OutputDirectory.c_str(), W_OK|R_OK|X_OK) != 0) {
				std::cout << "ERROR: Can not create or fix output directory: '"
									<< OutputDirectory
									<< "'"
									<< std::endl;
				exit(2);
			}
		}
	}

	Preprocess(Input, CppArgs);
	ifstream xdrFile (PostCppFileName);
	//ifstream xdrFile (Input);
	if (xdrFile.is_open()) {

		ANTLRInputStream input(xdrFile);
		xdrLexer lexer(&input);
		CommonTokenStream tokens(&lexer);

		tokens.fill();

		//for (auto token : tokens.getTokens()) {
		// std::cout << token->toString() << std::endl;
		//}

		xdrParser parser(&tokens);
		tree::ParseTree * Tree = parser.xdrSpecification();

		MyXdrListener	Listener;

		tree::ParseTreeWalker::DEFAULT.walk(&Listener, Tree);
		
		//		std::cout << Tree->toStringTree(&parser) << std::endl;

		CppNamespace = NamespaceToCppNamespace();
		
		if (OrderedItems.size() > 0) {
			if (!RunQuiet) {
				std::cout << OrderedItems.size()
									<< " items will be generated."
									<< std::endl;
			}

			std::vector<Item*>::const_iterator ItemIt;
			Item * OneItem = nullptr;
			std::string Define;

			if (Language == "CPP" ) {
				CppOutputDirectory += OutputDirectory;
				if (OutFile == "") {
					if (Namespace != "") {
						CppOutputDirectory += "/";
						CppOutputDirectory += NamespaceToIncludePath();
					}
					CppOutputDirectory += "/c++";
				}
				MakePath(CppOutputDirectory);

				// C++ Headers.
				//
				if (GenerateHeaders) {

					std::string HeaderFile;

					// Generate the shared header file
					// In the same directory.
					//
					GenerateSharedHpp(CppOutputDirectory);

					HeaderFile = CppOutputDirectory;
					HeaderFile += "/";
					HeaderFile += InputNoExtension;
					HeaderFile += ".hpp";

					ofstream Header(HeaderFile);

					bool NamespacePrinted  = false;
					
					Header << "/**" << std::endl;
					if (!NoBanner) {
						GenerateThisFileWasGenerated(" * ", Header);
					}
					Header << " */" << std::endl << std::endl;
							
					Define = "_RIVEREXPLORER_XDRGEN_";

					Define += Replace(ToUpper(Namespace), ':', '_');
					Define += "_";
					Define += ToUpper(InputNoExtension);
					Define += "_X_HPP_";
					Header << "#ifndef " << Define << std::endl;
					Header << "#define " << Define << std::endl;
					Header << std::endl;

					Header << "#include <XdrGenShared.hpp>" << std::endl;
					Header << "#include <string>" << std::endl;
					Header << "#include <vector>" << std::endl;
					Header << "#include <map>" << std::endl;
					Header << "#include <rpc/rpc.h>" << std::endl;
					Header << "#ifndef W64" << std::endl;
					Header << "#include <unistd.h>" << std::endl;
					Header << "#endif // W64" << std::endl;
					Header << "#include <memory.h>" << std::endl;

					for (ItemIt = OrderedItems.cbegin()
								 ; ItemIt != OrderedItems.cend()
								 ; ItemIt++) {

						OneItem = *ItemIt;
						OneItem->PrintCppHeader(Header);
					}

					// Now generate one xdr_xx(XDR * xdrs, obj*);
					// method for each declaration.
					//

					for (ItemIt = OrderedItems.cbegin()
								 ; ItemIt != OrderedItems.cend()
								 ; ItemIt++) {

						OneItem = *ItemIt;
						if (OneItem->Type != "comment" && OneItem->Type != "passthrough") {
							OneItem->PrintCppHeaderXdr(Header);
						}
					}

					Header << std::endl << "#endif // " << Define << std::endl;
					Header.close();
				}

				// C++ Stubs.
				//
				if (GenerateStubs) {
					ofstream StubsUnused;
					Item			* AnItem;

					for (std::vector<Item*>::iterator it = OrderedItems.begin()
								 ; it != OrderedItems.end()
								 ; it++) {
						
						AnItem = *it;
						// The method prints one file per method name.
						// So the stream is not used.
						//
						AnItem->PrintCppStubs(StubsUnused);
					}
				}
			}

			// XDR
			//
			if (GenerateXdr) {

				std::string XdrFileFile = CppOutputDirectory;

				XdrFileFile += "/";

				if (OutFile == "") {
					XdrFileFile += InputNoExtension;
					XdrFileFile += "_xdr.cpp";
				} else {
					XdrFileFile = OutFile;
				}

				ofstream XdrFile(XdrFileFile);

				XdrFile << "/**" << std::endl;
				if (!NoBanner) {
					GenerateThisFileWasGenerated(" * ", XdrFile);
				}
				XdrFile << " */" << std::endl << std::endl;

				XdrFile << "#include \"" << InputNoExtension
								<< ".hpp\"" << std::endl;

#ifdef NOPE				
				if (Namespace != "") {
					XdrFile << std::endl;
					XdrFile << "namespace " << Namespace
									<< std::endl << "{" << std::endl;
					IndentLevel++;
				}
#endif				
				for (ItemIt = OrderedItems.cbegin()
							 ; ItemIt != OrderedItems.cend()
							 ; ItemIt++) {
							
					OneItem = *ItemIt;
					OneItem->PrintCppXDR(XdrFile);
				}
#ifdef NOPE				
				if (Namespace != "") {
					IndentLevel--;
					XdrFile << "} // End namespace " << Namespace << std::endl;
				}
#endif				
				XdrFile.close();
			}

			// ABNF
			//
			if (GenerateAbnf) {
				std::string AbnfFile;

				AbnfOutputDirectory += OutputDirectory;

				if (OutFile == "") {
					if (Namespace != "") {
						AbnfOutputDirectory += "/";
						AbnfOutputDirectory += NamespaceToIncludePath();
					}
					AbnfOutputDirectory += "/ABNF";
				}
				
				MakePath(AbnfOutputDirectory);

				AbnfFile = AbnfOutputDirectory;
				AbnfFile += "/";
				AbnfFile += InputNoExtension;
				AbnfFile += ".abnf";

				ofstream Abnf(AbnfFile);
				
				if (!NoBanner) {
					GenerateThisFileWasGenerated(" ; ", Abnf);
				}
				
				for (ItemIt = OrderedItems.cbegin()
							 ; ItemIt != OrderedItems.cend()
							 ; ItemIt++) {
							
					OneItem = *ItemIt;
					OneItem->PrintAbnf(Abnf);
				}
				Abnf.close();
			}

		} else {
			if (!RunQuiet) {
				std::cout << "No items to generated."
									<< " Zero items parsed from '"
									<< Input
									<< "' file."
									<< std::endl;
			}
		}
		xdrFile.close();
	}
}
