/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
/* Copyright (c) 1983, 1984, 1985, 1986, 1987, 1988, 1989 AT&T */
/* All Rights Reserved */
/*
 * University Copyright- Copyright (c) 1982, 1986, 1988
 * The Regents of the University of California
 * All Rights Reserved
 *
 * University Acknowledgment- Portions of this document are derived from
 * software developed by the University of California, Berkeley, and its
 * contributors.
 */

/*
 * rpc_scan.c, Scanner for the RPC protocol compiler
 */

#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include "rpc_scan.h"
#include "rpc_parse.h"
#include "rpc_util.h"

#define	startcomment(where)	(where[0] == '/' && where[1] == '*')
#define	endcomment(where)	(where[-1] == '*' && where[0] == '/')

static int pushed = 0;	/* is a token pushed */
static token lasttok;	/* last token, if pushed */

static void unget_token(token *);
static void findstrconst(char **, char **);
static void findchrconst(char **, char **);
static void findconst(char **, char **);
static void findkind(char **, token *);
static int cppline(char *);
static int directive(char *);
static void printdirective(char *);
static void docppline(char *, int *, char **);

/*
 * scan expecting 1 given token
 */
void
scan(tok_kind expect, token *tokp)
{
	get_token(tokp);

	if (tokp->kind != expect) {
		expected1(expect);
	}

	return;
}

/*
 * scan expecting any of the 2 given tokens
 */
void
scan2(tok_kind expect1, tok_kind expect2, token *tokp)
{
	get_token(tokp);

	if (tokp->kind != expect1 && tokp->kind != expect2) {
		expected2(expect1, expect2);
	}

	return;
}

/*
 * scan expecting any of the 3 given token
 */
void
scan3(tok_kind expect1, tok_kind expect2, tok_kind expect3, token *tokp)
{
	get_token(tokp);

	if (tokp->kind != expect1
		&& tokp->kind != expect2
		&& tokp->kind != expect3) {

		expected3(expect1, expect2, expect3);
	}

	return;
}

/*
 * scan expecting a constant, possibly symbolic
 */
void
scan_num(token *tokp)
{
	get_token(tokp);

	switch (tokp->kind) {
	case TOK_IDENT:
		break;

	default:
		error("constant or identifier expected");
	}

	return;
}

/*
 * Peek at the next token
 */
void
peek(token *tokp)
{
	get_token(tokp);
	unget_token(tokp);

	return;
}

/*
 * Peek at the next token and scan it if it matches what you expect
 */
int
peekscan(tok_kind expect, token *tokp)
{
	peek(tokp);
	if (tokp->kind == expect) {
		get_token(tokp);
		return (1);
	}

	return (0);
}

/*
 * Get the next token, printing out any directive that are encountered.
 */
void
get_token(token *tokp)
{
	int commenting;
	int stat = 0;

	if (pushed) {
		pushed = 0;
		*tokp = lasttok;
		return;
	}

	commenting = 0;

	for (;;) {
		if (*where == 0) {
			for (;;) {
				if (!fgets(curline, MAXLINESIZE, fin)) {
					tokp->kind = TOK_EOF;

					/*
					 * now check if cpp returned
					 * non NULL value
					 */
					(void) waitpid(childpid, &stat,

								   WUNTRACED);
					if (stat > 0) {
						/* Set return value from rpcgen */
						nonfatalerrors = stat >> 8;
					}
					*where = 0;
					return;
				}

				linenum++;
				if (commenting) {
					break;

				} else if (cppline(curline)) {
					docppline(curline, &linenum,
							  &infilename);

				} else if (directive(curline)) {
					printdirective(curline);

				} else {
					break;
				}
			}
			where = curline;

		} else if (isspace(*where)) {
			while (isspace(*where)) {
				where++;	/* eat */
			}

		} else if (commenting) {
			for (where++; *where; where++) {
				if (endcomment(where)) {
					where++;
					commenting--;
					break;
				}
			}

		} else if (startcomment(where)) {
			where += 2;
			commenting++;

		} else {
			break;
		}
	}

	/*
	 * 'where' is not whitespace, comment or directive Must be a token!
	 */
	switch (*where) {

	case ':':
		tokp->kind = TOK_COLON;
		where++;
		break;

	case ';':
		tokp->kind = TOK_SEMICOLON;
		where++;
		break;

	case ',':
		tokp->kind = TOK_COMMA;
		where++;
		break;

	case '=':
		tokp->kind = TOK_EQUAL;
		where++;
		break;

	case '*':
		tokp->kind = TOK_STAR;
		where++;
		break;

	case '&':
		tokp->kind = TOK_REF;
		where++;
		break;

	case '[':
		tokp->kind = TOK_LBRACKET;
		where++;
		break;

	case ']':
		tokp->kind = TOK_RBRACKET;
		where++;
		break;

	case '{':
		tokp->kind = TOK_LBRACE;
		where++;
		break;

	case '}':
		tokp->kind = TOK_RBRACE;
		where++;
		break;

	case '(':
		tokp->kind = TOK_LPAREN;
		where++;
		break;

	case ')':
		tokp->kind = TOK_RPAREN;
		where++;
		break;

	case '<':
		tokp->kind = TOK_LANGLE;
		where++;
		break;

	case '>':
		tokp->kind = TOK_RANGLE;
		where++;
		break;

	case '"':
		tokp->kind = TOK_STRCONST;
		findstrconst(&where, &tokp->str);
		break;

	case '\'':
		tokp->kind = TOK_CHARCONST;
		findchrconst(&where, &tokp->str);
		break;

	case '-':
		/*FALLTHRU*/
	case '0':
		/*FALLTHRU*/
	case '1':
		/*FALLTHRU*/
	case '2':
		/*FALLTHRU*/
	case '3':
		/*FALLTHRU*/
	case '4':
		/*FALLTHRU*/
	case '5':
		/*FALLTHRU*/
	case '6':
		/*FALLTHRU*/
	case '7':
		/*FALLTHRU*/
	case '8':
		/*FALLTHRU*/
	case '9':
		tokp->kind = TOK_IDENT;
		findconst(&where, &tokp->str);
		break;

	default:
		if (!(isalpha(*where) || *where == '_')) {
			char buf[100];
			char *p;
			size_t blen;

			(void) snprintf(buf, sizeof (buf),
							"illegal character in file: ");
			blen = strlen(buf);
			p = buf + blen;

			if (isprint(*where)) {
				(void) snprintf(p, sizeof (buf) - blen,
								"%c", *where);

			} else {
				(void) snprintf(p, sizeof (buf) - blen,
								"%d", *where);
			}
			error(buf);
		}
		findkind(&where, tokp);
		break;
	}

	return;
}

static void
unget_token(token *tokp)
{
	lasttok = *tokp;
	pushed = 1;

	return;
}

static void
findstrconst(char **str, char **val)
{
	char *p;
	int size;

	p = *str;

	do {
		p++;
	} while (*p && *p != '"');

	if (*p == 0) {
		error("unterminated string constant");
	}

	p++;
	size = p - *str;
	*val = malloc(size + 1);
	(void) strncpy(*val, *str, size);
	(*val)[size] = 0;
	*str = p;

	return;
}

static void
findchrconst(char **str, char **val)
{
	char *p;
	int size;

	p = *str;

	do {
		p++;
	} while (*p && *p != '\'');

	if (*p == 0) {
		error("unterminated string constant");
	}
	
	p++;
	size = p - *str;

	if (size != 3) {
		error("empty char string");
	}
	
	*val = malloc(size + 1);
	(void) strncpy(*val, *str, size);
	(*val)[size] = 0;
	*str = p;

	return;
}

static void
findconst(char **str, char **val)
{
	char *p;
	int size;

	p = *str;

	if (*p == '0' && *(p + 1) == 'x') {
		p++;

		do {
			p++;
		} while (isxdigit(*p));

	} else {

		do {
			p++;

		} while (isdigit(*p));

	}

	size = p - *str;
	*val = malloc(size + 1);
	(void) strncpy(*val, *str, size);
	(*val)[size] = 0;
	*str = p;

	return;
}

static token symbols[] = {
	{TOK_CONST, "const"},
	{TOK_UNION, "union"},
	{TOK_SWITCH, "switch"},
	{TOK_CASE, "case"},
	{TOK_DEFAULT, "default"},
	{TOK_STRUCT, "class"},
	{TOK_STRUCT, "struct"},
	{TOK_TYPEDEF, "typedef"},
	{TOK_ENUM, "enum"},
	{TOK_OPAQUE, "opaque"},
	{TOK_BOOL, "bool"},
	{TOK_VOID, "void"},
	{TOK_ONEWAY, "oneway"},
	{TOK_CHAR, "char"},
	{TOK_INT, "int"},
	{TOK_UNSIGNED, "unsigned"},
	{TOK_SHORT, "short"},
	{TOK_LONG, "long"},
	{TOK_HYPER, "hyper"},
	{TOK_FLOAT, "float"},
	{TOK_DOUBLE, "double"},
	{TOK_QUAD, "quadruple"},
	{TOK_STRING, "string"},
	{TOK_PROGRAM, "program"},
	{TOK_VERSION, "version"},
	{TOK_EOF, "??????"},
};

static void
findkind(char **mark, token *tokp)
{
	int len;
	token *s;
	char *str;

	str = *mark;

	for (s = symbols; s->kind != TOK_EOF; s++) {
		len = strlen(s->str);
		if (strncmp(str, s->str, len) == 0) {
			if (!isalnum(str[len]) && str[len] != '_') {
				tokp->kind = s->kind;
				tokp->str = s->str;
				*mark = str + len;
				return;
			}
		}
	}

	tokp->kind = TOK_IDENT;

	for (len = 0; isalnum(str[len]) || str[len] == '_'; len++)
		/* LOOP */;
	
	tokp->str = malloc(len + 1);
	(void) strncpy(tokp->str, str, len);
	tokp->str[len] = 0;
	*mark = str + len;

	return;
}

static int
cppline(char *line)
{
	return (line == curline && *line == '#');
}

static int
directive(char *line)
{
	return (line == curline && *line == '%');
}

static void
printdirective(char *line)
{
	f_print(fout, "%s", line + 1);

	return;
}

static void
docppline(char *line, int *lineno, char **fname)
{
	char *file;
	int num;
	char *p;

	line++;
	while (isspace(*line)) {
		line++;
	}
	
	num = atoi(line);

	while (isdigit(*line)) {
		line++;
	}
	
	while (isspace(*line)) {
		line++;
	}
	
	if (*line != '"') {
		error("preprocessor error");
	}
	
	line++;
	p = file = malloc(strlen(line) + 1);

	while (*line && *line != '"') {
		*p++ = *line++;
	}
	
	if (*line == 0) {
		error("preprocessor error");
	}
	
	*p = 0;
	if (*file == 0) {
		*fname = NULL;

	} else {
		*fname = file;
	}

	*lineno = num - 1;

	return;
}
