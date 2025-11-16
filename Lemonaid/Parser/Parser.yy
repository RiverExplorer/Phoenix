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
#include "GenerateNATIVE.hpp"

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
extern RiverExplorer::Phoenix::Protocol::Range * CurrentRange;
extern RiverExplorer::Phoenix::Protocol::DefaultValue * CurrentDefaultValue;

/*  PACKING */
extern bool PackingOrderSet;
extern bool PackingSizeSet;
extern std::string BuildingNS;

/* OUTPUT DIRS */
extern GenerateNATIVE * Native;

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

%token									COMMENT		/* // */

%token 									LCURLY		/* { */
%token 									RCURLY		/* } */

%token 									LPAREN		/* ( */
%token 									RPAREN		/* ) */

%token 									LSQUARE		/* [ */
%token 									RSQUARE		/* ] */

%token 									COMMA			/* , */
%token 									SEMI			/* ; */
%token 									COLON			/* : */

%token <DefaultValue*>	DEFAULT
%type  <DefaultValue*>	uint_default

%token									RANGE
%token 									LESS_THAN							/* < */
%token 									LESS_THAN_OR_EQUAL		/* <= */
%token 									EQUAL									/* = */
%token 									GREATER_THAN_OR_EQUAL	/* >= */
%token 									GREATER_THAN					/* > */

%type <Range::RangeCmp_e> range_comparison
%type <Range*>					 uint_range

%token 									PROTOCOL
%token 									VERSION

%token 		<std::string>	DQSTRING /* Double quotes string "abc" */
%token 		<std::string>	SQSTRING /* Single quotes string 'abc' */

%token									CLASS
%token									UINT
%token									SINT
%token									FLOAT
%token									VOID
%token									OPAQUE

%token									PACKNBO
%token									PACKLR
%token									PACKRL
%token									BITSTREAM
%token		<uint64_t>		PACKSIZE
%token		<uint64_t>		DEFAULTPACKSIZE

%token		<std::string>	INCLUDE

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

%token									NAMESPACE

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

%locations /* THEY DO NOT SEEM TO WORK */

%%

protocol : init comments protocoldirectives PROTOCOL IDSTRING LCURLY versions RCURLY SEMI {
				 Global::Log << "Protocol ID: " << $5;
						PrintLine();
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
				/**@todo*/
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
						 CurrentScope->Namespace = $3;
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
				 
version : VERSION IDSTRING LCURLY contents RCURLY SEMI
				{
					Global::Log << "Protocol Version: " << $2;
						PrintLine();
				}
				;

contents : content
				 | contents content;
				 ;

content : simpletype SEMI
				| classdef SEMI
				| classtype SEMI
				| methodsig SEMI
				| comments
				;

simpletype : uinttype
					 | sinttype
					 | floattype
					 | stringtype
					 | opaquetype
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
					 
uinttype : uintsingle
				 | uintfixedarray
				 | uintvariablearray
				 ;

uint_default : LCURLY DEFAULT EQUAL DIGITS RCURLY		/* [DEFAULT=3] */
						 {
							CurrentDefaultValue = new DefaultValueT<uint64_t>($4);
							$$ = CurrentDefaultValue;
						 }

						 | LCURLY DEFAULT EQUAL IDSTRING	RCURLY /* [DEFAULT=UpperLimitVar] */
						 {
							CurrentDefaultValue = new DefaultValueT<std::string>($4);
							$$ = CurrentDefaultValue;
						 }
						 ;

uint_range : LCURLY RANGE COLON range_comparison DIGITS COMMA range_comparison DIGITS RCURLY
					 {
							CurrentRange = new RangeT<uint64_t>($4, $5, $7, $8);
							$$ = CurrentRange;
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
						CurrentSymbol->Default = CurrentDefaultValue;
						CurrentDefaultValue = nullptr;

						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING
           {
						Global::Log << "UNSIGNED " << $3
						     << "-BIT INTEGER: "
								 << $4;
						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, $3, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
				 	 ;

uintfixedarray : UINT IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $4
						     << " UNSIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, $2, Symbol::FixedArray_t, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $6
						     << " UNSIGNED " << $3
								 << "-BIT INTEGERS: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, $3, $4, Symbol::FixedArray_t, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;
					 
uintvariablearray : UINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " UNSIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, $2, Symbol::VariableArray_t, $4, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $6
						     << " to " << $8 << " "
								 << $3 << "-BIT"
						     << " UNSIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | UINT IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more UNSIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, $2, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | UINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more "
						     << $3 << "-BIT UNSIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;

sinttype : sintsingle
				 | sintfixedarray
				 | sintvariablearray
				 ;
				 
sintsingle : SINT IDSTRING
           {
						Global::Log << "SIGNED INTEGER: " << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $2);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING
           {
						Global::Log << "SIGNED " << $3
						     << "-BIT INTEGER: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $3, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
				 	 ;

sintfixedarray : SINT IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $4
						     << " SIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $2, Symbol::FixedArray_t, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $6
						     << " SIGNED " << $3
								 << "-BIT INTEGERS: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $3, $4, Symbol::FixedArray_t, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;
					 
sintvariablearray : SINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " SIGNED INTEGERS: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $2, Symbol::VariableArray_t, $4, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $6
						     << " to " << $8 << " "
								 << $3 << "-BIT"
						     << " SIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | SINT IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more SIGNED INTEGERS: " << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $2, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | SINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more "
						     << $3 << "-BIT SIGNED INTEGERS: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
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
						Symbol * NewOne = new Symbol( Symbol::opaque_t, $2, Symbol::FixedArray_t, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;
					 
opaquevariablearray : OPAQUE IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " OPAQUE: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::opaque_t, $2, Symbol::VariableArray_t, $4, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | OPAQUE IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more OPAQUE: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::opaque_t, $2, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;


floattype : floatsingle
					| floatfixedarray
				 	| floatvariablearray
					;
				 
floatsingle : FLOAT IDSTRING
           {
						Global::Log << "FLOAT: " << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $2);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING
           {
						Global::Log << $3
						     << "-BIT FLOAT: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $3, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
				 	 ;

floatfixedarray : FLOAT IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $4
						     << " FLOAT: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $2, Symbol::FixedArray_t, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $6
						     << $3
								 << "-BIT FLOAT: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $3, $4, Symbol::FixedArray_t, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;
					 
floatvariablearray : FLOAT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " FLOAT: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $2, Symbol::VariableArray_t, $4, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $6
						     << " to " << $8 << " "
								 << $3 << "-BIT"
						     << " FLOAT: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | FLOAT IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more FLOAT: " << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $2, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | FLOAT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more "
						     << $3 << "-BIT FLOAT: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;

stringtype : stringsingle
					| stringfixedarray
				 	| stringvariablearray
					;
				 
stringsingle : STRING IDSTRING
           {
						Global::Log << "STRING: " << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $2);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING
           {
						Global::Log << $3
						     << "-BIT STRING: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $3, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
				 	 ;

stringfixedarray : STRING IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $4
						     << " STRING: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $2, Symbol::FixedArray_t, $4);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
           {
						Global::Log << "Fixed Array of " << $6
						     << $3
								 << "-BIT STRING: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $3, $4, Symbol::FixedArray_t, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;
					 
stringvariablearray : STRING IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $4
						     << " to " << $6
						     << " STRING: "
								 << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $2, Symbol::VariableArray_t, $4, $6);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
					 {
						Global::Log << "Variable Array of " << $6
						     << " to " << $8 << " "
								 << $3 << "-BIT"
						     << " STRING: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $3, $4, Symbol::VariableArray_t, $6, $8);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | STRING IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more STRING: " << $2;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $2, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 
				 	 | STRING COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
					 {
						Global::Log << "Variable Array of zero or more "
						     << $3 << "-BIT STRING: "
								 << $4;
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, $3, $4, Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
					 ;

methodsig : mreturn IDSTRING LPAREN parameters RPAREN
					{
						Global::Log << "Method: " << $2
						     << " Returning: " << $2; /**@todo*/
 						PrintLine();
					}
					;

mreturn : IDSTRING /*simpledatatype*/
				| VOID
				;
							
parameters : %empty
					 | paramtype
					 | parameters paramtype
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
	GenerateNATIVE * Native = nullptr;
	
 	bool Building_REST_Client = false;
 	bool Building_REST_Server = false;

	bool Building_XDR_Client = false;
	bool Building_XDR_Server = false;

 	bool Building_XML_Client = false;
 	bool Building_XML_Server = false;

	Scope					*		CurrentScope = nullptr;
	Symbol				*		CurrentSymbol = nullptr;
	Range					*		CurrentRange = nullptr;
	DefaultValue	*		CurrentDefaultValue = nullptr;

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