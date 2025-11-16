%{

/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "Scanner.hpp"

extern int LineNumber;

#undef  YY_DECL

#define YY_DECL int RiverExplorer::Phoenix::Protocol::Scanner::yylex(RiverExplorer::Phoenix::Protocol::Parser::semantic_type * const lval, RiverExplorer::Phoenix::Protocol::Parser::location_type *location)

/* using "token" to make the returns for the tokens shorter to type */
using token = RiverExplorer::Phoenix::Protocol::Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
/*#define YY_NO_UNISTD_H*/

/* update location on matching */
#define YY_USER_ACTION Loc->step(); Loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="RiverExplorer::Phoenix::Protocol::Scanner"
%option noyywrap
%option c++

%option yylineno

%x DSTRING
%x SSTRING
%x IN_VERSION

%%
%{
	std::string * NewString;
	
	 /** Code executed at at the beginning of yylex */

	 yylval = lval;

%}

protocol {
	return(token::PROTOCOL);
}

version {
	return(token::VERSION);
}

Namespace {
  return(token::NAMESPACE);
}

include {
	return(token::INCLUDE);
}

C\+\+ {
	return(token::GEN_CPP_CODE);
}
			 
NoC\+\+ {
	return(token::NO_GEN_CPP_CODE);
}

C# {
	return(token::GEN_CSHARP_CODE);
}

NoC# {
	return(token::NO_GEN_CSHARP_CODE);
}

GenCBOR {
	return(token::GEN_CBOR);
}

NoGenCBOR {
	return(token::NO_GEN_CBOR);
}

GenCBORClient {
	return(token::GEN_CBOR_CLIENT);
}

NoGenCBORClient {
	return(token::NO_GEN_CBOR_CLIENT);
}

GenCBORServer {
	return(token::GEN_CBOR_SERVER);
}

NoCBORServer {
	return(token::NO_GEN_CBOR_SERVER);
}

GenJSON {
	return(token::GEN_JSON);
}

NoGenJSON {
	return(token::NO_GEN_JSON);
}

GenJSONClient {
	return(token::NO_GEN_JSON_CLIENT);
}

NoGenJSONClient {
	return(token::NO_GEN_JSON_CLIENT);
}

GenJSONServer {
	return(token::GEN_JSON_SERVER);
}

NoGenJSONServer {
	return(token::NO_GEN_JSON_SERVER);
}

GenNATIVE {
	return(token::GEN_NATIVE);
}

NoGenNATIVE {
	return(token::NO_GEN_NATIVE);
}

GenNATIVEClient {
	return(token::GEN_NATIVE_CLIENT);
}

NoGenNATIVEClient {
	return(token::NO_GEN_NATIVE_CLIENT);
}

GenNATIVEServer {
	return(token::GEN_NATIVE_SERVER);
}

NoGenNATIVEServer {
	return(token::NO_GEN_NATIVE_SERVER);
}

GenREST {
	return(token::GEN_REST);
}

NoGenREST {
	return(token::NO_GEN_REST);
}

GenRESTClient {
	return(token::GEN_REST_CLIENT);
}

NoGenRESTClient {
	return(token::NO_GEN_REST_CLIENT);
}

GenRESTServer {
	return(token::GEN_REST_SERVER);
}

NoGenRESTServer {
	return(token::NO_GEN_REST_SERVER);
}

GenXDR {
	return(token::GEN_XDR);
}

NoGenXDR {
	return(token::NO_GEN_XDR);
}

GenXDRClient {
	return(token::GEN_XDR_CLIENT);
}

NoGenXDRClient {
	return(token::NO_GEN_XDR_CLIENT);
}

GenXDRServer {
	return(token::GEN_XDR_SERVER);
}

NoGenXDRServer {
	return(token::NO_GEN_XDR_SERVER);
}

GenXML {
	return(token::GEN_XML);
}

NoGenXML {
	return(token::NO_GEN_XML);
}

GenXMLClient {
	return(token::GEN_XML_CLIENT);
}

NoGenXMLClient {
	return(token::NO_GEN_XML_CLIENT);
}

GenXMLServer {
	return(token::GEN_XML_SERVER);
}

NoGenXMLServer {
	return(token::NO_GEN_XML_SERVER);
}

PackLR {
 return(token::PACKLR);
}

PackRL {
 return(token::PACKRL);
}

PackNBO {
 return(token::PACKNBO);
}

BITSTREAM {
 return(token::BITSTREAM);
}

PackSize {
 yylval->emplace<uint64_t>(atoi(yytext));
 return(token::PACKSIZE);
}

DefaultPackSize {
 yylval->emplace<uint64_t>(atoi(yytext));
 return(token::DEFAULTPACKSIZE);
}

DOCUMENT {
	return(token::DOCUMENT);
}

string {
	return(token::STRING);
}
	
void {
	return(token::VOID);
}

opaque {
	return(token::OPAQUE);
}

uint {
 return(token::UINT);
}
 
sint {
 return(token::SINT);
}
 
float {
 return(token::FLOAT);
}
 
class {
 return(token::CLASS);
}

Range {
	return(token::RANGE);
}

Default {
	return(token::DEFAULT);
}

\" {
	NewString = new std::string;
	BEGIN(DSTRING);
}


\' {
	NewString = new std::string;
	BEGIN(SSTRING);
}

<DSTRING>\" {
		 BEGIN(INITIAL);
		 yylval->emplace<std::string>(*NewString);
		 return(token::DQSTRING);
}

<SSTRING>\' {
		 BEGIN(INITIAL);
		 yylval->emplace<std::string>(*NewString);
		 return(token::SQSTRING);
}

<DSTRING>\\n {
		NewString->append("\n");
}

<SSTRING>\\n {
		NewString->append("\n");
}

<DSTRING>\\t {
		NewString->append("\t");
}

<SSTRING>\\t {
		NewString->append("\t");
}

<DSTRING>\n {
	Global::Log << "Newline in string constant";
}

<SSTRING>\n {
	Global::Log << "Newline in string constant";
}

<DSTRING>\\. {
	  /* Handle other escaped character */
		NewString->append("\\");
		(*NewString) += yytext[1];
}

<SSTRING>\\. {
	  /* Handle other escaped character */
		NewString->append("\\");
		(*NewString) += yytext[1];
}

<DSTRING>[^\\\n\"]+ {
		NewString->append(yytext);		
}

<SSTRING>[^\\\n\']+ {
		NewString->append(yytext);		
}

[-][0-9]+ {
	yylval->emplace<int64_t>(atoi(yytext));
	return(token::NDIGITS);
}

[0-9]+ {
	yylval->emplace<uint64_t>(atoi(yytext));
	return(token::DIGITS);
}

[+-]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][+-]?[0-9]+)? {
	yylval->emplace<long double>(atoi(yytext));
	return(token::FDIGITS);
}

"//".*[&\n] {
	return(token::COMMENT);
}

"<" {
	return(token::LESS_THAN);
}

"<=" {
	return(token::LESS_THAN_OR_EQUAL);
}

"=" {
	return(token::EQUAL);
}

">=" {
	return(token::GREATER_THAN_OR_EQUAL);
}

">" {
	return(token::GREATER_THAN);
}

[{] {
	return(token::LCURLY);
}

[}] {
	return(token::RCURLY);
}

[(] {
	return(token::LPAREN);
}

[)] {
	return(token::RPAREN);
}

[\[] {
	return(token::LSQUARE);
}

[\]] {
	return(token::RSQUARE);
}

[,] {
	return(token::COMMA);
}

[;] {
	return(token::SEMI);
}

[:] {
	return(token::COLON);
}

 /*
  * [\.] {
  *	return(token::DOT);
  *}
  */

\n {
	/** Update the line number */
  LineNumber++;
	Loc->lines();
}

[a-zA-Z_][a-zA-Z0-9_]+ {
				std::cout<< "IDSTRING: " << yytext << std::endl;
				std::string Tmp(yytext);
				yylval->emplace<std::string>(Tmp);
				return(token::IDSTRING);
}

 /*
 . {
 	return(token::CHAR);
 }
 */

[ \t]+ ;

