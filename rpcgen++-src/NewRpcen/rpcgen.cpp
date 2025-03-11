/**
 * Project: Phoenix
 * Time-stamp: <2025-03-10 23:08:21 doug>
 * 
 * @file rpcgen.cpp
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
#include "rpcgen.hpp"
#include "GenerateCpp.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "antlr4-runtime.h"
#include "xdrLexer.h"
#include "xdrParser.h"
#include "xdrListener.h"
#include "xdrBaseListener.h"

#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace antlr4;
using namespace std;

namespace RiverExplorer::rpcgen
{
	extern std::string ToUpper(const std::string & In);
	extern void GenerateSharedHpp();
	extern void GenerateThisFileWasGenerated(std::string Prefix,
																					 ofstream & Stream);
	extern std::string Indent();
	extern bool IsBuiltInXdrType(const std::string & Type);
	extern std::string RemoveFileExtension(std::string FileName);
	
	int										IndentLevel = 0;
	
	bool									RunQuiet = false;
	std::string						InputFileName;
	std::string						InputNoExtension;
	std::string						OutputDirectory;
	std::string						CppOutputDirectory;
	std::string						ThisFileWasGenerated;
	
	std::string						Namespace;
	std::vector<Item*>		OrderedItems;
	State									CurrentState = Unknown;
	bool									InArray = false;
	Constant						*	CurrentConstant = nullptr;
	StructMember				*	CurrentStructMember = nullptr;
	Struct							*	CurrentStruct = nullptr;
	Union								*	CurrentUnion = nullptr;
	UnionCase						*	CurrentUnionCase = nullptr;

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

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::EnumBodyContext* Ctx)
	{
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
			CurrentUnion = nullptr;
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

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter,
														 std::string From,
														 xdrParser::ConstantDefContext* Ctx)
	{
		std::string Text = Ctx->getText();

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

		case InTypedef:
			std::cout << "Text = " << Text << endl;
			break;
			
		case InProcedureDef:
			std::cout << "Text = " << Text << endl;
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
			
		case InStructBody:
			if (Enter) {
				if (CurrentStructMember == nullptr) {
					CurrentStructMember = new StructMember();
					std::cout << "------------------------------------------" << std::endl;
				}
				if (Text == "{" ) {
					/*Ignore*/
					
				} else if (Text == "}" ) {
					/*Ignore*/

				} else if (Text == "struct") {
					/*Ignore*/
					
				} else if (Text == CurrentStruct->Name) {
					/*Ignore*/
					
				} else if (Text == ";" ) {
					CurrentStruct->Members.push_back(CurrentStructMember);
					CurrentStructMember = nullptr;
					
				} else if (CurrentStructMember->Type == "") {
					CurrentStructMember->Type = Text;
					std::cout << "Got Struct " << CurrentStruct->Name << " Member Type: " << Text << std::endl;
				} else if (Text == "*") {
					CurrentStructMember->IsPointer = true;
					std::cout << "Got Struct " << CurrentStruct->Name << " Pointer" << std::endl;

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
				CurrentStruct->Members.push_back(CurrentStructMember);
				CurrentStructMember = nullptr;
			}
			break;

		case InUnion:
			std::cout << "In Union: " << Text << std::endl;

			if (Text == "default") {
				if (CurrentUnionCase != nullptr) {
					CurrentUnion->Cases.push_back(CurrentUnionCase);
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
					}
					CurrentUnionCase = new UnionCase();

				} else if (Text == ":") {
					/*EMPTY*/

				} else if (Text == ";") {
					CurrentUnion->Cases.push_back(CurrentUnionCase);
					CurrentUnionCase = nullptr;
					CurrentState = InUnion;
				
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
														 xdrParser::TypedefDefContext  * Ctx)
	{
		std::string Text = Ctx->getText();

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
														 xdrParser::VarPtrContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVarPtr;
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
 														 xdrParser::VarFixedPtrContext * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVarFixedPtr;
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
														 xdrParser::VarVariablePtrContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InVarVariablePtr;
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
														 xdrParser::OpaqueFixedPtrContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InOpaqueFixedPtr;
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
														 xdrParser::OpaqueVariablePtrContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InOpaqueVariablePtr;
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
														 xdrParser::StringPtrContext  * Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InStringPtr;
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
			} else {
				OrderedItems.push_back(NewOne);
			}
			//std::cout << From << Text << std::endl;
		} else {
		}

		return;
	}

	///////////////////////////////////////////////////////////////////////
	void
	MyXdrListener::enterDeclaration(xdrParser::DeclarationContext *Ctx)
	{
		//std::cout << "In: enterDeclaration" << std::endl;
		//ProcessNode(true, "Enter Dec: ", Ctx);
	}
	
	void
	MyXdrListener::exitDeclaration(xdrParser::DeclarationContext *Ctx)
	{
		//std::cout << "In: exitDeclaration" << std::endl;
		//ProcessNode(false, "Exit Dec: ", Ctx);
	}

	void
	MyXdrListener::enterDeclaration(xdrParser::ConstantDefContext *Ctx)
	{
		//std::cout << "In: enterDeclaration" << std::endl;
		ProcessNode(true, "Exit Constant Dec: ", Ctx);
	}
	
	void
	MyXdrListener::exitDeclaration(xdrParser::ConstantDefContext *Ctx)
	{
		//std::cout << "In: exitDeclaration" << std::endl;
		ProcessNode(false, "Exit Constant Dec: ", Ctx);
	}
	
	void
	MyXdrListener::enterValue(xdrParser::ValueContext *Ctx)
	{
		//std::cout << "In: enterValue" << std::endl;
		ProcessNode(true, "Enter Value: ", Ctx);
	}
	
	void
	MyXdrListener::exitValue(xdrParser::ValueContext *Ctx)
	{
		//std::cout << "In: exitValue" << std::endl;
		ProcessNode(false, "Exit Value: ", Ctx);
	}

	void
	MyXdrListener::enterConstant(xdrParser::ConstantContext *Ctx)
	{
		//std::cout << "In: enterConstant" << std::endl;
		ProcessNode(true, "Enter Const: ", Ctx);
	}

	void
	MyXdrListener::exitConstant(xdrParser::ConstantContext *Ctx)
	{
		//std::cout << "In: exitConstant" << std::endl;
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
		//std::cout << "In: exitTypeSpecifier" << std::endl;
		//ProcessNode(false, "Exit TypeSpecifier: ", Ctx);
	}

	void
	MyXdrListener::enterEnumTypeSpec(xdrParser::EnumTypeSpecContext *Ctx)
	{
		//std::cout << "In: enterEnumTypeSpec" << std::endl;
		ProcessNode(true,"Enter Enum: ", Ctx);
	}

	void
	MyXdrListener::exitEnumTypeSpec(xdrParser::EnumTypeSpecContext *Ctx)
	{
		//std::cout << "In: exitEnumTypeSpec" << std::endl;
		ProcessNode(false, "Exit Typedef: ", Ctx);
	}

	void
	MyXdrListener::enterEnumBody(xdrParser::EnumBodyContext *Ctx)
	{
		//std::cout << "In: enterEnumBody" << std::endl;
		ProcessNode(true,"Enter EnumBody: ", Ctx);
	}

	void
	MyXdrListener::exitEnumBody(xdrParser::EnumBodyContext *Ctx)
	{
		//std::cout << "In: exitEnumBody" << std::endl;
		ProcessNode(false, "Exit EnumBody: ", Ctx);
	}

	void
	MyXdrListener::enterStructTypeSpec(xdrParser::StructTypeSpecContext *Ctx)
	{
		//std::cout << "In: enterStructTypeSpec" << std::endl;
		ProcessNode(true,"Enter Struct: ", Ctx);
	}

	void
	MyXdrListener::exitStructTypeSpec(xdrParser::StructTypeSpecContext *Ctx)
	{
		//std::cout << "In: exitStructTypeSpec" << std::endl;
		ProcessNode(false, "Exit Struct: ", Ctx);
	}

	void
	MyXdrListener::enterStructBody(xdrParser::StructBodyContext *Ctx)
	{
		//std::cout << "In: enterStructBody" << std::endl;
		ProcessNode(true,"Enter StructBody: ", Ctx);
	}

	void
	MyXdrListener::exitStructBody(xdrParser::StructBodyContext *Ctx)
	{
		//std::cout << "In: exitStructBody" << std::endl;
		//ProcessNode(false, "Exit StructBody: ", Ctx);
	}

	void
	MyXdrListener::enterUnionTypeSpec(xdrParser::UnionTypeSpecContext *Ctx)
	{
		//std::cout << "In: enterUnionTypeSpec" << std::endl;
		ProcessNode(true,"Enter Union: ", Ctx);
	}

	void
	MyXdrListener::exitUnionTypeSpec(xdrParser::UnionTypeSpecContext *Ctx)
	{
		//std::cout << "In: exitUnionTypeSpec" << std::endl;
		ProcessNode(false, "Exit Union : ", Ctx);
	}

	void
	MyXdrListener::enterUnionBody(xdrParser::UnionBodyContext *Ctx)
	{
		//std::cout << "In: enterUnionBody" << std::endl;
		ProcessNode(true,"Enter UnionBody: ", Ctx);
	}

	void
	MyXdrListener::exitUnionBody(xdrParser::UnionBodyContext *Ctx)
	{
		//std::cout << "In: exitUnionBody" << std::endl;
		ProcessNode(false, "Exit UnionBody: ", Ctx);
	}

	void
	MyXdrListener::enterCaseSpec(xdrParser::CaseSpecContext *Ctx)
	{
		//std::cout << "In: enterCaseSpec" << std::endl;
		ProcessNode(true,"Enter Case: ", Ctx);
	}

	void
	MyXdrListener::exitCaseSpec(xdrParser::CaseSpecContext *Ctx)
	{
		//std::cout << "In: exitCaseSpec" << std::endl;
		ProcessNode(false, "Exit Case: ", Ctx);
	}

	void
	MyXdrListener::enterConstantDef(xdrParser::ConstantDefContext *Ctx)
	{
		//std::cout << "In: enterConstantDef" << std::endl;
		ProcessNode(true,"Enter ConstDef: ", Ctx);
	}

	void
	MyXdrListener::exitConstantDef(xdrParser::ConstantDefContext *Ctx)
	{
		//std::cout << "In: exitConstantDef" << std::endl;
		ProcessNode(false, "Exit ConstDef: ", Ctx);
	}

	void
	MyXdrListener::enterTypeDef(xdrParser::TypeDefContext *Ctx)
	{
		//std::cout << "In: enterTypeDef" << std::endl;
		ProcessNode(true,"Enter TypeDef: ", Ctx);
	}

	void
	MyXdrListener::exitTypeDef(xdrParser::TypeDefContext *Ctx)
	{
		//std::cout << "In: exitTypeDef" << std::endl;
		ProcessNode(false, "Exit TypeDef: ", Ctx);
	}

	void
	MyXdrListener::enterDefinition(xdrParser::DefinitionContext *Ctx)
	{
		//std::cout << "In: enterDefinition" << std::endl;
		ProcessNode(true,"Enter Def: ", Ctx);
	}

	void
	MyXdrListener::exitDefinition(xdrParser::DefinitionContext *Ctx)
	{
		//std::cout << "In: exitDefinition" << std::endl;
		ProcessNode(false, "Exit Def: ", Ctx);
	}

	void
	MyXdrListener::enterXdrSpecification(xdrParser::XdrSpecificationContext *Ctx)
	{
		//std::cout << "In: enterXdrSpecification" << std::endl;
		ProcessNode(true,"Enter Spec: ", Ctx);
	}
	
	void
	MyXdrListener::exitXdrSpecification(xdrParser::XdrSpecificationContext *Ctx)
	{
		//std::cout << "In: exitXdrSpecification" << std::endl;
		ProcessNode(false, "Exit Spec: ", Ctx);
	}

	void
	MyXdrListener::enterVar(xdrParser::VarContext *Ctx)
	{
		//std::cout << "In: enterVar" << std::endl;
		ProcessNode(false, "Enter Var: ", Ctx);
	}

	void
	MyXdrListener::exitVar(xdrParser::VarContext *Ctx)
	{
		//std::cout << "In: exitVar" << std::endl;
		ProcessNode(false, "Exit Var: ", Ctx);
	}

	void
	MyXdrListener::enterVarPtr(xdrParser::VarPtrContext *Ctx)
	{
		//std::cout << "In: enterVarPtr" << std::endl;
		ProcessNode(false, "Enter VarPtr: ", Ctx);
	}

	void
	MyXdrListener::exitVarPtr(xdrParser::VarPtrContext *Ctx)
	{
		//std::cout << "In: exitVarPtr" << std::endl;
		ProcessNode(false, "Exit VarPtr: ", Ctx);
	}

	void
	MyXdrListener::enterVarFixed(xdrParser::VarFixedContext *Ctx)
	{
		//std::cout << "In: enterVarFixed" << std::endl;
		ProcessNode(false, "Enter VarFixed: ", Ctx);
	}

	void
	MyXdrListener::exitVarFixed(xdrParser::VarFixedContext *Ctx)
	{
		//std::cout << "In: exitVarFixed" << std::endl;
		ProcessNode(false, "Exit VarFixed: ", Ctx);
	}

	void
	MyXdrListener::enterVarFixedPtr(xdrParser::VarFixedPtrContext *Ctx)
	{
		//std::cout << "In: enterVarFixedPtr" << std::endl;
		ProcessNode(false, "Enter VarFixedPtr: ", Ctx);
	}

	void
	MyXdrListener::exitVarFixedPtr(xdrParser::VarFixedPtrContext *Ctx)
	{
		//std::cout << "In: exitVarFixedPtr" << std::endl;
		ProcessNode(false, "Exit VarFixedPtr: ", Ctx);
	}

	void
	MyXdrListener::enterVarVariable(xdrParser::VarVariableContext *Ctx)
	{
		//std::cout << "In: enterVarVariable" << std::endl;
		ProcessNode(false, "Enter VarVariable: ", Ctx);
	}

	void
	MyXdrListener::exitVarVariable(xdrParser::VarVariableContext *Ctx)
	{
		//std::cout << "In: exitVarVariable" << std::endl;
		ProcessNode(false, "Exit VarVariable: ", Ctx);
	}

	void
	MyXdrListener::enterVarVariablePtr(xdrParser::VarVariablePtrContext *Ctx)
	{
		//std::cout << "In: enterVarVariablePtr" << std::endl;
		ProcessNode(false, "Enter VarVariablePtr: ", Ctx);
	}

	void
	MyXdrListener::exitVarVariablePtr(xdrParser::VarVariablePtrContext *Ctx)
	{
		//std::cout << "In: exitVarVariablePtr" << std::endl;
		ProcessNode(false, "Exit VarVariablePtr: ", Ctx);
	}

	void
	MyXdrListener::enterOpaqueFixed(xdrParser::OpaqueFixedContext *Ctx)
	{
		//std::cout << "In: enterOpaqueFixed" << std::endl;
		ProcessNode(false, "Enter OpaqueFixed: ", Ctx);
	}

	void
	MyXdrListener::exitOpaqueFixed(xdrParser::OpaqueFixedContext *Ctx)
	{
		//std::cout << "In: exitOpaqueFixed" << std::endl;
		ProcessNode(false, "Exit OpaqueFixed: ", Ctx);
	}

	void
	MyXdrListener::enterOpaqueFixedPtr(xdrParser::OpaqueFixedPtrContext *Ctx)
	{
		//std::cout << "In: enterOpaqueFixedPtr" << std::endl;
		ProcessNode(false, "Enter OpaqueFixedPtr: ", Ctx);
	}

	void
	MyXdrListener::exitOpaqueFixedPtr(xdrParser::OpaqueFixedPtrContext *Ctx)
	{
		//std::cout << "In: exitOpaqueFixedPtr" << std::endl;
		ProcessNode(false, "Exit OpaqueFixedPtr: ", Ctx);
	}

	void
	MyXdrListener::enterOpaqueVariable(xdrParser::OpaqueVariableContext *Ctx)
	{
		//std::cout << "In: enterOpaqueVariable" << std::endl;
		ProcessNode(false, "Enter OpaqueVariable: ", Ctx);
	}

	void
	MyXdrListener::exitOpaqueVariable(xdrParser::OpaqueVariableContext *Ctx)
	{
		//std::cout << "In: exitOpaqueVariable" << std::endl;
		ProcessNode(false, "Exit OpaqueVariable: ", Ctx);
	}

	void
	MyXdrListener::enterOpaqueVariablePtr(xdrParser::OpaqueVariablePtrContext *Ctx)
	{
		//std::cout << "In: enterOpaquePtrVariable" << std::endl;
		ProcessNode(false, "Enter OpaquePtrVariable: ", Ctx);
	}

	void
	MyXdrListener::exitOpaqueVariablePtr(xdrParser::OpaqueVariablePtrContext *Ctx)
	{
		//std::cout << "In: exitOpaquePtrVariable" << std::endl;
		ProcessNode(false, "Exit OpaquePtrVariable: ", Ctx);
	}

	void
	MyXdrListener::exitString(xdrParser::StringContext *Ctx)
	{
		//std::cout << "In: exitString" << std::endl;
		ProcessNode(false, "Exit String: ", Ctx);
	}

	void
	MyXdrListener::enterString(xdrParser::StringContext *Ctx)
	{
		//std::cout << "In: enterString" << std::endl;
		ProcessNode(false, "Enter String: ", Ctx);
	}

	void
	MyXdrListener::enterStringPtr(xdrParser::StringPtrContext *Ctx)
	{
		//std::cout << "In: enterStringPtr" << std::endl;
		ProcessNode(false, "Enter StringPtr: ", Ctx);
	}

	void
	MyXdrListener::exitStringPtr(xdrParser::StringPtrContext *Ctx)
	{
		//std::cout << "In: exitStringPtr" << std::endl;
		ProcessNode(false, "Exit StringPtr: ", Ctx);
	}

	void
	MyXdrListener::enterVoid(xdrParser::VoidContext *Ctx)
	{
		//std::cout << "In: Void" << std::endl;
		ProcessNode(false, "Enter Void: ", Ctx);
	}

	void
	MyXdrListener::exitVoid(xdrParser::VoidContext *Ctx)
	{
		//std::cout << "In: exitVoid" << std::endl;
		ProcessNode(false, "Exit Void: ", Ctx);
	}

	void
	MyXdrListener::visitTerminal(tree::TerminalNode * Node)
	{
		//std::cout << "In: visitTerminal" << std::endl;
		ProcessNode(true,"Visit Terminal: ", Node);
	}
	
	void
	MyXdrListener::visitErrorNode(tree::ErrorNode * Node)
	{
		//std::cout << "In: visitErrorNode" << std::endl;
		ProcessNode(true, "Visit Error : ", Node);
	}

	void
	MyXdrListener::enterEveryRule(ParserRuleContext * Ctx)
	{
		//std::cout << "In: enterEveryRule" << std::endl;
		//ProcessNode(true,"Enter Every Rule: ", Ctx);
	}
	
	void
	MyXdrListener::exitEveryRule(ParserRuleContext * Ctx)
	{
		//std::cout << "In: exitEveryRule" << std::endl
		//ProcessNode(false, "Exit Every Rule: ", Ctx);
	}
	
	void
	MyXdrListener::enterNamespaceDef(xdrParser::NamespaceDefContext * Ctx)
	{
		//std::cout << "In: visitErrorNode" << std::endl;
		ProcessNode(true, "Namespace Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitNamespaceDef(xdrParser::NamespaceDefContext * Ctx)
	{
		//std::cout << "In: visitErrorNode" << std::endl;
		ProcessNode(false, "Namespce Exit : ", Ctx);
	}
	
	void
	MyXdrListener::enterTypedefDef(xdrParser::TypedefDefContext * Ctx)
	{
		//std::cout << "In: visitErrorNode" << std::endl;
		ProcessNode(true, "Typedef Enter : ", Ctx);
	}
	
	void
	MyXdrListener::exitTypedefDef(xdrParser::TypedefDefContext * Ctx)
	{
		//std::cout << "In: visitErrorNode" << std::endl;
		ProcessNode(false, "TypedefExit : ", Ctx);
	}
	
	void
	MyXdrListener::enterSpecs(xdrParser::SpecsContext * Ctx)
	{
		//std::cout << "In: Node" << std::endl;
		ProcessNode(true, "SpecsExit : ", Ctx);
	}
	
	void
	MyXdrListener::exitSpecs(xdrParser::SpecsContext * Ctx)
	{
		//std::cout << "In: exitSpecs" << std::endl;
		ProcessNode(false, "Specs Exit : ", Ctx);
	}
	
	void
	MyXdrListener::enterComment(xdrParser::CommentContext * Ctx)
	{
		//std::cout << "In: Enter Comment" << std::endl;
		ProcessNode(true, "Comment Exit : ", Ctx);
	}
	
	void
	MyXdrListener::exitComment(xdrParser::CommentContext * Ctx)
	{
		//std::cout << "In: exitComment" << std::endl;
		ProcessNode(false, "Comment Exit : ", Ctx);
	}
	
	Constant::~Constant()
	{
	}
	
	EnumValue::~EnumValue()
	{
	}
	
	TypeDef::~TypeDef()
	{
	}
	
	StructMember::~StructMember()
	{
	}
	
	Struct::~Struct()
	{
	}

	UnionCase::~UnionCase()
	{
	}

	Union::Union()
	{
		Default = nullptr;

		return;
	}
	
	Union::~Union()
	{
		if (Default != nullptr) {
			delete Default;
			Default = nullptr;
		}

		return;
	}
	
	Procedure::~Procedure()
	{
	}
	
	Version::~Version()
	{
	}
	
	Program::~Program()
	{
	}
	
} // End namespace RiverExplorer::rpcgen

void
Usage(const char * Pgm)
{
	std::cout << std::endl
						<< "Usage: "
						<< std::endl
						<< basename(Pgm)
						<< " [--quiet]"
						<< " --input <filename.x>"
						<< " --outdir Dir"
						<< " --lang (CPP|C#)"
						<< " (-all or one or more of: --xdr, --xds, --headers, --abnf, --stubs)"
						<< std::endl;

	return;
}

using namespace RiverExplorer::rpcgen;

int
main(int argc, char *argv[])
{
	string line;

	int Opt;

	std::string Input;
	std::string OutDir;
	std::string Language;
	
	bool GenerateAll = false;
	bool GenerateXdr = false;
	bool GenerateHeaders = false;
	bool GenerateStubs = false;
	bool GenerateXsd = false;
	bool GenerateAbnf = false;

	bool Error = false;

	const int NoArgument = 0;
	const int RequiredArgument = 1;
	const int OptionalArgument = 2;
	
	while (1) {
		int ThisOpt = optind ? optind : 1;
		int OptIndex = 0;

		static struct option LongOpts[] = {
			{"input",		RequiredArgument,	0, 0},
			{"outdir",	RequiredArgument,	0, 0},
			{"all",			NoArgument,				0, 0},
			{"lang",		RequiredArgument,	0, 0},
			{"xdr",			NoArgument,				0, 0},
			{"xsd",			NoArgument,				0, 0},
			{"headers",	NoArgument,				0, 0},
			{"abnf",		NoArgument,				0, 0},
			{"stubs",		NoArgument,				0, 0},
			{"quiet",		NoArgument,				0, 0}
		};
		
		Opt = getopt_long(argc, argv, "",
											LongOpts, &OptIndex);

		if (Opt == -1) {
			break;
		}

		switch (OptIndex) {

		case 0: // Input
			if (optarg) {
				Input = optarg;
				InputFileName = Input;
				InputNoExtension = RemoveFileExtension(Input);
				
				if (!RunQuiet) {
					std::cout << "Reading from input: " << Input << std::endl;
				}
			}
			break;

		case 1: // OutDir
			if (optarg) {
				OutDir = optarg;
				OutputDirectory = OutDir;
				if (!RunQuiet) {
					std::cout << "Writing to directory: " << OutDir << std::endl;
				}
			}
			break;

		case 2: // All
			GenerateAll = true;
			if (!RunQuiet) {
				std::cout << "Generating Everyting. " << OutDir << std::endl;
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
			break;

		case 5: // XSD
			GenerateXsd = true;
			break;

		case 6: // Headers.
			GenerateHeaders = true;
			break;

		case 7: // ABNF
			GenerateAbnf = true;
			break;

		case 8: // Stubs
			GenerateStubs = true;
			break;

		case 9: // Quiet
			RunQuiet = true;
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
	if (!GenerateAll && (!GenerateXdr
											 && !GenerateXsd
											 && !GenerateHeaders
											 && !GenerateAbnf
											 && !GenerateStubs)) {
		std::cout << "\nError: Must have one of -all or one or more of: --xdr, --xds, --headers, --abnf, --stubs\n"
							<< std::endl;
		Error = true;

	}

	if (OutDir == "") {
		std::cout << "\nError: Must specific output directory --outdir <dir>"
							<< std::endl;
		Error = true;
	}

	if (Language == "") {
		std::cout << "\nError: Must specific a target language."
							<< "'CPP', and 'C#' supported."
							<< std::endl;
		Error = true;
	}

	if (Error) {
		Usage(argv[0]);
		exit(1);
	}

	if (access(OutDir.c_str(), W_OK|R_OK|X_OK) != 0) {
		if (!RunQuiet) {
			std::cout << "Output directory: '"
								<< OutDir
								<< " 'does not exist, or can't access it."
								<< " Attepting to fix."
								<< std::endl;
		}
		chmod(OutDir.c_str(), 0700);
		if (access(OutDir.c_str(), W_OK|R_OK|X_OK) != 0) {
			mkdir(OutDir.c_str(), 0700);

			if (access(OutDir.c_str(), W_OK|R_OK|X_OK) != 0) {
				std::cout << "ERROR: Can not create or fix output directory: '"
									<< OutDir
									<< "'"
									<< std::endl;
				exit(2);
			}
		}
	}

	ifstream xdrFile (Input);
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
				if (Namespace != "") {
					CppOutputDirectory += "/";
					CppOutputDirectory += NamespaceToIncludePath();
				}
				CppOutputDirectory += "/c++";
				MakePath(CppOutputDirectory);
			
				if (GenerateHeaders || GenerateAll) {
					// Generate the header files.
					//
				
					if (GenerateHeaders || GenerateAll) {

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

						Header << "/**" << std::endl;
						GenerateThisFileWasGenerated(" * ", Header);
						Header << " */" << std::endl << std::endl;
							
						Define = "_RIVEREXPLORER_RPCGEN_";

						Define += ToUpper(InputNoExtension);
						Define += "_X_HPP_";
						Header << "#ifndef " << Define << std::endl;
						Header << "#define " << Define << std::endl;
						Header << std::endl;

						Header << "#include <RpcgenShared.hpp>" << std::endl;
						Header << "#include <string>" << std::endl;
						Header << "#include <vector>" << std::endl;
						Header << "#include <map>" << std::endl;
						Header << "#include <rpc/rpc.h>" << std::endl;
						Header << "#ifndef W64" << std::endl;
						Header << "#include <unistd.h>" << std::endl;
						Header << "#endif // W64" << std::endl;
						Header << "#include <memory.h>" << std::endl;
						
						if (Namespace != "") {
							Header << std::endl;
							Header << "namespace " << NamespaceToCppNamespace()
										 << std::endl << "{" << std::endl;
							IndentLevel++;
						}
						for (ItemIt = OrderedItems.cbegin()
									 ; ItemIt != OrderedItems.cend()
									 ; ItemIt++) {
							
							OneItem = *ItemIt;
							OneItem->PrintCppHeader(Header);
						}
						if (Namespace != "") {
							IndentLevel--;
							Header << "} // End namespace "
										 << NamespaceToCppNamespace()<< std::endl;
						}
						Header << std::endl << "#endif // " << Define << std::endl;
						Header.close();
					}
				}
			}
			if (GenerateXdr || GenerateAll) {

				std::string XdrFileFile = CppOutputDirectory;

				XdrFileFile += "/";
				XdrFileFile += InputNoExtension;
				XdrFileFile += "_xdr.cpp";

				ofstream XdrFile(XdrFileFile);

				XdrFile << "/**" << std::endl;
				GenerateThisFileWasGenerated(" * ", XdrFile);
				XdrFile << " */" << std::endl << std::endl;

				XdrFile << "#include \"" << InputNoExtension
								<< ".hpp\"" << std::endl;
						
				if (Namespace != "") {
					XdrFile << std::endl;
					XdrFile << "namespace " << Namespace
									<< std::endl << "{" << std::endl;
					IndentLevel++;
				}
				for (ItemIt = OrderedItems.cbegin()
							 ; ItemIt != OrderedItems.cend()
							 ; ItemIt++) {
							
					OneItem = *ItemIt;
					OneItem->PrintCppXDR(XdrFile);
				}
				if (Namespace != "") {
					IndentLevel--;
					XdrFile << "} // End namespace " << Namespace << std::endl;
				}
				XdrFile.close();
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
