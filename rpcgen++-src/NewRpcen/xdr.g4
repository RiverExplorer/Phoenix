grammar xdr;

xdrSpecification
    : specs+
    ; //this is the top level rule for xdr (rfc 4506)

specs
	: structTypeSpec
	| unionTypeSpec
	| enumTypeSpec
	| constantDef
	| typedefDef
	| namespaceDef
	| comment
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
    ;

constant
    : DECIMAL
    | HEXADECIMAL
    | OCTAL
    ;

typeSpecifier
    : 'unsigned'? 'int'
    | 'unsigned'? 'hyper'
    | 'float'
    | 'double'
    | 'quadruple'
    | 'bool'
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
    : 'struct' IDENTIFIER structBody ';'
    ;

structBody
    : '{' (declaration ';') (declaration ';')* '}'
    ;

unionTypeSpec
    : 'union' IDENTIFIER unionBody ';'
    ;

unionBody
    : 'switch' '(' declaration ')' '{' caseSpec caseSpec* ('default' ':' declaration ';')? '}'
    ;

caseSpec
    : (('case' value':' declaration ) | comment )
			(('case' value':' declaration) | comment)* ';'
    ;

typedefDef
		: 'typedef' declaration ';'
		;
		
constantDef
    : 'const' IDENTIFIER '=' constant ';'
    ;

typeDef
    : 'typedef' declaration ';'
    | 'enum' IDENTIFIER enumBody ';'
    | 'struct' IDENTIFIER structBody ';'
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

// lexer rules

//COMMENT
//    : '/*' .*? '*/' -> skip
//    ;

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

WS
    : [ \t\r\n]+ -> skip
    ;
