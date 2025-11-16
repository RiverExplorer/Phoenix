/**
 * Project: Phoenix
 * Time-stamp: <2025-03-31 16:33:54 doug>
 * 
 * @file protocolgen.hpp
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
#ifndef _RIVEREXPLORER_XDRGEN_XDRGEN_HPP_
#define _RIVEREXPLORER_XDRGEN_XDRGEN_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include "antlr4-runtime.h"
#include "xdrLexer.h"
#include "xdrParser.h"
#include "xdrListener.h"
#include "xdrBaseListener.h"

#include <rpc/rpc.h>

#include "GenerateCpp.hpp"

using namespace antlr4;
using namespace std;

namespace RiverExplorer::protocolgen
{
	/**
	 * The input file name.
	 */
	extern std::string InputFileName;

	/**
	 * Used sometimes for the target output directory.
	 */
	extern std::string CppOutputDirectory;
	
	/**
	 * The input file name without extension.
	 */
	extern std::string InputNoExtension;
	
	/**
	 * The input file name without extension.
	 */
	extern std::string OutputDirectory;

	/**
	 * Shared 'this file was generated ... message.
	 */
	extern std::string ThisFileWasGenerated;
	
	/**
	 * When true, run a quiet as possibe.
	 * Error are still printed.
	 */
	extern bool RunQuiet;

	/**
	 * When not empty, everything goes into this namespace.
	 */
	extern std::string Namespace;
	
	/**
	 * Namespace converted to 'part1::part2::...' format.
	 */
	extern std::string CppNamespace;
	
	/**
	 * Indentation level used when generating code.
	 */
	extern int IndentLevel;

	/**
	 * Test if Type is a built n XDR type.
	 * (int, unsigned int, string, opaque, ...)
	 *
	 * @param Type The type name to test.
	 *
	 * @return true if Type is a known built in type.
	 */
	extern bool IsBuiltInXdrType(const std::string & Type);
	 
	/**
	 * Generate a string given IndenLevel.
	 *
	 * @return The indent string.
	 */
	extern std::string Indent();
	
	/**
	 * Generate a string given Level.
	 *
	 * @param Leven How many levels of indent.
	 *
	 * @return The indent string.
	 */
	extern std::string Indent(int Level);
	
	enum State	{
		Unknown,
		InVar,
		InVarPtr,
		InVarFixed,
		InVarFixedPtr,
		InVarVariable,
		InVarVariablePtr,
		InOpaqueFixed,
		InOpaqueFixedPtr,
		InOpaqueVariable,
		InOpaqueVariablePtr,
		InString,
		InStringPtr,
		InNamespaceDef,
		InStruct,
		InStructBody,
		InTypeDef,
		InUnion,
		InUnionCase,
		InProcedureDef,
		InVoid,
		InComment,
		InPassThrough,
		InEnum,
		InEnumBody,
		InConstant,
		InMethod,
		InProgram,
		InVersion,
		InVersionMethod,
		InDeclaration,
		InEnumTypeSpec,
		InStructTypeSpec,
		InUnionTypeSpec
	};

	/**
	 * @class Item
	 * This exists as a base class for evertying, so
	 * we can put them into one vector and keep the
	 * original objcet order.
	 */
	class Item
	{
	public:
		
		virtual void PrintCppHeader(ofstream & Stream) const = 0;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const = 0;
		virtual void PrintCppXDR(ofstream & Steam) const = 0;
		virtual void PrintCppStubs(ofstream & Stream) const = 0;
		virtual void PrintXSD(ofstream & Stream) const = 0;
		virtual void PrintAbnf(ofstream & Stream) const = 0;
		virtual void PrintServer(ofstream & Stream) const = 0;
		
		bool IsPointer = false;
		bool IsReference = false;
		bool IsFixedArray = false;
		bool IsVariableArray = false;
		std::string SizeOrValue;

		std::string Type;
		std::string Name;

		/**
		 * Print the variable for a CPP Header.
		 */
		void PrintCppDeclareVariable(ofstream & Stream) const;
	};

	extern std::vector<Item*> OrderedItems;

	extern State CurrentState;
	extern bool InArray;

	/**
	 * @class Constant protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Constant
		: public Item
	{
	public:

		virtual ~Constant();

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};
	extern Constant * CurrentConstant;

	/**
	 * @class EnumValue protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class EnumValue
		: public Item
	{
	public:

		virtual ~EnumValue();

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class Enum protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Enum
		: public Item
	{
	public:

		virtual ~Enum();

		std::vector<Item*> Enums;
		
		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class Variable protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Variable
		: public Item
	{
	public:

		virtual ~Variable();
		
		void PrintCppHeader(ofstream & Stream, bool PrintExtern) const;
		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class TypeDef protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class TypeDef
		: public Item
	{
	public:

		virtual ~TypeDef();

		Variable * Declaration;
		
		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class StructMember protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class StructMember
		: public Item
	{
	public:

		virtual ~StructMember();

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};
	extern StructMember * CurrentStructMember;

	/**
	 * @class Struct protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Struct
		: public Item
	{
	public:

		virtual ~Struct();
		
		std::vector<Item*> Members;

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};
	extern Struct * CurrentStruct;

	/**
	 * @class UnionCase protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class UnionCase
		: public Item
	{
	public:

		virtual ~UnionCase();
		
		std::string CaseValue;
		bool IsDefault;

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class Union protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Union
		: public Item
	{
	public:

		Union();
		virtual ~Union();
		
		std::string Name;
		std::string SwitchType;
		std::string SwitchVariable;
		std::vector<Item*> Cases;
		UnionCase * Default;
		
		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};
	extern Union * CurrentUnion;

	/**
	 * @class Procedure protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Procedure
		: public Item
	{
	public:

		virtual ~Procedure();
		
		std::string ReturnType;
		std::vector<Item*> Params;
		int64_t ProcNumber;

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class Program protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Program
		: public Item
	{
	public:

		virtual ~Program();
		
		std::vector<Item*> Versions;
		std::map<std::string, Constant*> Constants;
		std::map<std::string, std::vector<Item*>*> Enums;
		std::map<std::string, TypeDef*> TypeDefs;
		std::map<std::string, Struct*> Structs;
		std::map<std::string, Union*> Unions;

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class Version protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Version
		: public Item
	{
	public:

		Version(Program & Parent);
		virtual ~Version();
		
		std::vector<Item*> Procedures;

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;

		Program & Parent;
	};

	/**
	 * @class Comment protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class Comment
		: public Item
	{
	public:
		
		virtual ~Comment();
		
		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class PassThrough protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 */
	class PassThrough
		: public Item
	{
	public:
		
		virtual ~PassThrough();

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;
	};

	/**
	 * @class Method protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 * A 'Method' is of the form:
	 *
	 *   returnType MethodName(Parameter1, ...);
	 *
	 * Or if wihin a program/version:
	 *
	 *   returnType MethodName(Parameter1, ...) = value;
	 */
	class Method
		: public Item
	{
	public:

		Method(Struct & Parent);
		virtual ~Method();

		std::vector<std::string> Parameters;

		/**
		 * Only used inside of a program/version.
		 */
		std::string VersionValue;

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;

		Struct & Parent;
	};
	
	/**
	 * @class Method protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 * A 'Method' is of the form:
	 *
	 *   returnType MethodName(Parameter1, ...);
	 *
	 * Or if wihin a program/version:
	 *
	 *   returnType MethodName(Parameter1, ...) = value;
	 */
	class VersionMethod
		: public Item
	{
	public:

		VersionMethod(Version & Parent);
		virtual ~VersionMethod();

		std::vector<std::string> Parameters;

		/**
		 * Only used inside of a program/version.
		 */
		std::string MethodValue;

		virtual void PrintCppHeader(ofstream & Stream) const;
		virtual void PrintCppHeaderXdr(ofstream & Stream) const;
		virtual void PrintCppXDR(ofstream & Stream) const;
		virtual void PrintCppStubs(ofstream & Stream) const;
		virtual void PrintXSD(ofstream & Stream) const;
		virtual void PrintAbnf(ofstream & Stream) const;
		virtual void PrintServer(ofstream & Stream) const;

		Version & Parent;
	};
	
	/**
	 * @class MyXdrListener protocolgen.hpp <RiverExplorer/protocolgen/protocolgen.hpp>
	 * A support class for the antlr4 generated code.
	 */
	class MyXdrListener
		: public xdrListener
	{
	public:
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::ConstantContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::TypeSpecifierContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::EnumTypeSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::EnumBodyContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::StructTypeSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::StructBodyContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::UnionTypeSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::UnionBodyContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::CaseSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::TypeDefContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::ConstantDefContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::ValueContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::XdrSpecificationContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::NamespaceDefContext  * Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::DefinitionContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::SpecsContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::CommentContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::PassThroughContext* Ctx);

		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::MethodContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::ProcFirstArgContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::ProgramContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VersionContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VersionMethodContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 tree::TerminalNode* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 ParserRuleContext* Ctx);
		
		//////////////////////////////////////////////////////////////////////
		virtual void enterValue(xdrParser::ValueContext *Ctx);
		virtual void exitValue(xdrParser::ValueContext *Ctx);
		
		virtual void enterConstant(xdrParser::ConstantContext *Ctx);
		virtual void exitConstant(xdrParser::ConstantContext *Ctx);
		
		virtual void enterTypeSpecifier(xdrParser::TypeSpecifierContext *Ctx);
		virtual void exitTypeSpecifier(xdrParser::TypeSpecifierContext *Ctx);
		
		virtual void enterEnumTypeSpec(xdrParser::EnumTypeSpecContext *Ctx);
		virtual void exitEnumTypeSpec(xdrParser::EnumTypeSpecContext *Ctx);
		
		virtual void enterEnumBody(xdrParser::EnumBodyContext *Ctx);
		virtual void exitEnumBody(xdrParser::EnumBodyContext *Ctx);
		
		virtual void enterStructTypeSpec(xdrParser::StructTypeSpecContext *Ctx);
		virtual void exitStructTypeSpec(xdrParser::StructTypeSpecContext *Ctx);
		
		virtual void enterStructBody(xdrParser::StructBodyContext *Ctx);
		virtual void exitStructBody(xdrParser::StructBodyContext *Ctx);
		
		virtual void enterUnionTypeSpec(xdrParser::UnionTypeSpecContext *Ctx);
		virtual void exitUnionTypeSpec(xdrParser::UnionTypeSpecContext *Ctx);
		
		virtual void enterUnionBody(xdrParser::UnionBodyContext *Ctx);
		virtual void exitUnionBody(xdrParser::UnionBodyContext *Ctx);
		
		virtual void enterCaseSpec(xdrParser::CaseSpecContext *Ctx);
		virtual void exitCaseSpec(xdrParser::CaseSpecContext *Ctx);
		
		virtual void enterConstantDef(xdrParser::ConstantDefContext *Ctx);
		virtual void exitConstantDef(xdrParser::ConstantDefContext *Ctx);
		
		virtual void enterTypeDef(xdrParser::TypeDefContext *Ctx);
		virtual void exitTypeDef(xdrParser::TypeDefContext *Ctx);
		
		virtual void enterDefinition(xdrParser::DefinitionContext *Ctx);
		virtual void exitDefinition(xdrParser::DefinitionContext *Ctx);
		
		virtual void enterXdrSpecification(xdrParser::XdrSpecificationContext *Ctx);
		virtual void exitXdrSpecification(xdrParser::XdrSpecificationContext *Ctx);
		
		virtual void enterNamespaceDef(xdrParser::NamespaceDefContext * Ctx);
		virtual void exitNamespaceDef(xdrParser::NamespaceDefContext * Ctx);
		
		virtual void enterSpecs(xdrParser::SpecsContext * Ctx);
		virtual void exitSpecs(xdrParser::SpecsContext * Ctx);
		
		virtual void enterEveryRule(ParserRuleContext * Ctx);
		virtual void exitEveryRule(ParserRuleContext * Ctx);

		virtual void enterMethod(xdrParser::MethodContext * Ctx);
		virtual void exitMethod(xdrParser::MethodContext * Ctx);

		virtual void enterProcFirstArg(xdrParser::ProcFirstArgContext * Ctx);
		virtual void exitProcFirstArg(xdrParser::ProcFirstArgContext * Ctx);

		virtual void enterComment(xdrParser::CommentContext * Ctx);
		virtual void exitComment(xdrParser::CommentContext * Ctx);

		virtual void enterPassThrough(xdrParser::PassThroughContext * Ctx);
		virtual void exitPassThrough(xdrParser::PassThroughContext * Ctx);

		virtual void enterProgram(xdrParser::ProgramContext * Ctx);
		virtual void exitProgram(xdrParser::ProgramContext * Ctx);

		virtual void enterVersion(xdrParser::VersionContext * Ctx);
		virtual void exitVersion(xdrParser::VersionContext * Ctx);

		virtual void enterVersionMethod(xdrParser::VersionMethodContext * Ctx);
		virtual void exitVersionMethod(xdrParser::VersionMethodContext * Ctx);

		virtual void enterDataType(xdrParser::DataTypeContext * Ctx);
		virtual void exitDataType(xdrParser::DataTypeContext * Ctx);

		virtual void enterDeclaration(xdrParser::DeclarationContext * Ctx);
		virtual void exitDeclaration(xdrParser::DeclarationContext * Ctx);

		virtual void visitTerminal(tree::TerminalNode * Node);
		virtual void visitErrorNode(tree::ErrorNode * Node);
		
	};
}
#endif // _RIVEREXPLORER_XDRGEN_XDRGEN_HPP_
