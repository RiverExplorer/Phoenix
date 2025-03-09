/**
 * Project: Phoenix
 * Time-stamp: <2025-03-08 22:09:36 doug>
 * 
 * @file rpcgen.cpp
 * @author Douglas Mark Royer
 * @date 08-MAR-20205
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#include "rpcgen.hpp"
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
	int										IndentLevel = 0;
	
	bool									RunQuiet = false;
	std::string						InputFileName;
	std::string						InputNoExtension;
	std::string						OutputDirectory;
	std::string						ThisFileWasGenerated;
	
	std::string						Namespace;
	std::vector<Item*>		OrderedItems;
	State									CurrentState = Unknown;
	bool									InArray = false;
	Constant						*	CurrentConstant = nullptr;
	StructMember				*	CurrentStructMember = nullptr;
	Struct							*	CurrentStruct = nullptr;
	Union								*	CurrentUnion = nullptr;

	static std::vector<std::string> XdrBuiltInTypes = {
		"bool",
		"char",
		"double",
		"emum",
		"float",
		"int",
		"int16_t",
		"int64_t",
		"int8_t",
		"long",
		"opaque",
		"short",
		"string",
		"u_char",
		"u_int",
		"uint16_t",
		"uint64_t",
		"uint8_t",
		"u_long",
		"u_short"
	};
	
	bool IsBuiltInXdrType(const std::string & Type)
	{
		return(std::binary_search(XdrBuiltInTypes.begin(),
															XdrBuiltInTypes.end(),
															Type));
	}
	
	std::string
	Indent()
	{
		static std::string LastResults;
		static int LastIndent = 10000;
		
		std::string Results;

		if (IndentLevel != LastIndent) {
			int Tmp = IndentLevel;

			while (Tmp-- > 0) {
				Results += '\t';
			}
			LastIndent = IndentLevel;
			LastResults = Results;
		} else {
			Results = LastResults;
		}

		return(LastResults);
	}
	
	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::DeclarationContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::ConstantContext* Ctx)
	{
		std::string Text = Ctx->getText();

		//std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::TypeSpecifierContext* Ctx)
	{
		std::string Text = Ctx->getText();

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::EnumTypeSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::EnumBodyContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::StructTypeSpecContext* Ctx)
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
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::StructBodyContext* Ctx)
	{
		std::string Text = Ctx->getText();

		if (Enter) {
			CurrentState = InStructBody;

		} else {

			CurrentState = Unknown;
		}
		//std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::UnionTypeSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::UnionBodyContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::CaseSpecContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::TypeDefContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::ConstantDefContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::ValueContext* Ctx)
	{
		std::string Text = Ctx->getText();

		//std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::XdrSpecificationContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, tree::TerminalNode* Ctx)
	{

		std::string Text = Ctx->getText();

		switch (CurrentState) {

		case InStruct:
			/*EMPTY*/
			break;

		case InNamespaceDef:
			if (Text != "namespace" && Text != ";") {
				Namespace = Text;
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
					// Ignore
					
				} else if (Text == "}" ) {
					// Ignore

				} else if (Text == "struct") {
					// Ignore
					
				} else if (Text == CurrentStruct->Name) {
					// Ignore
					
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

		default:
			//std::cout << From << Text << std::endl;
			break;
		}

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::DefinitionContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, ParserRuleContext* Ctx)
	{
		std::string Text = Ctx->getText();

		std::cout << From << Text << std::endl;

		return;
	}

	void
	MyXdrListener::ProcessNode(bool Enter, std::string From, xdrParser::NamespaceDefContext  * Ctx)
	{
		if (Enter) {
			CurrentState = InNamespaceDef;
		} else {
			CurrentState = Unknown;
		}

		return;
	}
	
	///////////////////////////////////////////////////////////////////////////
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
		//std::cout << "In: enterTypeSpecifier" << std::endl;
		ProcessNode(true, "Enter TypeSpecifier: ", Ctx);
	}

	void
	MyXdrListener::exitTypeSpecifier(xdrParser::TypeSpecifierContext *Ctx)
	{
		//std::cout << "In: exitTypeSpecifier" << std::endl;
		ProcessNode(false, "Exit TypeSpecifier: ", Ctx);
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
		//std::cout << "In: exitEveryRule" << std::endl;
		//ProcessNode(false, "Exit Every Rule: ", Ctx);
	}
	
	void
	MyXdrListener::enterMain(xdrParser::MainContext * Ctx)
	{
		//std::cout << "In: enterMain" << std::endl;
		//ProcessNode(true,"Enter Main: ", Ctx);
	}
	
	void
	MyXdrListener::exitMain(xdrParser::MainContext * Ctx)
	{
		//std::cout << "In: exitMain" << std::endl;
		//ProcessNode(false, "Exit Main: ", Ctx);
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
	
	Union::~Union()
	{
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

static std::string
ToUpper(const std::string & In)
{
	string Results = In;
	
	std::transform(Results.begin(),
								 Results.end(),
								 Results.begin(),
								 [](unsigned char c) {return std::toupper(c);}
								 );

	return(Results);
}

static void
GenerateThisFileWasGenerated(std::string Prefix,
														 ofstream & Stream)
{
	Stream << Prefix << "DO NOT EDIT THIS FILE" << std::endl;
	Stream << Prefix << std::endl;
	Stream << Prefix << "It was generated from: " << InputFileName << std::endl;
	Stream << Prefix << "Using RiverExplorer rpcgen++ program." << std::endl;
	Stream << Prefix << "Available at: https://github.com/Phoenix." << std::endl;

	return;
}

static std::string
RemoveFileExtension(std::string FileName)
{
	std::string Results;
	
	size_t	LastDot = FileName.find_last_of('.');

	if (LastDot == std::string::npos) {
		Results = FileName;
	} else {
		Results = FileName.substr(0, LastDot);
	}

	return(Results);
}

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
		tree::ParseTree * Tree = parser.main();

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
			
			if (GenerateHeaders || GenerateAll) {
				// Generate the header files.
				//
				
				if (Language == "CPP" ) {
					if (GenerateHeaders || GenerateAll) {

						// SHARED header data.
						//
						std::string SharedFileName = OutputDirectory;
						SharedFileName += "/";
						SharedFileName += "RpcgenShared.hpp";

						if (access(SharedFileName.c_str(), R_OK) != F_OK) {
							ofstream Shared(SharedFileName);

							Shared << "/**" << std::endl;
							GenerateThisFileWasGenerated(" * ", Shared);
							Shared << " */" << std::endl << std::endl;
							
							Define = "_RIVEREXPLORER_RPCGEN_SHARED_";

							Define += ToUpper(InputNoExtension);
							Define += "_X_HPP_";
							Shared << "#ifndef " << Define << std::endl;
							Shared << "#define " << Define << std::endl;
							Shared << std::endl;

							Shared << "#include <string>" << std::endl;
							Shared << "#include <vector>" << std::endl;
							Shared << "#include <map>" << std::endl;
							Shared << "#include <rpc/rpc.h>" << std::endl;
							Shared << "#ifndef W64" << std::endl;
							Shared << "#include <unistd.h>" << std::endl;
							Shared << "#endif // W64" << std::endl;
								Shared << "#include <memory.h>" << std::endl;
						
								if (Namespace != "") {
									Shared << std::endl;
									Shared << "namespace " << Namespace
												 << std::endl << "{" << std::endl;
									IndentLevel++;
								}
								std::string I1 = Indent();
						
								Shared << I1 << "/**" << std::endl;
								Shared << I1 << "* An implementation of an XDR routine" << std::endl;
								Shared << I1 << "* for std::vector<uint8_t>." << std::endl;
								Shared << I1 << "*" << std::endl;
								Shared << I1 << "* @param Xdr The initalized XDR object." << std::endl;
								Shared << I1 << "*" << std::endl;
								Shared << I1 << "* @param opaque The address of the std::vector<uint8_t> object." << std::endl;
								Shared << I1 << "*" << std::endl;
								Shared << I1 << "* @param[optional] MaxLength The maximum size of the vector." << std::endl;
								Shared << I1 << "* @return true on no errors." << std::endl;
								Shared << I1 << "*/" << std::endl;
								Shared << I1 << "bool xdr_VectorOfOpaque(XDR * Xdr," << std::endl;
								Shared << I1 << "\tstd::vector<uint8_t> * Opaque," << std::endl;
								Shared << I1 << "\tuint32_t MaxLength = ~0);" << std::endl;
								Shared << I1 << "" << std::endl;
								Shared << I1 << "" << std::endl;
								Shared << I1 << "/**" << std::endl;
								Shared << I1 << " * An implementation of an XDR routine" << std::endl;
								Shared << I1 << " * for std::vector<uint8_t>." << std::endl;
								Shared << I1 << " *" << std::endl;
								Shared << I1 << " * @param Xdr The initalized XDR object." << std::endl;
								Shared << I1 << " *" << std::endl;
								Shared << I1 << " * @param Str The address of the std::string." << std::endl;
								Shared << I1 << " *" << std::endl;
								Shared << I1 << " * @param[optional] MaxLength The maximum size of the string." << std::endl;
								Shared << I1 << " * @return true on no errors." << std::endl;
								Shared << I1 << " */" << std::endl;
								Shared << I1 << "bool xdr_StdString(XDR * Xdr," << std::endl;
								Shared << I1 << "\tstd::string * Str," << std::endl;
								Shared << I1 << "\tuint32_t MaxLength = ~0);" << std::endl;
								Shared << I1 << "" << std::endl;
								Shared << I1 << "/**" << std::endl;
								Shared << I1 << " * An implementation of an XDR routine" << std::endl;
								Shared << I1 << " * for std::vector<uint8_t>." << std::endl;
								Shared << I1 << " *" << std::endl;
								Shared << I1 << " * @param Xdr The initalized XDR object." << std::endl;
								Shared << I1 << " *" << std::endl;
								Shared << I1 << " * @param Obj A pointer to the object." << std::endl;
								Shared << I1 << " *" << std::endl;
								Shared << I1 << " * @param Proc the xdrproc_t routine that can" << std::endl;
								Shared << I1 << " * handle type 'T'." << std::endl;
								Shared << I1 << " *" << std::endl;
								Shared << I1 << " * @param[optional] MaxLength The maximum size of the string." << std::endl;
								Shared << I1 << " * @return true on no errors." << std::endl;
								Shared << I1 << " */" << std::endl;
								Shared << I1 << "template <class T>" << std::endl;
								Shared << I1 << "bool xdr_VectorOf(XDR * Xdr," << std::endl;
								Shared << I1 << "\tstd::vector<T> * Obj," << std::endl;
								Shared << I1 << "\txdrproc_t Proc," << std::endl;
								Shared << I1 << "\tuint32_t MaxLength = ~0)" << std::endl;
								Shared << I1 << "{" << std::endl;
								Shared << I1 << "\tbool Results = false;" << std::endl;
								Shared << I1 << "" << std::endl;
								Shared << I1 << "\tif (Obj != nullptr && Proc != nullptr) {" << std::endl;
								Shared << I1 << "\t\t\tuint32_t Size = Obj->size();" << std::endl;
								Shared << I1 << "" << std::endl;
								Shared << I1 << "\t\tif (xdr_uint32_t(Xdr, &Size)) {" << std::endl;
								Shared << I1 << "" << std::endl;
								Shared << I1 << "\t\t\ttypename std::vector<T>::iterator it;" << std::endl;
								Shared << I1 << "\t\t\tT Item;" << std::endl;
								Shared << I1 << "\t\t\tuint32_t Offset = 0;" << std::endl;
								Shared << I1 << "\t\t\tfor (it = Obj->begin(); it != Obj->end(); it++) {" << std::endl;
								Shared << I1 << "\t\t\t\tItem = *it;" << std::endl;
								Shared << I1 << "\t\t\t\tResults = (*Proc)(Xdr, &Item);" << std::endl;
								Shared << I1 << "\t\t\t\tif (!Results) {" << std::endl;
								Shared << I1 << "\t\t\t\t\tbreak;" << std::endl;
								Shared << I1 << "\t\t\t\t}" << std::endl;
								Shared << I1 << "\t\t\t\tif (++Offset > MaxLength) {" << std::endl;
								Shared << I1 << "\t\t\t\t\tbreak;" << std::endl;
								Shared << I1 << "\t\t\t\t}" << std::endl;
								Shared << I1 << "\t\t\t}" << std::endl;
								Shared << I1 << "\t\t}" << std::endl;
								Shared << I1 << "\t}" << std::endl;
								Shared << I1 << "" << std::endl;
								Shared << I1 << "\treturn(Results);" << std::endl;
								Shared << I1 << "}" << std::endl;
								if (Namespace != "") {
									IndentLevel--;
									Shared << "} // End namespace " << Namespace << std::endl;
								}
								Shared << std::endl << "#endif // " << Define << std::endl;
								Shared.close();
						}
						
						std::string HeaderFile = OutputDirectory;

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
							Header << "namespace " << Namespace
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
							Header << "} // End namespace " << Namespace << std::endl;
						}
						Header << std::endl << "#endif // " << Define << std::endl;
						Header.close();
					}
				}
			}
			if (GenerateXdr || GenerateAll) {

				std::string XdrFileFile = OutputDirectory;

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
