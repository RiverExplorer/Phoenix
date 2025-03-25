/**
 * Project: Phoenix
 * Time-stamp: <2025-03-24 23:59:53 doug>
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

	std::string						CurrentTypeSpecifier;

	bool									WaitingValue = false;
	
	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::DeclarationContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::ConstantContext* Ctx)
	{
		std::string Text = Ctx->getText();

		//std::cout << From << Text << std::endl;

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

		if (Enter) {
			CurrentState = InEnum;
			CurrentEnum = new Enum();

			if (Ctx->IDENTIFIER() != nullptr) {
				CurrentEnum->Name = Ctx->IDENTIFIER()->getSymbol()->getText();
			}
			std::cout << "BEGIN Enum " << CurrentEnum->Name << std::endl;

		} else {
			if (CurrentEnum != nullptr) {
				OrderedItems.push_back(CurrentEnum);
			}
			std::cout << "END Enum " << CurrentEnum->Name << std::endl;

			CurrentState = Unknown;
		}

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
		//std::string Text = Ctx->getText();
		//std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::StructTypeSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InStruct;
			CurrentStruct = new Struct();

			if (Ctx->IDENTIFIER()) {
				CurrentStruct->Name = Ctx->IDENTIFIER()->getSymbol()->getText();
			}
			std::cout << "BEGIN struct " << CurrentStruct->Name << std::endl;

		} else {
			if (CurrentStruct != nullptr) {
				OrderedItems.push_back(CurrentStruct);
			}
			std::cout << "END struct " << CurrentStruct->Name << std::endl;

			CurrentState = Unknown;
		}

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
		//std::cout << From << Text << std::endl;

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

		//std::cout << From << Text << std::endl;

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

		//std::cout << From << Text << std::endl;

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

		//std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::XdrSpecificationContext* Ctx)
	{
		std::string Text = Ctx->getText();

		//std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 tree::TerminalNode* Ctx)
	{

		std::string Text = Ctx->getText();

		switch (CurrentState) {

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
			//std::cout << From << Text << std::endl;
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

		//std::cout << From << Text << std::endl;

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
														 xdrParser::TypeDefDefContext  * Ctx)
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
														 xdrParser::SpecsContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		//std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::VarContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVar;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::VarFixedContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVarFixed;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::VarVariableContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVarVariable;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::OpaqueFixedContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InOpaqueFixed;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::OpaqueVariableContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InOpaqueVariable;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}
	
	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::StringContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InString;
			std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::VoidContext  * Ctx)
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
														 xdrParser::CommentContext  * Ctx)
	{
		std::string Text = Ctx->getText();

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
			//std::cout << From << Text << std::endl;
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
			//std::cout << From << Text << std::endl;
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
														 xdrParser::ProcReturnContext  * Ctx)
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

	///////////////////////////////////////////////////////////////////////
	void
	MyXdrListener::enterDeclaration(xdrParser::DeclarationContext *Ctx)
	{
		//ProcessNode(true, "Enter Dec: ", Ctx);
	}
	
	void
	MyXdrListener::exitDeclaration(xdrParser::DeclarationContext *Ctx)
	{
		//ProcessNode(false, "Exit Dec: ", Ctx);
	}

	void
	MyXdrListener::enterDeclaration(xdrParser::ConstantDefContext *Ctx)
	{
		ProcessNode(true, "Exit Constant Dec: ", Ctx);
	}
	
	void
	MyXdrListener::exitDeclaration(xdrParser::ConstantDefContext *Ctx)
	{
		ProcessNode(false, "Exit Constant Dec: ", Ctx);
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
		//ProcessNode(false, "Exit TypeSpecifier: ", Ctx);
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
		//ProcessNode(false, "Exit StructBody: ", Ctx);
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
		ProcessNode(true,"Enter Def: ", Ctx);
	}

	void
	MyXdrListener::exitDefinition(xdrParser::DefinitionContext *Ctx)
	{
		ProcessNode(false, "Exit Def: ", Ctx);
	}

	void
	MyXdrListener::enterXdrSpecification(xdrParser::XdrSpecificationContext *Ctx)
	{
		ProcessNode(true,"Enter Spec: ", Ctx);
	}
	
	void
	MyXdrListener::exitXdrSpecification(xdrParser::XdrSpecificationContext *Ctx)
	{
		ProcessNode(false, "Exit Spec: ", Ctx);
	}

	void
	MyXdrListener::enterVar(xdrParser::VarContext *Ctx)
	{
		ProcessNode(false, "Enter Var: ", Ctx);
	}

	void
	MyXdrListener::exitVar(xdrParser::VarContext *Ctx)
	{
		ProcessNode(false, "Exit Var: ", Ctx);
	}

	void
	MyXdrListener::enterVarFixed(xdrParser::VarFixedContext *Ctx)
	{
		ProcessNode(false, "Enter VarFixed: ", Ctx);
	}

	void
	MyXdrListener::exitVarFixed(xdrParser::VarFixedContext *Ctx)
	{
		ProcessNode(false, "Exit VarFixed: ", Ctx);
	}

	void
	MyXdrListener::enterVarVariable(xdrParser::VarVariableContext *Ctx)
	{
		ProcessNode(false, "Enter VarVariable: ", Ctx);
	}

	void
	MyXdrListener::exitVarVariable(xdrParser::VarVariableContext *Ctx)
	{
		ProcessNode(false, "Exit VarVariable: ", Ctx);
	}

	void
	MyXdrListener::enterOpaqueFixed(xdrParser::OpaqueFixedContext *Ctx)
	{
		ProcessNode(false, "Enter OpaqueFixed: ", Ctx);
	}

	void
	MyXdrListener::exitOpaqueFixed(xdrParser::OpaqueFixedContext *Ctx)
	{
		ProcessNode(false, "Exit OpaqueFixed: ", Ctx);
	}

	void
	MyXdrListener::enterOpaqueVariable(xdrParser::OpaqueVariableContext *Ctx)
	{
		ProcessNode(false, "Enter OpaqueVariable: ", Ctx);
	}

	void
	MyXdrListener::exitOpaqueVariable(xdrParser::OpaqueVariableContext *Ctx)
	{
		ProcessNode(false, "Exit OpaqueVariable: ", Ctx);
	}

	void
	MyXdrListener::exitString(xdrParser::StringContext *Ctx)
	{
		ProcessNode(false, "Exit String: ", Ctx);
	}

	void
	MyXdrListener::enterString(xdrParser::StringContext *Ctx)
	{
		ProcessNode(false, "Enter String: ", Ctx);
	}

	void
	MyXdrListener::enterVoid(xdrParser::VoidContext *Ctx)
	{
		ProcessNode(false, "Enter Void: ", Ctx);
	}

	void
	MyXdrListener::exitVoid(xdrParser::VoidContext *Ctx)
	{
		ProcessNode(false, "Exit Void: ", Ctx);
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
		//ProcessNode(true,"Enter Every Rule: ", Ctx);
	}
	
	void
	MyXdrListener::exitEveryRule(ParserRuleContext * Ctx)
	{
		//ProcessNode(false, "Exit Every Rule: ", Ctx);
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
	MyXdrListener::enterTypeDefDef(xdrParser::TypeDefDefContext * Ctx)
	{
		ProcessNode(true, "TypeDef Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitTypeDefDef(xdrParser::TypeDefDefContext * Ctx)
	{
		ProcessNode(false, "TypeDefExit : ", Ctx);
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
	MyXdrListener::enterProcReturn(xdrParser::ProcReturnContext * Ctx)
	{
		ProcessNode(true, "ProcReturn Exit : ", Ctx);
	}
	
	void
	MyXdrListener::exitProcReturn(xdrParser::ProcReturnContext * Ctx)
	{
		ProcessNode(false, "ProcReturn Exit : ", Ctx);
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
			mkdir(OutputDirectory.c_str(), 0700);

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

#if wrong_method					
					// Find the last '^%#include'
					//
					Item * LastInclude = nullptr;
					
					for (ItemIt = OrderedItems.cbegin()
								 ; ItemIt != OrderedItems.cend()
								 ; ItemIt++) {

						OneItem = *ItemIt;

						if (OneItem->Type == "PassThrough") {
							if (OneItem->Name.starts_with("#include")) {
								LastInclude = OneItem;
							}
						}
					}
#endif							
					for (ItemIt = OrderedItems.cbegin()
								 ; ItemIt != OrderedItems.cend()
								 ; ItemIt++) {

#if NOPE
						// Only print namespace, after the last
						// '#include', so we don't accidently
						// double the depth of the namespaces.
						//
						if (!NamespacePrinted) {
							if (Namespace != "") {
								Header << std::endl;
								Header << "namespace " << NamespaceToCppNamespace()
											 << std::endl << "{" << std::endl;
								IndentLevel++;
							}
						}
#endif
						OneItem = *ItemIt;
						OneItem->PrintCppHeader(Header);
					}
#ifdef NOPE					
					if (Namespace != "") {
						IndentLevel--;
						Header << "} // End namespace "
									 << NamespaceToCppNamespace()<< std::endl;
					}
#endif					
					Header << std::endl << "#endif // " << Define << std::endl;
					Header.close();
				}

				// C++ Stubs.
				//
				if (GenerateStubs) {

					uint32_t	Offset = 0;
					
					std::string StubFileName;

					for (ItemIt = OrderedItems.cbegin()
								 ; ItemIt != OrderedItems.cend()
								 ; ItemIt++) {

						Struct * AStruct = dynamic_cast<Struct*>(*ItemIt);

						if (AStruct != nullptr) {
							std::vector<Item*>::const_iterator MIt;
							Method * AMethod;
							
							for (Item * MethodItem : AStruct->Members) {
								AMethod = dynamic_cast<Method*>(MethodItem);

								if (AMethod != nullptr) {
						
									StubFileName = CppOutputDirectory;
									StubFileName += "/";
									StubFileName += InputNoExtension;
									StubFileName += "_";
									StubFileName += AStruct->Name;
									StubFileName += "_";
									StubFileName += AMethod->Name;
									StubFileName += "_";
									StubFileName += std::format("{:02}", Offset++);
									StubFileName += ".cpp";

									ofstream StubFile(StubFileName);

									StubFile << "/**" << std::endl;
									if (!NoBanner) {
										GenerateEditThisFile(" * ", StubFile);
									}
									StubFile << " */" << std::endl << std::endl;
							
									StubFile << "#include \""
													 << InputNoExtension << ".hpp\"" << std::endl;
						
									StubFile << "// Get the XDR definitions" << std::endl;
									StubFile << "#include <rpc/rpc.h>" << std::endl;
									StubFile << "#include <string>" << std::endl;
									StubFile << "#include <vector>" << std::endl;
									StubFile << std::endl;

									if (Namespace != "") {
										StubFile << std::endl;
										StubFile << "namespace " << NamespaceToCppNamespace()
														 << std::endl << "{" << std::endl;
										IndentLevel++;
									}
									AMethod->PrintCppStubs(StubFile);

									if (Namespace != "") {
										IndentLevel--;
										StubFile << "} // End namespace "
														 << NamespaceToCppNamespace()<< std::endl;
									}
									StubFile.close();
								}
							}
						}
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
