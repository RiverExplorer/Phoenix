%skeleton "lalr1.cc"
%language "C++"
%require "3.0"
%defines
%define api.namespace {RiverExplorer::Phoenix::Protocol}
%define api.parser.class {Parser}

%code requires {
#include <fstream>
#include <string>

#include "Globals.hpp"
#include "Protocol.hpp"
#include "GenerateCPP.hpp"

using namespace RiverExplorer::Phoenix::Protocol;

	namespace RiverExplorer::Phoenix::Protocol
	{
		class Driver;
		class Scanner;
	}

extern bool VerboseDebug;
extern int LineNumber;
extern void PrintLine();


extern RiverExplorer::Phoenix::Protocol::Control CurrentProtocol;
extern RiverExplorer::Phoenix::Protocol::Scope * CurrentScope;
extern RiverExplorer::Phoenix::Protocol::Symbol * CurrentSymbol;
extern RiverExplorer::Phoenix::Protocol::Symbol::Visibility_e CurrentVisibility;

extern RiverExplorer::Phoenix::Protocol::Range * CurrentRange;

extern RiverExplorer::Phoenix::Protocol::DefaultValueT<uint64_t> * CurrentUintDefaultValue;
extern RiverExplorer::Phoenix::Protocol::DefaultValueT<int64_t> * CurrentSintDefaultValue;
extern RiverExplorer::Phoenix::Protocol::DefaultValueT<long double> * CurrentFloatDefaultValue;
extern RiverExplorer::Phoenix::Protocol::DefaultValueT<std::string> * CurrentStringDefaultValue;
extern RiverExplorer::Phoenix::Protocol::DefaultValueT<bool> * CurrentBoolDefaultValue;

extern std::vector<SymbolValue*> *	CurrentEnumDefs;

extern RiverExplorer::Phoenix::Protocol::Symbol * CurrentReturnType;

extern std::vector<Symbol*>	*	CurrentParameters;
extern std::string						CurrentNamespace;

/*  PACKING */
extern bool PackingOrderSet;
extern bool PackingSizeSet;
extern std::string BuildingNS;

/* OUTPUT DIRS */
extern GenerateCPP * CPP;

#define YYDEBUG 1

#ifndef YY_NULLPTR
#if defined __cplusplus && 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#endif
}

%parse-param { Scanner & _Scanner }
%parse-param { Driver & _Driver }

%define parse.error verbose
%define parse.trace

%code {
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "Driver.hpp"

#undef yylex
#define yylex _Scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token									END	0	"end of file"
%token <uint64_t>		 		DIGITS		/* Positive digits */
%token <int64_t>		 		NDIGITS		/* Negative digits */
%token <long double> 		FDIGITS		/* Floating point digits */

%token <std::string> 		IDSTRING
%token 									STRING
%type  <Symbol*>				string
%token <std::string>		DQSTRING		/* Double quotes string "abc" */
%token <std::string>		SQSTRING 		/* Single quotes string 'abc' */
%type <std::string>			anystring		/* DQSTRING or SQSTRING */
%type <Symbol*>					conststring

%token <std::string>		COMMENT		/* // */

%token 									LCURLY		/* { */
%token 									RCURLY		/* } */

%token 									LPAREN		/* ( */
%token 									RPAREN		/* ) */

%token 									LSQUARE		/* [ */
%token 									RSQUARE		/* ] */

%token 									COMMA			/* , */
%token 									SEMI			/* ; */
%token 									COLON			/* : */

%token 															DEFAULT

%type  <DefaultValueT<uint64_t>*>			uint_default		/* uint id Default(1) ...; */
%type  <DefaultValueT<int64_t>*>			sint_default		/* sint id Default(-1) ...; */
%type  <DefaultValueT<long double>*>	float_default		/* float id Default(1.2) ...; */
%type  <DefaultValueT<std::string>*>	string_default	/* string id Default('xxx') ...; */

%token 												FUNCTION
%token										 		RANGE

%token												URANGE
%type <RangeT<uint64_t>*>			consturange		/* const URange id(3,10); */

%token												SRANGE
%type <RangeT<int64_t>*>			constsrange		/* const SRange id(-2,15); */

%token												FRANGE
%type <RangeT<long double>*>	constfrange		/* const FRange id(-3.3, 7.8); */

%token						 						ENUM
%type <Symbol*>								enumtype
%type <SymbolValue*>					enumdef
%type <std::vector<SymbolValue*>*>	enumdefs

%token 									LESS_THAN							/* < */
%token 									LESS_THAN_OR_EQUAL		/* <= */
%token 									EQUAL									/* = */
%token 									GREATER_THAN_OR_EQUAL	/* >= */
%token 									GREATER_THAN					/* > */

%type <int64_t>					p_or_n								/* Positive or negative integer */
%type <long double>			p_or_n_or_f						/* Positive, negative integer, or float */

%type <Range::RangeCmp_e> range_comparison
%type <Range*>					 uint_range
%type <Range*>					 sint_range
%type <Range*>					 float_range

%token 									PROTOCOL
%type <Symbol*>					protocol
%type <Symbol*>					protocolid

%token 									VERSION
%type <Symbol*>					version
%type <Symbol*>					versionid

%token									BOOL
%type <Symbol*>					bool
%type <Symbol*>					constbool
%token									TRUE
%token 									FALSE
%type <bool>						t_or_f
%type <bool>						bool_default

%token									CLASS
%type <Symbol*>					class

%token									UINT
%type <Symbol*>					constuint
%type <Symbol*>					uint

%token									SINT
%type <Symbol*>					constsint
%type <Symbol*>					sint

%token									FLOAT
%type <Symbol*>					constfloat
%type <Symbol*>					float

%token									VOID
%type <Symbol*>					void

%token									OPAQUE
%type <Symbol*>					opaque

%token									CONST
%type <bool>						opt_const;

%token									MUTABLE

%token									PACKNBO
%token									PACKLR
%token									PACKRL
%token									BITSTREAM

%token		<uint64_t>		PACKSIZE
%token		<uint64_t>		DEFAULTPACKSIZE

%token									INCLUDE			/* Include an additional file at this point, then resume. */
%token									FILENAME		/* The file name in use at the time it was parsed. */

%token									SERIALIZE
%token									NO_SERIALIZE

%token									GEN_CBOR
%token									GEN_CBOR_CLIENT
%token									GEN_CBOR_SERVER
%token									NO_GEN_CBOR
%token									NO_GEN_CBOR_CLIENT
%token									NO_GEN_CBOR_SERVER

%token									GEN_JSON
%token									GEN_JSON_CLIENT
%token									GEN_JSON_SERVER
%token									NO_GEN_JSON
%token									NO_GEN_JSON_CLIENT
%token									NO_GEN_JSON_SERVER

%token									GEN_NATIVE
%token									GEN_NATIVE_CLIENT
%token									GEN_NATIVE_SERVER
%token									NO_GEN_NATIVE
%token									NO_GEN_NATIVE_CLIENT
%token									NO_GEN_NATIVE_SERVER

%token									GEN_REST
%token									GEN_REST_CLIENT
%token									GEN_REST_SERVER
%token									NO_GEN_REST
%token									NO_GEN_REST_CLIENT
%token									NO_GEN_REST_SERVER

%token									GEN_XDR
%token									GEN_XDR_CLIENT
%token									GEN_XDR_SERVER
%token									NO_GEN_XDR
%token									NO_GEN_XDR_CLIENT
%token									NO_GEN_XDR_SERVER

%token									GEN_XML
%token									GEN_XML_CLIENT
%token									GEN_XML_SERVER
%token									NO_GEN_XML
%token									NO_GEN_XML_CLIENT
%token									NO_GEN_XML_SERVER
%token									XML_NAMSPACE
%token									XML_ELEMENT_NAME
%token									XML_ATTRIBUTE_NAME

%token									NAMESPACE
%token									NAMESPACE_ID		/* The current namespace as a string. */

%token									GEN_CPP_CODE
%token									NO_GEN_CPP_CODE

%token									GEN_CSHARP_CODE
%token									NO_GEN_CSHARP_CODE

%token									VISIBILITY_PUBLIC
%token									VISIBILITY_PROTECTED
%token									VISIBILITY_INTERNAL
%token									VISIBILITY_PRIVATE

%token									DOCUMENT
%token									NO_DOCUMENT

%type <std::string>			namespacestring

%type <Symbol*>								boolsingle
%type <Symbol*>								boolfixedarray
%type <Symbol*>								boolvariablearray
%type <Symbol*>								booltype
%type <Symbol*>								classtype
%type <Symbol*>								comment
%type <Symbol*>								floattype
%type <Symbol*>								floatsingle
%type <Symbol*>								floatfixedarray
%type <Symbol*>								floatvariablearray
%type <Symbol*>								mreturn
%type <Symbol*>								methodsig
%type <Symbol*>								opaquetype
%type <Symbol*>								opaquefixedarray
%type <Symbol*>								opaquevariablearray
%type <std::vector<Symbol*>*>	parameters
%type <Symbol*>								paramtype
%type <Symbol*>								simpletype
%type <Symbol*>								sinttype
%type <Symbol*>								sintsingle
%type <Symbol*>								sintfixedarray
%type <Symbol*>								sintvariablearray
%type <Symbol*>								stringtype
%type <Symbol*>								stringsingle
%type <Symbol*>								stringfixedarray
%type <Symbol*>								stringvariablearray
%type <Symbol*>								uintsingle
%type <Symbol*>								uintfixedarray
%type <Symbol*>								uintvariablearray
%type <Symbol*>								uinttype

%locations /* THEY DO NOT SEEM TO WORK */

%%

protocol : init comments protocoldirectives comments protocolid LCURLY comments versions RCURLY SEMI
				 {
					$$ = $5;
				 }
				 ;

protocolid : PROTOCOL IDSTRING
					 {
					  Global::Log << "Protocol ID: " << $2;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::protocol_t, $2);

						if (CurrentNamespace.length() > 0) {
							 CurrentNamespace = CurrentNamespace + ":" + $2;
						} else {
							 CurrentNamespace =  $2;
						}
						CurrentSymbol->Namespace = $2;
						
						SymbolValueT<std::string> * NewValue = new SymbolValueT<std::string>($2);
						CurrentSymbol->Value = NewValue;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
						$$ = CurrentSymbol;
					 }
					 ;

init : %empty {
		 CurrentScope = CurrentProtocol.GlobalScope;
		 Global::CurrentScope.push_front(CurrentScope);
}

comments : %empty
				 | comments comment
				 ;

comment : COMMENT
				{
					Global::Log << "COMMENT: " << $1;
					PrintLine();
					CurrentSymbol = new Symbol(Symbol::comment_t, "");
					CurrentSymbol->Namespace = CurrentScope->Namespace;
					SymbolValueT<std::string> * NewValue = new SymbolValueT<std::string>($1);
					CurrentSymbol->Value = NewValue;
					CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					$$ = CurrentSymbol;
				}
;

gen_cbor : GEN_CBOR {
					  Global::Log <<"Generating CBOR Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = true;
						Global::GScope.GenerateCBOR_Server = true;
						}
						;
						
no_gen_cbor :  NO_GEN_CBOR {
					  Global::Log <<"Not generating CBOR Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = false;
						Global::GScope.GenerateCBOR_Server = false;
					 }
					 ;

gen_cbor_client : GEN_CBOR_CLIENT {
					  Global::Log <<"Generating CBOR Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = true;
					 }
					 ;

no_gen_cbor_client : NO_GEN_CBOR_CLIENT {
					  Global::Log <<"Not generating CBOR Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = false;
					 }
					 ;

gen_cbor_server : GEN_CBOR_SERVER {
					  Global::Log <<"Generating CBOR Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Server = true;
					 }
					 ;

no_gen_cbor_server : NO_GEN_CBOR_SERVER {
					  Global::Log <<"Not generating CBOR Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Server = false;
					 }
					 ;

gen_json : GEN_JSON {
						Global::Log << "Generating JSON Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = true;
						Global::GScope.GenerateJSON_Server = true;
					 }
					 ;

no_gen_json : NO_GEN_JSON {
						Global::Log << "Not generating JSON Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = false;
						Global::GScope.GenerateJSON_Server = false;
					 }
					 ;

gen_json_client :GEN_JSON_CLIENT {
						Global::Log << "Generating JSON Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = true;
					 }
					 ;

no_gen_json_client :NO_GEN_JSON_CLIENT {
						Global::Log << "Not generating JSON Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = false;
					 }
					 ;

gen_json_server :GEN_JSON_SERVER {
						Global::Log << "Generating JSON Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Server = true;
					 }
					 ;

no_gen_json_server :NO_GEN_JSON_SERVER {
						Global::Log << "Not generating JSON Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Server = false;
					 }
					 ;

gen_native : GEN_NATIVE {
					 	Global::Log << "Generating NATIVE Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = true;
						Global::GScope.GenerateNATIVE_Server = true;
					 }
					 ;

no_gen_native : NO_GEN_NATIVE {
					 	Global::Log << "Not generating NATIVE Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = false;
						Global::GScope.GenerateNATIVE_Server = false;
					 }
					 ;

gen_native_client :GEN_NATIVE_CLIENT {
					 	Global::Log << "Generating NATIVE Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = true;
					 }
					 ;

no_gen_native_client : NO_GEN_NATIVE_CLIENT {
					 	Global::Log << "Not generating NATIVE Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = false;
					 }
					 ;

gen_native_server : GEN_NATIVE_SERVER {
					 	Global::Log << "Generating NATIVE Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateNATIVE_Server = true;
					 }
					 ;

no_gen_native_server : NO_GEN_NATIVE_SERVER {
						 Global::Log << "Not generating NATIVE Server protocol and code.";
						 PrintLine();
						 Global::GScope.GenerateNATIVE_Server = false;
					 }
					 ;

gen_rest : GEN_REST {
						Global::Log << "Global::Generating REST Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = true;
						Global::GScope.GenerateREST_Server = true;
					 }
					 ;

no_gen_rest : NO_GEN_REST {
						Global::Log << "Global::Not generating REST Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = false;
						Global::GScope.GenerateREST_Server = false;
					 }
					 ;

gen_rest_client : GEN_REST_CLIENT {
						Global::Log << "Generating REST Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = true;
					 }
					 ;

no_gen_rest_client : NO_GEN_REST_CLIENT {
						Global::Log << "Not generating REST Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = false;
					 }
					 ;

gen_rest_server : GEN_REST_SERVER {
						Global::Log << "Global::Generating REST Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Server = true;
					 }
					 ;

no_gen_rest_server : NO_GEN_REST_SERVER {
						Global::Log << "Global::Not generating REST Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Server = false;
					 }
					 ;

gen_xdr : GEN_XDR {
						Global::Log << "Global::Generating XDR Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = true;
						Global::GScope.GenerateXDR_Server = true;
					 }
					 ;

no_gen_xdr : NO_GEN_XDR {
						Global::Log << "Global::Not generating XDR Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = false;
						Global::GScope.GenerateXDR_Server = false;
					 }
					 ;

gen_xdr_client : GEN_XDR_CLIENT {
						Global::Log << "Generating XDR Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = true;
					 }
					 ;

no_gen_xdr_client : NO_GEN_XDR_CLIENT {
						Global::Log << "Not generating XDR Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = false;
					 }
					 ;

gen_xdr_server : GEN_XDR_SERVER {
						Global::Log << "Generating XDR Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Server = true;
					 }
					 ;

no_gen_xdr_server : NO_GEN_XDR_SERVER {
						Global::Log << "Not generating XDR Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Server = false;
					 }
					 ;

gen_xml : GEN_XML {
						Global::Log << "Generating XML Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = true;
						Global::GScope.GenerateXML_Server = true;
					 }
					 ;

no_gen_xml : NO_GEN_XML {
						Global::Log << "Not generating XML Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = false;
						Global::GScope.GenerateXML_Server = false;
					 }
					 ;

gen_xml_client : GEN_XML_CLIENT {
						Global::Log << "Generating XML Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = true;
					 }
					 ;

no_gen_xml_client : NO_GEN_XML_CLIENT {
						Global::Log << "Not generating XML Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = false;
					 }
					 ;

gen_xml_server : GEN_XML_SERVER {
						Global::Log << "Generating XML Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Server = true;
					 }
					 ;

no_gen_xml_server : NO_GEN_XML_SERVER {
						Global::Log << "Not generating XML Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Server = false;
					 }
					 ;
	
packlr :  PACKLR {
						if (!PackingOrderSet) {
							 Global::GScope.Packing.PackingOrder = Pack::PackLR;
							 PackingOrderSet = true;
							 Global::Log << "Defalt bit Packing Left to Right.";
						} else {
							Global::Log << "Error: Packing order already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
					 ;

packrl : PACKRL {
						if (!PackingOrderSet) {
							 Global::GScope.Packing.PackingOrder = Pack::PackRL;
							 PackingOrderSet = true;
							 Global::Log << "Defalt bit Packing Right to Left.";
						} else {
							Global::Log << "Error: Packing order already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
					 ;

packnbo : PACKNBO {
						if (!PackingOrderSet) {
							 Global::GScope.Packing.PackingOrder = Pack::PackNBO;
							 PackingOrderSet = true;
							 Global::Log << "Defalt bit Packing network byte order.";
						} else {
							Global::Log << "Error: Packing order already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
					 ;

bitstream : BITSTREAM {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.BitStream = true;
							 PackingSizeSet = true;
							 Global::Log << "Default Packing set to none - Bit Stream";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
					 ;
					 

defaultpacksize : DEFAULTPACKSIZE COLON DIGITS {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.PackSize = (uint8_t)$3;
							 PackingSizeSet = true;
							 Global::Log << "Deafult Packing set to " << $3 << " bits";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
					 ;

packsize : PACKSIZE COLON DIGITS {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.PackSize = (uint8_t)$3;
							 PackingSizeSet = true;
							 Global::Log << "Deafult Packing set to " << $3 << " bits";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
					 ;

gen_cpp_code : GEN_CPP_CODE {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCPP = true;
						 Global::Log << "C++ Code generation turned on.";
						PrintLine();
					 }
					 ;

no_gen_cpp_code : NO_GEN_CPP_CODE {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCPP = false;
						 Global::Log << "C++ Code generation turned off.";
						PrintLine();
					 }
					 ;

gen_csharp_code : GEN_CSHARP_CODE {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCSharp = true;
						 Global::Log << "C# Code generation turned on.";
						PrintLine();
					 }
					 ;

no_gen_csharp_code : NO_GEN_CSHARP_CODE {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCSharp = false;
						 Global::Log << "C# Code generation turned off.";
						PrintLine();
					 }
					 ;


document : DOCUMENT {
						 Global::Log << "Start Document";
						PrintLine();
					 }
					 ;

no_document : NO_DOCUMENT {
						 Global::Log << "No Document";
						PrintLine();
					 }
					 ;

dnamespace :  NAMESPACE EQUAL namespacestring {
					 	 	Global::Log << "Namespace: " << $3;
	 						PrintLine();
							CurrentNamespace = $3;
						 	CurrentScope->Namespace = $3;
							Global::GScope.Namespace = $3;
					 }
					 ;

protocoldirectives : LSQUARE pdirectives RSQUARE
									 | protocoldirectives LSQUARE pdirectives RSQUARE
									 ;

/* One or more directives needed */
pdirectives : directive
						| pdirectives COMMA directive
						;

directive : gen_cbor
					| no_gen_cbor
					| gen_cbor_client
					| no_gen_cbor_client
					| gen_cbor_server
					| no_gen_cbor_server
					| gen_json
					| no_gen_json
					| gen_json_client
					| no_gen_json_client
					| gen_json_server
					| no_gen_json_server
					| gen_native
					| no_gen_native
					| gen_native_client
					| no_gen_native_client
					| gen_native_server
					| no_gen_native_server
					| gen_rest
					| no_gen_rest
					| gen_rest_client
					| no_gen_rest_client
					| gen_rest_server
					| no_gen_rest_server
					| gen_xdr
					| no_gen_xdr
					| gen_xdr_client
					| no_gen_xdr_client
					| gen_xdr_server
					| no_gen_xdr_server
					| gen_xml
					| no_gen_xml
					| gen_xml_client
					| no_gen_xml_client
					| gen_xml_server
					| no_gen_xml_server
					| packlr
          | packrl
					| packnbo
					| bitstream
					| defaultpacksize
					| packsize
					| gen_cpp_code
					| no_gen_cpp_code
					| gen_csharp_code
					| no_gen_csharp_code
					| document
					| no_document
					| dnamespace
					;

begin_public : VISIBILITY_PUBLIC
						 {
								CurrentVisibility = Symbol::Public_t;
						 }
						 ;
						 
begin_protected : VISIBILITY_PROTECTED
						 {
								CurrentVisibility = Symbol::Protected_t;
						 }
						 ;
						 
begin_internal : VISIBILITY_INTERNAL
						 {
								CurrentVisibility = Symbol::Internal_t;
						 }
						 ;
						 
begin_private : VISIBILITY_PRIVATE
						 {
								CurrentVisibility = Symbol::Private_t;
						 }
						 ;
						 
namespacestring : IDSTRING {
									BuildingNS = $1;
									$$ = $1;
              	}
								|	namespacestring COLON IDSTRING {
									BuildingNS = $1 + ":" + $3;
									$$ = BuildingNS;
								}
								;
								
versions : version
         | versions version
				 ;
				 
version : versionid LCURLY contents RCURLY SEMI
				{
					$$ = $1;
				}
				;

versionid : VERSION IDSTRING
					 {
					  Global::Log << "VERSION ID: " << $2;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::version_t, $2);

						if (CurrentNamespace.length() > 0) {
							 CurrentNamespace = CurrentNamespace + ":" + $2;
						} else {
							 CurrentNamespace =  $2;
						}

						SymbolValueT<std::string> * NewValue = new SymbolValueT<std::string>($2);
						CurrentSymbol->Value = NewValue;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
						$$ = CurrentSymbol;
					 }
					 ;

contents : content
				 | contents content;
				 ;

content : simpletype SEMI
				{
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
				}
				| classdef SEMI
				{
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
				}
				| classtype SEMI
				{
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
				}
				| methodsig SEMI
				{
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
				}
				| comments
				| consttype SEMI
				{
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
				}

				| enumtype SEMI
				| begin_public
				| begin_protected
				| begin_internal
				| begin_private
				;

consttype : constuint
					| constsint
					| constfloat
					| conststring
					| consturange
					| constsrange
					| constfrange
					| constbool
					;

t_or_f : TRUE
			 {
				$$ = true;
			 }
			 
			 | FALSE
			 {
				$$ = false;
			 }
			 ;


enumdef : IDSTRING EQUAL DIGITS		/* Red = 3 */
				 {
					EnumDefT<uint64_t> * NewDef = new EnumDefT<uint64_t>($1, $3);
					$$ = NewDef;
				 }
				 
				 | IDSTRING EQUAL NDIGITS		/* Rotation = -4 */
				 {
					EnumDefT<int64_t> * NewDef = new EnumDefT<int64_t>($1, $3);
					$$ = NewDef;
				 }
				 | IDSTRING EQUAL FDIGITS		/* Offset = 8.2 */
				 {
					EnumDefT<long double> * NewDef = new EnumDefT<long double>($1, $3);
					$$ = NewDef;
				 }
				 | IDSTRING EQUAL anystring	/* Name = 'A string' */
				 {
					EnumDefT<std::string> * NewDef = new EnumDefT<std::string>($1, $3);
					$$ = NewDef;
				 }
				 | IDSTRING EQUAL IDSTRING	/* Matches = ExistingConstVariable */
				 {
				  $$ = nullptr;
					Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $3);
					if (Found != nullptr) {
					 if (Found->IsConstant) {
					  $$ = Found->Value;
					 } else {
					 	 Global::Log << "enumdef: Found: " << $3 << ", it must be a const.\n";
						 YYERROR;
					 }
					} else {
					 	 Global::Log << "enumdef: Could not find const " << $3 << ".\n";
						 YYERROR;
					}
				 }
				 ;

enumdefs : enumdef
				 {
					if (CurrentEnumDefs == nullptr) {
						 CurrentEnumDefs = new std::vector<SymbolValue*>();
					}
					CurrentEnumDefs->push_back($1);
					$$ = CurrentEnumDefs;
				 }
				 | enumdefs COMMA enumdef
				 {
					if (CurrentEnumDefs == nullptr) {
						 CurrentEnumDefs = new std::vector<SymbolValue*>();
					}
					CurrentEnumDefs->push_back($3);
					$$ = CurrentEnumDefs;
				 }
				 ;
				 
enumtype : ENUM IDSTRING LCURLY enumdefs RCURLY
				 {
					Enum * NewSymbol= new Enum($2);
					NewSymbol->Defs = $4;
					CurrentEnumDefs = nullptr; /* was $4 */
					CurrentSymbol = NewSymbol;
					$$ = CurrentSymbol;
				 }
				 ;
				 
constbool : CONST BOOL IDSTRING EQUAL t_or_f
					{
						Global::Log << "CONST BOOL: " << $3;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::bool_t, $3);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<bool> * NewValue = new SymbolValueT<bool>($5);
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						$$ = CurrentSymbol;
					}

constuint : CONST UINT IDSTRING EQUAL DIGITS
           {
						Global::Log << "CONST UNSIGNED INTEGER: " << $3;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $3);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<uint64_t> * NewValue = new SymbolValueT<uint64_t>($5);
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						$$ = CurrentSymbol;
					 }

					 | CONST UINT COLON DIGITS IDSTRING EQUAL DIGITS
           {
						Global::Log << "CONST UNSIGNED " << $4 << "-BIT INTEGER: " << $5;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $4, $5);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<uint64_t> * NewValue = new SymbolValueT<uint64_t>($7);
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						$$ = CurrentSymbol;
					 }
					;

constsint : CONST SINT IDSTRING EQUAL p_or_n
           {
						Global::Log << "CONST SIGNED INTEGER: " << $3;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $3);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<int64_t> * NewValue = new SymbolValueT<int64_t>((int64_t)$5);
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						$$ = CurrentSymbol;
					 }
					 
					| CONST SINT COLON DIGITS IDSTRING EQUAL p_or_n
           {
						Global::Log << "CONST SIGNED " << $4 << "-BIT INTEGER: " << $5;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $4, $5);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<int64_t> * NewValue = new SymbolValueT<int64_t>((int64_t)$7);
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						$$ = CurrentSymbol;
					 }
					;

constfloat : CONST FLOAT IDSTRING EQUAL p_or_n_or_f
           {
						Global::Log << "CONST FLOAT: " << $3;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $3);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<long double> * NewValue = new SymbolValueT<long double>($5);
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						$$ = CurrentSymbol;
					 }

					 | CONST FLOAT COLON DIGITS IDSTRING EQUAL p_or_n_or_f
					 {
						Global::Log << "CONST " << $4 << "-BIT FLOAT: " << $5;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $4, $5);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<long double> * NewValue = new SymbolValueT<long double>($7);
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						$$ = CurrentSymbol;
					 }
					 ;

anystring : DQSTRING
					| SQSTRING
					;
					
conststring : CONST STRING IDSTRING EQUAL anystring
						{
							CurrentSymbol = new Symbol(Symbol::string_t, $3);
							CurrentSymbol->Namespace = CurrentScope->Namespace;
							SymbolValueT<std::string> * NewValue = new SymbolValueT<std::string>($5);
							CurrentSymbol->Value = NewValue;
							CurrentSymbol->IsConstant = true;
							$$ = CurrentSymbol;
 					  }
						;

simpletype : uinttype
					 {
						$$ = $1;
 					 }
					 | sinttype
					 {
						$$ = $1;
 					 }
					 | floattype
					 {
						$$ = $1;
 					 }
					 | stringtype
					 {
						$$ = $1;
 					 }
					 | opaquetype
					 {
						$$ = $1;
 					 }
					 | booltype
					 {
						$$ = $1;
 					 }
					 ;

classdef : CLASS IDSTRING LCURLY classcontents RCURLY;

classcontents : classcontent
							| classcontents classcontent
							;
							
classcontent : simpletype
						 | classtype
						 | methodsig
						 ;

classtype : IDSTRING IDSTRING
					{
						$$ = nullptr;
					}
					;

range_comparison : LESS_THAN								/* '<' Less than */
								 {
									$$ = Range::LessThan_t;
								 }
								 
						 		 | LESS_THAN_OR_EQUAL				/* '<=' */
								 {
									$$ = Range::LessThanOrEqualTo_t;
								 }
								 
			 					 | GREATER_THAN_OR_EQUAL		/* '>=' */
								 {
									$$ = Range::GreaterThanOrEqualTo_t;
								 }
								 
			 					 | GREATER_THAN							/* '>' Greater than */
								 {
									$$ = Range::GreaterThan_t;
								 }
			 					 ;

booltype : boolsingle
				 | boolfixedarray
				 | boolvariablearray
				 ;

bool_default : DEFAULT LPAREN t_or_f RPAREN
						 {
							CurrentBoolDefaultValue = new DefaultValueT<bool>($3);
							$$ = CurrentUintDefaultValue;
						 }
						 ;

opt_bool : %empty
				 | bool_default
				 ;

boolsingle : BOOL IDSTRING opt_bool
					 {
						Global::Log << "BOOL: " << $2;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::bool_t, $2);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->Default = CurrentBoolDefaultValue;
						CurrentBoolDefaultValue = nullptr;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;
boolfixedarray : BOOL IDSTRING LSQUARE DIGITS RSQUARE opt_bool
           {
						Global::Log << "Fixed Array of " << $4 << " BOOL: "	 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::bool_t, $2, Symbol::FixedArray_t, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->Default = CurrentBoolDefaultValue;
						CurrentBoolDefaultValue = nullptr;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;					 
					 
boolvariablearray : BOOL IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_bool
					 {
						Global::Log << "Variable Array of " << $4 << " to " << $6 << " BOOL: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::bool_t, $2, Symbol::VariableArray_t, $4, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->Default = CurrentBoolDefaultValue;
						CurrentUintDefaultValue = nullptr;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | BOOL IDSTRING LESS_THAN GREATER_THAN opt_bool
					 {
						Global::Log << "Variable Array of zero or more BOOL: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::bool_t, $2, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->Default = CurrentBoolDefaultValue;
						CurrentBoolDefaultValue = nullptr;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;					 
					 
uinttype : uintsingle
				 | uintfixedarray
				 | uintvariablearray
				 ;

uint_default : DEFAULT LPAREN DIGITS RPAREN 		/* DEFAULT(3) */
						 {
							CurrentUintDefaultValue = new DefaultValueT<uint64_t>($3);
							$$ = CurrentUintDefaultValue;
						 }

						 | DEFAULT LPAREN IDSTRING	RPAREN /* [DEFAULT=UpperLimitVar] */
						 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $3);
							if (Found != nullptr) {
								 if (Found->IsConstant) {
								  SymbolValueT<uint64_t> * VarValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
								  CurrentUintDefaultValue = new DefaultValueT<uint64_t>(VarValue->Value);
									$$ = CurrentUintDefaultValue;

								 } else {
								  Global::Log << "ERROR: Symbol " << $3 << " was found, it must be a constant to be used in a Default\n";
									YYERROR;
								 }
							} else {
								  Global::Log << "ERROR: Symbol " << $3 << " was not found, it must be alredy defined and be a constant to be used in a Default\n";
									YYERROR;
							}
						 }
						 ;

sint_default : DEFAULT LPAREN DIGITS RPAREN 		/* DEFAULT(3) */
						 {
							CurrentSintDefaultValue = new DefaultValueT<int64_t>((int64_t)$3);
							$$ = CurrentSintDefaultValue;
						 }

						 | DEFAULT LPAREN NDIGITS RPAREN 		/* DEFAULT(-3) */
						 {
							CurrentSintDefaultValue = new DefaultValueT<int64_t>($3);
							$$ = CurrentSintDefaultValue;
						 }

						 | DEFAULT LPAREN IDSTRING RPAREN /* [DEFAULT=UpperLimitVar] */
						 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $3);
							if (Found != nullptr) {
								 if (Found->IsConstant) {
								 		/* With signned value, the default can be signed, or unsigned. */
								 		SymbolValueT<int64_t> * SVarValue = dynamic_cast<SymbolValueT<int64_t>*>(Found->Value);
										SymbolValueT<uint64_t> * UVarValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
										
										if (SVarValue != nullptr) {
										 CurrentSintDefaultValue = new DefaultValueT<int64_t>(SVarValue->Value);
										 $$ = CurrentSintDefaultValue;
										 
										} else if (UVarValue != nullptr) {
											CurrentSintDefaultValue = new DefaultValueT<int64_t>((int64_t)UVarValue->Value);
											$$ = CurrentSintDefaultValue;
											
									  } else {
								  		Global::Log << "ERROR: Symbol " << $3 << " was found, it did not have a uint, or sint value.\n";
											YYERROR;
								  	}
									} else {
								  		Global::Log << "ERROR: Symbol " << $3 << " was found, it must be a constant to be used in a Default.\n";
											YYERROR;
									}
							} else {
								  Global::Log << "ERROR: Symbol " << $3 << " was not found, it must be alredy defined and be a constant to be used in a Default.\n";
									YYERROR;
							}
						 }
						 ;

consturange : CONST URANGE IDSTRING LPAREN range_comparison DIGITS COMMA range_comparison DIGITS RPAREN
			 {
				Symbol * S = new Symbol(Symbol::urange_t, $3);
				S->Namespace = CurrentScope->Namespace;
				S->IsConstant = true;
				RangeT<uint64_t> * NewRange = new RangeT<uint64_t>($5, $6, $8, $9);
				S->ValidRange = NewRange;
				$$ = NewRange;
			 }
			 ;
			 
constsrange : CONST SRANGE IDSTRING LPAREN range_comparison p_or_n COMMA range_comparison p_or_n RPAREN
			 {
				Symbol * S = new Symbol(Symbol::srange_t, $3);
				S->Namespace = CurrentScope->Namespace;
				S->IsConstant = true;
				RangeT<int64_t> * NewRange = new RangeT<int64_t>($5, $6, $8, $9);
				S->ValidRange = NewRange;
				$$ = NewRange;
			 }
       ;
			 
constfrange : CONST FRANGE IDSTRING LPAREN range_comparison p_or_n_or_f COMMA range_comparison p_or_n_or_f RPAREN
			 {
				Symbol * S = new Symbol(Symbol::frange_t, $3);
				S->Namespace = CurrentScope->Namespace;
				S->IsConstant = true;
				RangeT<long double> * NewRange = new RangeT<long double>($5, $6, $8, $9);
				S->ValidRange = NewRange;
				$$ = NewRange;
			 }
       ;

uint_range : RANGE LPAREN range_comparison DIGITS COMMA range_comparison DIGITS RPAREN
					 {
							CurrentRange = new RangeT<uint64_t>($3, $4, $6, $7);
							$$ = CurrentRange;
					 }

					 | RANGE LPAREN range_comparison IDSTRING COMMA range_comparison DIGITS RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $4);
							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<uint64_t> * SValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
									CurrentRange = new RangeT<uint64_t>($3, SValue->Value, $6, $7);
									$$ = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << $4
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }

					 | RANGE LPAREN range_comparison DIGITS COMMA range_comparison IDSTRING RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $7);
							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<uint64_t> * SValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
									CurrentRange = new RangeT<uint64_t>($3, $4, $6, SValue->Value);
									$$ = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << $7
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }

					 | RANGE LPAREN range_comparison IDSTRING COMMA range_comparison IDSTRING RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							SymbolValueT<uint64_t> * SValue4 = nullptr;
							SymbolValueT<uint64_t> * SValue7 = nullptr;
							
							Symbol * Found4 = SymbolTable::Find(CurrentScope->Namespace, $4);
							if (Found4 != nullptr && Found4->Value != nullptr && Found4->IsConstant) {
								 	SValue4 = dynamic_cast<SymbolValueT<uint64_t>*>(Found4->Value);
									
							} else {
								  Global::Log << "ERROR: Symbol " << $4
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}

							Symbol * Found7 = SymbolTable::Find(CurrentScope->Namespace, $7);
							if (Found7 != nullptr && Found7->Value != nullptr && Found7->IsConstant) {
								 	SValue7 = dynamic_cast<SymbolValueT<uint64_t>*>(Found7->Value);
							} else {
								  Global::Log << "ERROR: Symbol " << $7
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
							if (SValue4 != nullptr && SValue7 != nullptr) {
									CurrentRange = new RangeT<uint64_t>($3, SValue4->Value, $6, SValue7->Value);
									$$ = CurrentRange;
							}
					 }

					 | RANGE LPAREN IDSTRING RPAREN
					 {
						Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $3);
						if (Found != nullptr && Found->Type == Symbol::urange_t) {
							 CurrentRange = Found->ValidRange;
							 
						} else {
								  Global::Log << "ERROR: uint Range Symbol " << $3
															<< " was not found,"
															<< " it must be alredy defined to be used in a Range\n";
									YYERROR;
						}
					 }
					 ;

opt_uint 	 : %empty
					 | opt_uint uint_default
					 | opt_uint uint_range
					 ;
					 
uintsingle : UINT IDSTRING opt_uint
           {
						Global::Log << "UNSIGNED INTEGER: " << $2;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $2);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;

						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						
						$$ = CurrentSymbol;
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING opt_uint
           {
						Global::Log << "UNSIGNED " << $3 << "-BIT INTEGER: " << $4;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $3, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						
						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						
						$$ = CurrentSymbol;
					 }
				 	 ;

uintfixedarray : UINT IDSTRING LSQUARE DIGITS RSQUARE opt_uint
           {
						Global::Log << "Fixed Array of " << $4
						     << " UNSIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $2, Symbol::FixedArray_t, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE opt_uint
           {
						Global::Log << "Fixed Array of " << $6
						     << " UNSIGNED " << $3
								 << "-BIT INTEGERS: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $3, $4, Symbol::FixedArray_t, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;
					 
uintvariablearray : UINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_uint
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " UNSIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $2, Symbol::VariableArray_t, $4, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_uint
					 {
						Global::Log << "Variable Array of " << $6
						     << " to " << $8 << " "
								 << $3 << "-BIT"
						     << " UNSIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | UINT IDSTRING LESS_THAN GREATER_THAN opt_uint
					 {
						Global::Log << "Variable Array of zero or more UNSIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $2, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN opt_uint
					 {
						Global::Log << "Variable Array of zero or more "
						     << $3 << "-BIT UNSIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						
						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;

opt_sint 	 : %empty
					 | opt_sint sint_default
					 | opt_sint sint_range
					 ;

p_or_n : DIGITS /* positive uint */
			 {
				$$ = (int64_t)$1;
			 }
			 | NDIGITS /* negative number sint */
			 {
				$$ = $1;
			 }
			 ;
			 
p_or_n_or_f : DIGITS /* positive uint */
			 {
				$$ = (long double)$1;
			 }
			 
			 | NDIGITS /* negative number sint */
			 {
				$$ = (long double)$1;
			 }
			 
			 | FDIGITS /* floatint point float */
			 ;
			 
sinttype : sintsingle
				 | sintfixedarray
				 | sintvariablearray
				 ;

sint_range : RANGE LPAREN range_comparison p_or_n COMMA range_comparison p_or_n RPAREN
					 {
							CurrentRange = new RangeT<int64_t>($3, $4, $6, $7);
							$$ = CurrentRange;
					 }

					 | RANGE LPAREN range_comparison IDSTRING COMMA range_comparison p_or_n RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $4);

							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<uint64_t> * UValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
									SymbolValueT<int64_t> * SValue = dynamic_cast<SymbolValueT<int64_t>*>(Found->Value);

									if (UValue != nullptr) {
										 CurrentRange = new RangeT<int64_t>($3, (int64_t)UValue->Value, $6, $7);
										 
									} else if (SValue != nullptr) {
										 CurrentRange = new RangeT<int64_t>($3, SValue->Value, $6, $7);
										 
									} else {
									  Global::Log << "ERROR: Symbol " << $4
																<< " was found, it had no uint or sint value.\n";
										YYERROR;
									}
									$$ = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << $4
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }

					 | RANGE LPAREN range_comparison p_or_n COMMA range_comparison IDSTRING RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $7);
							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<int64_t> * SValue = dynamic_cast<SymbolValueT<int64_t>*>(Found->Value);
								 	SymbolValueT<uint64_t> * UValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);

									if (SValue != nullptr) {
										CurrentRange = new RangeT<int64_t>($3, $4, $6, SValue->Value);
										
									} else if (UValue != nullptr) {
										CurrentRange = new RangeT<int64_t>($3, $4, $6, (int64_t)UValue->Value);
										
									} else {
									  Global::Log << "ERROR: Symbol " << $4
																<< " was found, it had no uint or sint value.\n";
										YYERROR;
									}
									$$ = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << $7
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }

					 | RANGE LPAREN range_comparison IDSTRING COMMA range_comparison IDSTRING RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							SymbolValueT<int64_t> * SValue4 = nullptr;
							SymbolValueT<uint64_t> * UValue4 = nullptr;
							
							SymbolValueT<int64_t> * SValue7 = nullptr;
							SymbolValueT<uint64_t> * UValue7 = nullptr;
							
							int64_t Max;
							bool GotMax = false;
							
							int64_t	Min;
							bool GotMin = false;
							
							Symbol * Found4 = SymbolTable::Find(CurrentScope->Namespace, $4);
							if (Found4 != nullptr && Found4->Value != nullptr && Found4->IsConstant) {
								 	SValue4 = dynamic_cast<SymbolValueT<int64_t>*>(Found4->Value);
								 	UValue4 = dynamic_cast<SymbolValueT<uint64_t>*>(Found4->Value);

									if (SValue4 != nullptr) {
										 Min = SValue4->Value;
										 GotMin = true;
										 
									} else if (UValue4 != nullptr) {
										Min = (int64_t)UValue4->Value;
										GotMin = true;
										
									} else {
								  	Global::Log << "ERROR: Symbol " << $4
																<< " was found, and did not have a uint or sint value.";
										YYERROR;
									}
							} else {
								  Global::Log << "ERROR: Symbol " << $4
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}

							Symbol * Found7 = SymbolTable::Find(CurrentScope->Namespace, $7);
							if (Found7 != nullptr && Found7->Value != nullptr && Found7->IsConstant) {
								 	SValue7 = dynamic_cast<SymbolValueT<int64_t>*>(Found7->Value);
								 	UValue7 = dynamic_cast<SymbolValueT<uint64_t>*>(Found7->Value);

									if (SValue7 != nullptr) {
										 Max = SValue7->Value;
										 GotMax = true;
										 
									} else if (UValue7 != nullptr) {
										Max = (int64_t)UValue7->Value;
										GotMax = true;
										
									} else {
								  	Global::Log << "ERROR: Symbol " << $7
																<< " was found, and did not have a uint or sint value.";
										YYERROR;
									}
									
							} else {
								  Global::Log << "ERROR: Symbol " << $7
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}

							if (GotMin && GotMax) {
									CurrentRange = new RangeT<int64_t>($3, Min, $6, Max);
							} else {
								  CurrentRange = nullptr;
							}
							$$ = CurrentRange;
					 }
					 
					 | RANGE LPAREN IDSTRING RPAREN
					 {
						Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $3);
						if (Found != nullptr && Found->Type == Symbol::srange_t) {
							 CurrentRange = Found->ValidRange;
							 
						} else {
								  Global::Log << "ERROR: sint Range Symbol " << $3
															<< " was not found,"
															<< " it must be alredy defined to be used in a Range\n";
									YYERROR;
						}
					 }
					 ;

sintsingle : SINT IDSTRING opt_sint
           {
						Global::Log << "SIGNED INTEGER: " << $2;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $2);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;

						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						
						$$ = CurrentSymbol;
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING opt_sint
           {
						Global::Log << "SIGNED " << $3 << "-BIT INTEGER: " << $4;
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $3, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						
						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						
						$$ = CurrentSymbol;
					 }
				 	 ;
sintfixedarray : SINT IDSTRING LSQUARE DIGITS RSQUARE opt_sint
           {
						Global::Log << "Fixed Array of " << $4
						     << " SIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $2, Symbol::FixedArray_t, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE opt_sint
           {
						Global::Log << "Fixed Array of " << $6
						     << " UNSIGNED " << $3
								 << "-BIT INTEGERS: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $3, $4, Symbol::FixedArray_t, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;
					 
sintvariablearray : SINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_sint
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " SIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $2, Symbol::VariableArray_t, $4, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_sint
					 {
						Global::Log << "Variable Array of " << $6
						     << " to " << $8 << " "
								 << $3 << "-BIT"
						     << " SIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | SINT IDSTRING LESS_THAN GREATER_THAN opt_sint
					 {
						Global::Log << "Variable Array of zero or more UNSIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $2, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN opt_sint
					 {
						Global::Log << "Variable Array of zero or more "
						     << $3 << "-BIT SIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::sint_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						
						CurrentSymbol->Default = CurrentSintDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;

opaquetype : opaquefixedarray
				 	 | opaquevariablearray
				 	 ;

opaquefixedarray : OPAQUE IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $4
						     << " OPAQUE: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::opaque_t, $2, Symbol::FixedArray_t, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;
					 
opaquevariablearray : OPAQUE IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " OPAQUE: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::opaque_t, $2, Symbol::VariableArray_t, $4, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | OPAQUE IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more OPAQUE: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::opaque_t, $2, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;


opt_float 	 : %empty
						 | opt_float float_default
					 	 | opt_float float_range
					 	 ;
					 
floattype : floatsingle
					| floatfixedarray
				 	| floatvariablearray
					;
				 
float_default : DEFAULT LPAREN p_or_n_or_f RPAREN 		/* DEFAULT(3) */
						 {
							CurrentFloatDefaultValue = new DefaultValueT<long double>((long double)$3);
							$$ = CurrentFloatDefaultValue;
						 }

						 | DEFAULT LPAREN IDSTRING RPAREN /* [DEFAULT=UpperLimitVar] */
						 {
						  /** IDSTRING must alredy be defined as a constant. */
							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $3);
							if (Found != nullptr) {
								 if (Found->IsConstant) {
								 		/* With float value, the default can be signed, unsigned, or float. */
								 		SymbolValueT<int64_t> * SVarValue = dynamic_cast<SymbolValueT<int64_t>*>(Found->Value);
										SymbolValueT<uint64_t> * UVarValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
										SymbolValueT<long double> * FVarValue = dynamic_cast<SymbolValueT<long double>*>(Found->Value);
										
										if (SVarValue != nullptr) {
										 CurrentFloatDefaultValue = new DefaultValueT<long double>((long double)SVarValue->Value);
										 $$ = CurrentFloatDefaultValue;
										 
										} else if (UVarValue != nullptr) {
											CurrentFloatDefaultValue = new DefaultValueT<long double>((long double)UVarValue->Value);
											$$ = CurrentFloatDefaultValue;
											
										} else if (FVarValue != nullptr) {
											CurrentFloatDefaultValue = new DefaultValueT<long double>((long double)FVarValue->Value);
											$$ = CurrentFloatDefaultValue;
											
									  } else {
								  		Global::Log << "ERROR: Symbol " << $3 << " was found, it did not have a uint, sint, or float value.\n";
											YYERROR;
								  	}
									} else {
								  		Global::Log << "ERROR: Symbol " << $3 << " was found, it must be a constant to be used in a Default.\n";
											YYERROR;
									}
							} else {
								  Global::Log << "ERROR: Symbol " << $3 << " was not found, it must be alredy defined and be a constant to be used in a Default.\n";
									YYERROR;
							}
						 }
						 ;

float_range : RANGE LPAREN range_comparison p_or_n_or_f COMMA range_comparison p_or_n_or_f RPAREN
					 {
							CurrentRange = new RangeT<long double>($3, $4, $6, $7);
							$$ = CurrentRange;
					 }

					 | RANGE LPAREN range_comparison IDSTRING COMMA range_comparison p_or_n RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $4);

							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<uint64_t> * UValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
									SymbolValueT<int64_t> * SValue = dynamic_cast<SymbolValueT<int64_t>*>(Found->Value);
									SymbolValueT<long double> * FValue = dynamic_cast<SymbolValueT<long double>*>(Found->Value);

									if (UValue != nullptr) {
										 CurrentRange = new RangeT<long double>($3, (long double)UValue->Value, $6, $7);
										 
									} else if (SValue != nullptr) {
										 CurrentRange = new RangeT<long double>($3, (long double)SValue->Value, $6, $7);
										 
									} else if (FValue != nullptr) {
										 CurrentRange = new RangeT<long double>($3, FValue->Value, $6, $7);
										 
									} else {
									  Global::Log << "ERROR: Symbol " << $4
																<< " was found, it had no uint, sint, or float value.\n";
										YYERROR;
									}
									$$ = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << $4
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }

					 | RANGE LPAREN range_comparison p_or_n_or_f COMMA range_comparison IDSTRING RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $7);
							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<int64_t> * SValue = dynamic_cast<SymbolValueT<int64_t>*>(Found->Value);
								 	SymbolValueT<uint64_t> * UValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
								 	SymbolValueT<long double> * FValue = dynamic_cast<SymbolValueT<long double>*>(Found->Value);

									if (SValue != nullptr) {
										CurrentRange = new RangeT<long double>($3, $4, $6, (long double)SValue->Value);
										
									} else if (UValue != nullptr) {
										CurrentRange = new RangeT<long double>($3, $4, $6, (long double)UValue->Value);
										
									} else if (FValue != nullptr) {
										CurrentRange = new RangeT<long double>($3, $4, $6, FValue->Value);
										
									} else {
									  Global::Log << "ERROR: Symbol " << $4
																<< " was found, it had no uint, sint, or float value.\n";
										YYERROR;
									}
									$$ = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << $7
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }

					 | RANGE LPAREN range_comparison IDSTRING COMMA range_comparison IDSTRING RPAREN
					 {
						  /** IDSTRING must alredy be defined as a constant. */

							SymbolValueT<int64_t> * SValue4 = nullptr;
							SymbolValueT<uint64_t> * UValue4 = nullptr;
							SymbolValueT<long double> * FValue4 = nullptr;
							
							SymbolValueT<int64_t> * SValue7 = nullptr;
							SymbolValueT<uint64_t> * UValue7 = nullptr;
							SymbolValueT<long double> * FValue7 = nullptr;
							
							long double Max;
							bool GotMax = false;
							
							long double	Min;
							bool GotMin = false;
							
							Symbol * Found4 = SymbolTable::Find(CurrentScope->Namespace, $4);
							if (Found4 != nullptr && Found4->Value != nullptr && Found4->IsConstant) {
								 	SValue4 = dynamic_cast<SymbolValueT<int64_t>*>(Found4->Value);
								 	UValue4 = dynamic_cast<SymbolValueT<uint64_t>*>(Found4->Value);
								 	FValue4 = dynamic_cast<SymbolValueT<long double>*>(Found4->Value);

									if (SValue4 != nullptr) {
										 Min = (long double)SValue4->Value;
										 GotMin = true;
										 
									} else if (UValue4 != nullptr) {
										Min = (long double)UValue4->Value;
										GotMin = true;
										
									} else if (FValue4 != nullptr) {
										Min = FValue4->Value;
										GotMin = true;
										
									} else {
								  	Global::Log << "ERROR: Symbol " << $4
																<< " was found, and did not have a uint, sint, or float value.";
										YYERROR;
									}
							} else {
								  Global::Log << "ERROR: Symbol " << $4
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}

							Symbol * Found7 = SymbolTable::Find(CurrentScope->Namespace, $7);
							if (Found7 != nullptr && Found7->Value != nullptr && Found7->IsConstant) {
								 	SValue7 = dynamic_cast<SymbolValueT<int64_t>*>(Found7->Value);
								 	UValue7 = dynamic_cast<SymbolValueT<uint64_t>*>(Found7->Value);
								 	FValue7 = dynamic_cast<SymbolValueT<long double>*>(Found7->Value);

									if (SValue7 != nullptr) {
										 Max = (long double)SValue7->Value;
										 GotMax = true;
										 
									} else if (UValue7 != nullptr) {
										Max = (long double)UValue7->Value;
										GotMax = true;
										
									} else if (FValue7 != nullptr) {
										Max = FValue7->Value;
										GotMax = true;
										
									} else {
								  	Global::Log << "ERROR: Symbol " << $7
																<< " was found, and did not have a uint, sint, or float value.";
										YYERROR;
									}
									
							} else {
								  Global::Log << "ERROR: Symbol " << $7
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}

							if (GotMin && GotMax) {
									CurrentRange = new RangeT<long double>($3, Min, $6, Max);
							} else {
								  CurrentRange = nullptr;
							}
							$$ = CurrentRange;
					 }
					 
					 | RANGE LPAREN IDSTRING RPAREN
					 {
						Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $3);
						if (Found != nullptr && Found->Type == Symbol::frange_t) {
							 CurrentRange = Found->ValidRange;
							 
						} else {
								  Global::Log << "ERROR: float Range Symbol " << $3
															<< " was not found,"
															<< " it must be alredy defined to be used in a Range\n";
									YYERROR;
						}
					 }
					 ;

floatsingle : FLOAT IDSTRING opt_float
           {
						Global::Log << "FLOAT: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $2);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING opt_float
           {
						Global::Log << $3 << "-BIT FLOAT: " << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $3, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentFloatDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
						}
				 	 ;

floatfixedarray : FLOAT IDSTRING LSQUARE DIGITS RSQUARE opt_float
           {
						Global::Log << "Fixed Array of " << $4 << " FLOAT: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $2, Symbol::FixedArray_t, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentFloatDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE opt_float
           {
						Global::Log << "Fixed Array of " << $6 << $3 << "-BIT FLOAT: " << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $3, $4, Symbol::FixedArray_t, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentFloatDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;
					 
floatvariablearray : FLOAT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_float
					 {
						Global::Log << "Variable Array of " << $4 << " to " << $6 << " FLOAT: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $2, Symbol::VariableArray_t, $4, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentFloatDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_float
					 {
						Global::Log << "Variable Array of " << $6 << " to " << $8 << " " << $3 << "-BIT" << " FLOAT: " << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentFloatDefaultValue;
						CurrentSintDefaultValue = nullptr;

						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | FLOAT IDSTRING LESS_THAN GREATER_THAN opt_float
					 {
						Global::Log << "Variable Array of zero or more FLOAT: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $2, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						
						CurrentSymbol->Default = CurrentFloatDefaultValue;
						CurrentSintDefaultValue = nullptr;
						
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN opt_float
					 {
						Global::Log << "Variable Array of zero or more " << $3 << "-BIT FLOAT: " << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::float_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;

						CurrentSymbol->Default = CurrentFloatDefaultValue;
						CurrentSintDefaultValue = nullptr;

						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						$$ = CurrentSymbol;
					 }
					 ;

opt_string 	 : %empty
					 | opt_string string_default
					 ;
					 
string_default : DEFAULT LPAREN anystring RPAREN
							 {
								CurrentStringDefaultValue = new DefaultValueT<std::string>($3);
								$$ = CurrentStringDefaultValue;
							 }
							 ;
							 
stringtype : stringsingle
					| stringfixedarray
				 	| stringvariablearray
					;
				 
stringsingle : STRING IDSTRING opt_string
           {
						Global::Log << "STRING: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $2);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING opt_string
           {
						Global::Log << $3
						     << "-BIT STRING: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $3, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
				 	 ;

stringfixedarray : STRING IDSTRING LSQUARE DIGITS RSQUARE opt_string
           {
						Global::Log << "Fixed Array of " << $4
						     << " STRING: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $2, Symbol::FixedArray_t, $4);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE opt_string
           {
						Global::Log << "Fixed Array of " << $6
						     << $3
								 << "-BIT STRING: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $3, $4, Symbol::FixedArray_t, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
					 ;
					 
stringvariablearray : STRING IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_string
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " STRING: "
								 << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $2, Symbol::VariableArray_t, $4, $6);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_string
					 {
						Global::Log << "Variable Array of " << $6
						     << " to " << $8 << " "
								 << $3 << "-BIT"
						     << " STRING: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
					 
				 	 | STRING IDSTRING LESS_THAN GREATER_THAN opt_string
					 {
						Global::Log << "Variable Array of zero or more STRING: " << $2;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $2, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING LESS_THAN GREATER_THAN opt_string
					 {
						Global::Log << "Variable Array of zero or more "
						     << $3 << "-BIT STRING: "
								 << $4;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::string_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						$$ = CurrentSymbol;
					 }
					 ;

opt_const : %empty
					 {
						$$ = false;
					 }
					 | CONST
					 {
						$$ = true;
					 }
					 ;
					 
methodsig : FUNCTION mreturn IDSTRING LPAREN parameters RPAREN opt_const
					{
						Global::Log << "Method: " << $3;
 						PrintLine();
						CurrentSymbol = new Symbol(Symbol::method_t, $3);
						CurrentSymbol->IsConstant = $7;
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->MethodReturnType = $2;
						CurrentSymbol->MethodParameters = $5;
						CurrentParameters = nullptr;		/* Just used (saved) them in $4 */
						$$ = CurrentSymbol;
					}
					;

bool : BOOL
		 {
			CurrentReturnType = new Symbol(Symbol::bool_t, 1, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | BOOL LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::bool_t, 1, Symbol::FixedArray_t, $3);
			$$ = CurrentReturnType;
		 }
		 
		 | BOOL LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::bool_t, 1, Symbol::VariableArray_t, $3, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | BOOL LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::bool_t, 1, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 ;
		 
uint : UINT
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, 64, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | UINT COLON DIGITS
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, $3, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | UINT LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, 64, Symbol::FixedArray_t, $3);
			$$ = CurrentReturnType;
		 }
		 
		 | UINT COLON DIGITS LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, $3, Symbol::FixedArray_t, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | UINT LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, 64, Symbol::VariableArray_t, $3, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | UINT COLON DIGITS LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, $3, Symbol::VariableArray_t, $5, $7);
			$$ = CurrentReturnType;
		 }
		 
		 | UINT LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, 64, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 
		 | UINT COLON DIGITS LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::uint_t, $3, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 ;
		 
sint : SINT
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, 64, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | SINT COLON DIGITS
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, $3, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | SINT LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, 64, Symbol::FixedArray_t, $3);
			$$ = CurrentReturnType;
		 }
		 
		 | SINT COLON DIGITS LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, $3, Symbol::FixedArray_t, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | SINT LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, 64, Symbol::VariableArray_t, $3, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | SINT COLON DIGITS LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, $3, Symbol::VariableArray_t, $5, $7);
			$$ = CurrentReturnType;
		 }
		 
		 | SINT LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, 64, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 
		 | SINT COLON DIGITS LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::sint_t, $3, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 ;
		 
float : FLOAT
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, 0, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | FLOAT COLON DIGITS
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, $3, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | FLOAT LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, 0, Symbol::FixedArray_t, $3);
			$$ = CurrentReturnType;
		 }
		 
		 | FLOAT COLON DIGITS LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, $3, Symbol::FixedArray_t, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | FLOAT LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, 0, Symbol::VariableArray_t, $3, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | FLOAT COLON DIGITS LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, $3, Symbol::VariableArray_t, $5, $7);
			$$ = CurrentReturnType;
		 }
		 
		 | FLOAT LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, 0, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 
		 | FLOAT COLON DIGITS LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::float_t, $3, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 ;
		 
string : STRING
		 {
			CurrentReturnType = new Symbol(Symbol::string_t, 0, Symbol::NotArray_t);
			$$ = CurrentReturnType;
		 }
		 
		 | STRING LSQUARE DIGITS RSQUARE
		 {
			CurrentReturnType = new Symbol(Symbol::string_t, 0, Symbol::FixedArray_t, $3);
			$$ = CurrentReturnType;
		 }
		 
		 | STRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::string_t, 0, Symbol::VariableArray_t, $3, $5);
			$$ = CurrentReturnType;
		 }
		 
		 | STRING LESS_THAN GREATER_THAN
		 {
			CurrentReturnType = new Symbol(Symbol::string_t, 0, Symbol::VariableArray_t, 0, 0);
			$$ = CurrentReturnType;
		 }
		 ;

opaque : OPAQUE LSQUARE DIGITS RSQUARE
			 {
				CurrentReturnType = new Symbol(Symbol::opaque_t, 8, Symbol::VariableArray_t, 0, 0);
				$$ = CurrentReturnType;
			 }

			 | OPAQUE LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
			 {
				CurrentReturnType = new Symbol(Symbol::opaque_t, 8, Symbol::VariableArray_t, $3, $5);
				$$ = CurrentReturnType;
			 }
			 ;

class : IDSTRING
			{
					/* IDSTRING must be the name of an existing class */

				Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, $1);
				if (Found != nullptr && Found->Type == Symbol::class_t) {
					 CurrentReturnType = Found;
					 $$ = Found;
				} else {
					Global::Log << "ERROR: Symbol " << $1 << " was not found, or it was not a 'class' type.";
					YYERROR;
				}
			}
			;

void	: VOID
			{
				CurrentReturnType = new Symbol(Symbol::void_t, 0, Symbol::NotArray_t);
				$$ = CurrentReturnType;
			}
			;
			
mreturn : class /*simpledatatype*/
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				
				| CONST class
				{
					$$ = $2;
					$$->IsConstant = true;
				}
				
				| bool
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				| CONST bool
				{
					$$ = $2;
					$$->IsConstant = true;
				}

				| uint
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				| CONST uint
				{
					$$ = $2;
					$$->IsConstant = true;
				}
				
				| sint
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				| CONST sint
				{
					$$ = $2;
					$$->IsConstant = true;
				}
				
				| float
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				| CONST float
				{
					$$ = $2;
					$$->IsConstant = true;
				}
				
				| string
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				| CONST string
				{
					$$ = $2;
					$$->IsConstant = true;
				}
				
				| opaque
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				| CONST opaque
				{
					$$ = $2;
					$$->IsConstant = true;
				}
				
				| void
				{
					$$ = $1;
					$$->IsConstant = false;
				}
				;
							
parameters : %empty
					 {
						$$ = nullptr;
					 }
					 | paramtype
					 {
						if (CurrentParameters == nullptr) {
							 CurrentParameters = new std::vector<Symbol*>();
						}
						CurrentParameters->push_back($1);
						$$ = CurrentParameters;
					 }
					 | parameters COMMA paramtype
					 {
						if (CurrentParameters == nullptr) {
							 CurrentParameters = new std::vector<Symbol*>();
						}
						CurrentParameters->push_back($3);
						$$ = CurrentParameters;
					 }
					 ;

paramtype : simpletype
					| classtype
					;

%%
  // A string used to build namespace names.
	//
	std::string BuildingNS;
	
	Control			CurrentProtocol;

 	bool Building_CBOR_Client = false;
 	bool Building_CBOR_Server = false;

	bool Building_JSON_Client = false;
	bool Building_JSON_Server = false;

 	bool Building_NATIVE_Client = false;
 	bool Building_NATIVE_Server = false;

	GenerateCPP * CPP = nullptr;
	
 	bool Building_REST_Client = false;
 	bool Building_REST_Server = false;

	bool Building_XDR_Client = false;
	bool Building_XDR_Server = false;

 	bool Building_XML_Client = false;
 	bool Building_XML_Server = false;

	Scope												*	CurrentScope = nullptr;
	Symbol											*	CurrentSymbol = nullptr;
	Symbol::Visibility_e					CurrentVisibility = Symbol::Public_t;
	Range												*	CurrentRange = nullptr;
	DefaultValueT<uint64_t>			*	CurrentUintDefaultValue = nullptr;
	DefaultValueT<int64_t>			*	CurrentSintDefaultValue = nullptr;
	DefaultValueT<long double>	*	CurrentFloatDefaultValue = nullptr;
	DefaultValueT<std::string>	*	CurrentStringDefaultValue = nullptr;
	DefaultValueT<bool>					*	CurrentBoolDefaultValue = nullptr;
	
	std::vector<Symbol*>				*	CurrentParameters;
	std::string										CurrentNamespace;
	std::vector<SymbolValue*> 	*	CurrentEnumDefs;

	Symbol											*	CurrentReturnType = nullptr;

	bool					PackingOrderSet = false;
	bool					PackingSizeSet = false;

  int						LineNumber = 1;

  // FLEX or BISON wants this, I did not use it.
	//
	void
	RiverExplorer::Phoenix::Protocol::Parser::error(RiverExplorer::Phoenix::Protocol::location const & L,
			 																			 const std::string & Msg)
																						 {
																						  Global::Log << Msg;
																							 						PrintLine();
																						 }
 void
 PrintLine()
 {
   Global::Log << " (Line: " << LineNumber << ")\n";
	 Global::Log.flush();
	 
 }