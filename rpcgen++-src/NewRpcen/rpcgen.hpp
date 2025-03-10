/**
 * Project: Phoenix
 * Time-stamp: <2025-03-10 13:49:21 doug>
 * 
 * @file rpcgen.hpp
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
#ifndef _RIVEREXPLORER_RPCGEN_RPCGEN_HPP_
#define _RIVEREXPLORER_RPCGEN_RPCGEN_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include "antlr4-runtime.h"
#include "xdrLexer.h"
#include "xdrParser.h"
#include "xdrListener.h"
#include "xdrBaseListener.h"

#include <rpc/rpc.h>

using namespace antlr4;
using namespace std;

namespace RiverExplorer::rpcgen
{
	/**
	 * The input file name.
	 */
	extern std::string InputFileName;
	
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
	 * Indentation level used when generating code.
	 */
	extern int IndentLevel;

#ifdef FOO	
	/**
	 * An implementation of an XDR routine
	 * for std::vector<uint8_t>.
	 *
	 * @param Xdr The initalized XDR object.
	 *
	 * @param opaque The address of the std::vector<uint8_t> object.
	 *
	 * @param[optional] MaxLength The maximum size of the vector.
	 * @return true on no errors.
	 */
	bool xdr_Opaque(XDR * Xdr,
									std::vector<uint8_t> * Opaque,
									uint32_t MaxLength = ~0);
	
	 
	/**
	 * An implementation of an XDR routine
	 * for std::vector<uint8_t>.
	 *
	 * @param Xdr The initalized XDR object.
	 *
	 * @param Str The address of the std::string.
	 *
	 * @param[optional] MaxLength The maximum size of the string.
	 * @return true on no errors.
	 */
	bool xdr_String(XDR * Xdr,
									std::string * Str,
									uint32_t MaxLength = ~0);
	 
	/**
	 * An implementation of an XDR routine
	 * for std::vector<uint8_t>.
	 *
	 * @param Xdr The initalized XDR object.
	 *
	 * @param Obj A pointer to the object.
	 *
	 * @param Proc the xdrproc_t routine that can
	 * handle type 'T'.
	 *
	 * @param[optional] MaxLength The maximum size of the string.
	 * @return true on no errors.
	 */
	template <class T>
	bool xdr_VectorOf_(XDR * Xdr,
										 std::vector<T> * Obj,
										 xdrproc_t Proc,
										 uint32_t MaxLength = ~0)
	{
		bool Results = false;
		
		if (Obj != nullptr && Proc != nullptr) {
			uint32_t Size = Obj->size();
			
			if (xdr_uint32_t(Xdr, &Size)) {

				typename std::vector<T>::iterator it;
				T Item;
				uint32_t Offset = 0;
				for (it = Obj->begin(); it != Obj->end(); it++) {
					Item = *it;
					Results = (*Proc)(Xdr, &Item);
					if (!Results) {
						break;
					}
					if (++Offset > MaxLength) {
						break;
					}
				}
			}
		}

		return(Results);
	}
#endif
	
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
	 * @class Item
	 * This exists as a base class for evertying, so
	 * we can put them into one vector and keep the
	 * original objcet order.
	 */
	class Item
	{
	public:

		virtual void PrintCppHeader(ofstream & Stream) = 0;
		virtual void PrintCppXDR(ofstream & Steam) = 0;
		virtual void PrintCppStubs(ofstream & Stream) = 0;
		virtual void PrintXSD(ofstream & Stream) = 0;
		virtual void PrintAbnf(ofstream & Stream) = 0;
		virtual void DeclareVariable(ofstream & Stream) = 0;
		
	};

	extern std::vector<Item*> OrderedItems;

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
		InTypedef,
		InUnion,
		InUnionCase,
		InProcedureDef,
		InVoid
	};

	extern State CurrentState;
	extern bool InArray;

	struct Constant
		: public Item
	{
	public:

		virtual ~Constant();
		
		std::string Name;
		int64_t Value;

		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};
	extern Constant * CurrentConstant;

	struct EnumValue
		: public Item
	{
	public:

		virtual ~EnumValue();
		
		std::string Name;
		int64_t Value;

		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};

	struct TypeDef
		: public Item
	{
	public:

		virtual ~TypeDef();
		
		std::string Name;
		std::string BaseType;
		std::string Constraint;
		bool IsPointer = false;
		bool IsFixedArray = false;
		bool IsVariableArray = false;
		std::string ArraySize;

		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};

	struct StructMember
	{
	public:

		virtual ~StructMember();
		
		std::string Type;
		std::string Name;
		bool IsPointer = false;
		bool IsFixedArray = false;
		bool IsVariableArray = false;
		std::string ArraySize;

		virtual void DeclareVariable(ofstream & Stream);
	};
	extern StructMember * CurrentStructMember;

	struct Struct
		: public Item
	{
	public:

		virtual ~Struct();
		
		std::string Name;
		std::vector<StructMember*> Members;

		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};
	extern Struct * CurrentStruct;

	struct UnionCase
	{
	public:

		virtual ~UnionCase();
		
		std::string CaseValue;
		std::string Type;
		std::string Name;
		bool IsDefault;
		bool IsPointer = false;
		bool IsFixedArray = false;
		bool IsVariableArray = false;
		std::string ArraySize;
		virtual void DeclareVariable(ofstream & Stream);
	};

	struct Union
		: public Item
	{
	public:

		Union();
		virtual ~Union();
		
		std::string Name;
		std::string SwitchType;
		std::string SwitchVariable;
		std::vector<UnionCase*> Cases;
		UnionCase * Default;
		
		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};
	extern Union * CurrentUnion;

	struct Procedure
		: public Item
	{
	public:

		virtual ~Procedure();
		
		std::string ReturnType;
		std::string Name;
		std::vector<StructMember*> Params;
		int64_t ProcNumber;

		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};

	struct Version
		: public Item
	{
	public:

		virtual ~Version();
		
		std::string Name;
		int64_t VersionNumber;
		std::vector<Procedure*> Procedures;

		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};

	struct Program
		: public Item
	{
	public:

		virtual ~Program();
		
		std::string Name;
		uint64_t ProgramNumber;
		std::vector<Version*> Versions;
		std::map<std::string, Constant*> Constants;
		std::map<std::string, std::vector<EnumValue*>*> Enums;
		std::map<std::string, TypeDef*> TypeDefs;
		std::map<std::string, Struct*> Structs;
		std::map<std::string, Union*> Unions;

		virtual void PrintCppHeader(ofstream & Stream);
		virtual void PrintCppXDR(ofstream & Stream);
		virtual void PrintCppStubs(ofstream & Stream);
		virtual void PrintXSD(ofstream & Stream);
		virtual void PrintAbnf(ofstream & Stream);
		virtual void DeclareVariable(ofstream & Stream);
	};
		
	class MyXdrListener
		: public xdrListener
	{

		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::DeclarationContext* Ctx);
		
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
										 xdrParser::TypedefDefContext* Ctx);

		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VarContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VarPtrContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VarFixedContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VarFixedPtrContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VarVariableContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VarVariablePtrContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::OpaqueFixedContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::OpaqueFixedPtrContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::OpaqueVariableContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::OpaqueVariablePtrContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::VoidContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::StringContext* Ctx);

		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::StringPtrContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 xdrParser::SpecsContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 tree::TerminalNode* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 ParserRuleContext* Ctx);
		

		//////////////////////////////////////////////////////////////////////
		virtual void enterDeclaration(xdrParser::DeclarationContext *Ctx);
		virtual void exitDeclaration(xdrParser::DeclarationContext *Ctx);
		
		virtual void enterDeclaration(xdrParser::ConstantDefContext *Ctx);
		virtual void exitDeclaration(xdrParser::ConstantDefContext *Ctx);
		
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
		
		virtual void enterTypedefDef(xdrParser::TypedefDefContext * Ctx);
		virtual void exitTypedefDef(xdrParser::TypedefDefContext * Ctx);
		
		virtual void enterSpecs(xdrParser::SpecsContext * Ctx);
		virtual void exitSpecs(xdrParser::SpecsContext * Ctx);
		
		virtual void enterEveryRule(ParserRuleContext * Ctx);
		virtual void exitEveryRule(ParserRuleContext * Ctx);

		virtual void enterVar(xdrParser::VarContext * Ctx);
		virtual void exitVar(xdrParser::VarContext * Ctx);

		virtual void enterVarPtr(xdrParser::VarPtrContext * Ctx);
		virtual void exitVarPtr(xdrParser::VarPtrContext * Ctx);

		virtual void enterVarFixed(xdrParser::VarFixedContext * Ctx);
		virtual void exitVarFixed(xdrParser::VarFixedContext * Ctx);

		virtual void enterVarFixedPtr(xdrParser::VarFixedPtrContext * Ctx);
		virtual void exitVarFixedPtr(xdrParser::VarFixedPtrContext * Ctx);

		virtual void enterVarVariable(xdrParser::VarVariableContext * Ctx);
		virtual void exitVarVariable(xdrParser::VarVariableContext * Ctx);

		virtual void enterVarVariablePtr(xdrParser::VarVariablePtrContext * Ctx);
		virtual void exitVarVariablePtr(xdrParser::VarVariablePtrContext * Ctx);

		virtual void enterOpaqueFixed(xdrParser::OpaqueFixedContext * Ctx);
		virtual void exitOpaqueFixed(xdrParser::OpaqueFixedContext * Ctx);

		virtual void enterOpaqueFixedPtr(xdrParser::OpaqueFixedPtrContext * Ctx);
		virtual void exitOpaqueFixedPtr(xdrParser::OpaqueFixedPtrContext * Ctx);

		virtual void enterOpaqueVariable(xdrParser::OpaqueVariableContext * Ctx);
		virtual void exitOpaqueVariable(xdrParser::OpaqueVariableContext * Ctx);

		virtual void enterOpaqueVariablePtr(xdrParser::OpaqueVariablePtrContext * Ctx);
		virtual void exitOpaqueVariablePtr(xdrParser::OpaqueVariablePtrContext * Ctx);

		virtual void enterVoid(xdrParser::VoidContext * Ctx);
		virtual void exitVoid(xdrParser::VoidContext * Ctx);

		virtual void enterString(xdrParser::StringContext * Ctx);
		virtual void exitString(xdrParser::StringContext * Ctx);

		virtual void enterStringPtr(xdrParser::StringPtrContext * Ctx);
		virtual void exitStringPtr(xdrParser::StringPtrContext * Ctx);

		virtual void visitTerminal(tree::TerminalNode * Node);
		virtual void visitErrorNode(tree::ErrorNode * Node);
		
	};
}
#endif // _RIVEREXPLORER_RPCGEN_RPCGEN_HPP_
