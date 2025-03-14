grammar xdr;

xdrSpecification
    : specs+
    ; //this is the top level rule for xdr (rfc 4506)

specs
	: structTypeSpec
	| unionTypeSpec
	| enumTypeSpec
	| constantDef
	| typeDefDef
	| namespaceDef
	| comment
	| passThrough
	;
	
declaration
		: var
		| varPtr
		| varFixed
		| varFixedPtr
		| varVariable
		| varVariablePtr
		| opaqueFixed
		| opaqueFixedPtr
		| opaqueVariable
		| opaqueVariablePtr
		| string
		| stringPtr
		| void
		| comment
		;

var								: typeSpecifier IDENTIFIER ;
varPtr						: typeSpecifier '*' IDENTIFIER ;

varFixed					: typeSpecifier IDENTIFIER '[' value ']' ;
varFixedPtr				: typeSpecifier '*' IDENTIFIER '[' value ']' ;

varVariable				: typeSpecifier IDENTIFIER '<' value? '>' ;
varVariablePtr		: typeSpecifier '*' IDENTIFIER '<' value? '>' ;

opaqueFixed				: 'opaque' IDENTIFIER '[' value ']' ;
opaqueFixedPtr		: 'opaque' '*' IDENTIFIER '[' value ']' ;

opaqueVariable		: 'opaque' IDENTIFIER '<' value? '>' ;
opaqueVariablePtr	: 'opaque' '*' IDENTIFIER '<' value? '>' ;

string						: 'string' IDENTIFIER '<' value? '>' ;
stringPtr					: 'string' '*' IDENTIFIER '<' value? '>' ;

void							: 'void' ;

namespaceDef
		: 'namespace' IDENTIFIER ( ':' IDENTIFIER )* ';'
		;

value
    : constant
    | IDENTIFIER
    | QIDENTIFIERSingle
    | QIDENTIFIERDouble
    ;

constant
    : DECIMAL
    | HEXADECIMAL
    | OCTAL
    ;

typeSpecifier
		: 'char'
		| 'uint8_t'
		|	'int8_t'
		| 'uint16_t'
		| 'int16_t'
		| 'uint32_t'
		| 'int32_t'
		| 'uint64_t'
		| 'int64_t'
    | 'float'
    | 'double'
    | 'bool'
    | 'bool_t'
		| 'void'
    | enumTypeSpec
    | structTypeSpec
    | unionTypeSpec
    | IDENTIFIER
    ;

enumTypeSpec
    : 'enum' IDENTIFIER enumBody ';'
    ;

enumBody
    : '{' (IDENTIFIER '=' value) (',' IDENTIFIER '=' value)* '}'
    ;

structTypeSpec
    : ('struct'|'class') IDENTIFIER '{' structBody '}' ';'
    ;

structBody
    : ((declaration ';') | comment+ | method+) ((declaration ';') | comment+ | method+)*
    ;

unionTypeSpec
    : 'union' IDENTIFIER unionBody ';'
    ;

unionBody
    : 'switch' '(' declaration ')' '{' caseSpec caseSpec* ('default' ':' declaration ';')? '}'
    ;

caseSpec
    : (('case' value':' declaration ) | comment+ )
			(('case' value':' declaration) | comment+)* ';'
    ;

typeDefDef
		: 'typedef' declaration ';'
		| 'typedef' IDENTIFIER IDENTIFIER ';'
		| 'typedef' IDENTIFIER '*' IDENTIFIER ';'
		| 'typedef' IDENTIFIER IDENTIFIER '[' value ']' ';'
		| 'typedef' IDENTIFIER '*' IDENTIFIER '[' value ']' ';'
		| 'typedef' IDENTIFIER IDENTIFIER '<>' ';'
		| 'typedef' IDENTIFIER '*' IDENTIFIER '<>' ';'
		| 'typedef' IDENTIFIER IDENTIFIER '<' value '>' ';'
		| 'typedef' IDENTIFIER '*' IDENTIFIER '<' value '>' ';'
		| 'typedef' 'struct' IDENTIFIER IDENTIFIER ';'
		| 'typedef' 'struct' IDENTIFIER '*' IDENTIFIER ';'
		| 'typedef' 'struct' IDENTIFIER IDENTIFIER '[' value ']' ';'
		| 'typedef' 'struct' IDENTIFIER '*' IDENTIFIER '[' value ']' ';'
		| 'typedef' 'struct' IDENTIFIER IDENTIFIER '<>' ';'
		| 'typedef' 'struct' IDENTIFIER '*' IDENTIFIER '<>' ';'
		| 'typedef' 'struct' IDENTIFIER IDENTIFIER '<' value '>' ';'
		| 'typedef' 'struct' IDENTIFIER '*' IDENTIFIER '<' value '>' ';'
		;
		
constantDef
    : 'const' IDENTIFIER '=' constant ';'
    ;

typeDef
    : 'typedef' declaration ';'
    | 'enum' IDENTIFIER enumBody ';'
    | ('struct'|'class') IDENTIFIER structBody ';'
    | 'union' IDENTIFIER unionBody ';'
    ;

definition
    : typeDef
    | constantDef
    ;


comment : CommentOneLine
				| CommentMultiLine ;

CommentOneLine : '//' ~[\r\n]+ ;

CommentMultiLine : '/*' .*? '*/' ;

passThrough : PASS
					 ;

method: procReturn IDENTIFIER '(' procFirstArg (',' typeSpecifier)* ')' ';'
			;

procReturn
			: 'void'
			| typeSpecifier
			;

procFirstArg
			: 'void' ('*' | '&')* IDENTIFIER*
			| typeSpecifier ('*' | '&')* IDENTIFIER*
			;
			
// lexer rules

//COMMENT
//    : '/*' .*? '*/' -> skip
//    ;

PASS
		: '%' ~[\n\r]+
		;
		
OCTAL
    : '0' [1-7] ([0-7])*
    ;

DECIMAL
    : ('-')? ([0-9])+
    ;

HEXADECIMAL
    : '0x' ([a-fA-F0-9])+
    ;

IDENTIFIER
    : [a-zA-Z] ([a-zA-Z0-9_])*
    ;

QIDENTIFIERSingle
		: '\'' [a-zA-Z] ([a-zA-Z0-9_])* '\''
		;

QIDENTIFIERDouble
		: '"' [a-zA-Z] ([a-zA-Z0-9_])* '"'
		;

WS
    : [ \t\r\n]+ -> skip
    ;
