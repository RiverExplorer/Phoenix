grammar xdr;

xdrSpecification
    : specs+
    ; //this is the top level rule for xdr (rfc 4506)

specs
	: structTypeSpec
	| unionTypeSpec
	| enumTypeSpec
	| constantDef
	| namespaceDef
	| comment
	| passThrough
	| program
	| declaration ';'
	;

declaration : typeSpecifier IDENTIFIER
	| typeSpecifier IDENTIFIER '[' value ']'
	| typeSpecifier IDENTIFIER '<' value? '>'
	| 'opaque' IDENTIFIER '[' value ']'
	| 'opaque' IDENTIFIER '<' value? '>'
	| 'string' IDENTIFIER '<' value? '>'
	| typeSpecifier '*' IDENTIFIER
	| 'void'
	;

dataType : typeSpecifier
	| typeSpecifier '[' value ']'
	| typeSpecifier '<' value? '>'
	| 'opaque' '[' value ']'
	| 'opaque' '<' value? '>'
	| 'string' '<' value? '>'
	| dataType '*'
	| 'void'
	;

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
	: 'unsigned'? 'int'
	| 'unsigned'? 'short'
	| 'unsigned'? 'hyper'
	| 'unsigned'? 'quadruple'
	| 'unsigned'? 'char'
	| 'float'
	| 'long'? 'double'
	| 'bool'
	| 'bool_t'
	| 'uint8_t'
	| 'int8_t'
	| 'uint16_t'
	| 'int16_t'
	| 'uint32_t'
	| 'int32_t'
	| 'uint64_t'
	| 'int64_t'
	| enumTypeSpec
	| structTypeSpec
	| unionTypeSpec
	| IDENTIFIER
	;

enumTypeSpec
    : 'enum' enumBody
    ;

enumBody
    : '{' (IDENTIFIER '=' value) (',' IDENTIFIER '=' value)* '}'
    ;

structTypeSpec
    : ('struct'|'class') structBody
    ;

structBody
    : '{'
	((declaration ';') | comment+ | method+ | passThrough+)
	((declaration ';') | comment+ | method+ | passThrough+)*
	'}'
    ;

unionTypeSpec
    : 'union' unionBody
    ;

unionBody
    : 'switch' '(' declaration ')' '{'
	caseSpec
	caseSpec*
	('default' ':' declaration ';')?
	'}'
    ;

caseSpec
    : (('case' value':') | comment+ | passThrough+)
	(('case' value':') | comment+ | passThrough+)*
	declaration ';'
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

method: dataType IDENTIFIER '(' dataType IDENTIFIER? (',' dataType IDENTIFIER?)* ')' ';'
	;

procFirstArg
	: 'void'
	| dataType IDENTIFIER?
	;

program : 'program' IDENTIFIER '{' version+ '}' '=' value ';'
	;

version : 'version' IDENTIFIER '{' versionMethod+ '}' '=' value ';'
	;

versionMethod: dataType IDENTIFIER
 '(' ((dataType IDENTIFIER? (',' dataType IDENTIFIER?)*) | 'void') ')' '=' value ';'
	;							 

// lexer rules

//COMMENT
//    : '/*' .*? '*/' -> skip
//    ;

PASS
	: '%' ~[\n\r]+
	| '%' [\n\r]+
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

