// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "Parser.tab.hpp"


// Unqualified %code blocks.
#line 59 "Parser.yy"

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "Driver.hpp"

#undef yylex
#define yylex _Scanner.yylex

#line 57 "Parser.tab.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "Parser.yy"
namespace RiverExplorer { namespace Phoenix { namespace Protocol {
#line 150 "Parser.tab.cpp"

  /// Build a parser object.
  Parser::Parser (Scanner & _Scanner_yyarg, Driver & _Driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      _Scanner (_Scanner_yyarg),
      _Driver (_Driver_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_uint_default: // uint_default
        value.copy< DefaultValue* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_uint_range: // uint_range
        value.copy< Range* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_range_comparison: // range_comparison
        value.copy< Range::RangeCmp_e > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NDIGITS: // NDIGITS
        value.copy< int64_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FDIGITS: // FDIGITS
        value.copy< long double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDSTRING: // IDSTRING
      case symbol_kind::S_DQSTRING: // DQSTRING
      case symbol_kind::S_SQSTRING: // SQSTRING
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_namespacestring: // namespacestring
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DIGITS: // DIGITS
      case symbol_kind::S_PACKSIZE: // PACKSIZE
      case symbol_kind::S_DEFAULTPACKSIZE: // DEFAULTPACKSIZE
        value.copy< uint64_t > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_uint_default: // uint_default
        value.move< DefaultValue* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_uint_range: // uint_range
        value.move< Range* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_range_comparison: // range_comparison
        value.move< Range::RangeCmp_e > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NDIGITS: // NDIGITS
        value.move< int64_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FDIGITS: // FDIGITS
        value.move< long double > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IDSTRING: // IDSTRING
      case symbol_kind::S_DQSTRING: // DQSTRING
      case symbol_kind::S_SQSTRING: // SQSTRING
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_namespacestring: // namespacestring
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DIGITS: // DIGITS
      case symbol_kind::S_PACKSIZE: // PACKSIZE
      case symbol_kind::S_DEFAULTPACKSIZE: // DEFAULTPACKSIZE
        value.move< uint64_t > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_uint_default: // uint_default
        value.YY_MOVE_OR_COPY< DefaultValue* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_uint_range: // uint_range
        value.YY_MOVE_OR_COPY< Range* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_range_comparison: // range_comparison
        value.YY_MOVE_OR_COPY< Range::RangeCmp_e > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NDIGITS: // NDIGITS
        value.YY_MOVE_OR_COPY< int64_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FDIGITS: // FDIGITS
        value.YY_MOVE_OR_COPY< long double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDSTRING: // IDSTRING
      case symbol_kind::S_DQSTRING: // DQSTRING
      case symbol_kind::S_SQSTRING: // SQSTRING
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_namespacestring: // namespacestring
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DIGITS: // DIGITS
      case symbol_kind::S_PACKSIZE: // PACKSIZE
      case symbol_kind::S_DEFAULTPACKSIZE: // DEFAULTPACKSIZE
        value.YY_MOVE_OR_COPY< uint64_t > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_uint_default: // uint_default
        value.move< DefaultValue* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_uint_range: // uint_range
        value.move< Range* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_range_comparison: // range_comparison
        value.move< Range::RangeCmp_e > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NDIGITS: // NDIGITS
        value.move< int64_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FDIGITS: // FDIGITS
        value.move< long double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDSTRING: // IDSTRING
      case symbol_kind::S_DQSTRING: // DQSTRING
      case symbol_kind::S_SQSTRING: // SQSTRING
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_namespacestring: // namespacestring
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DIGITS: // DIGITS
      case symbol_kind::S_PACKSIZE: // PACKSIZE
      case symbol_kind::S_DEFAULTPACKSIZE: // DEFAULTPACKSIZE
        value.move< uint64_t > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_uint_default: // uint_default
        value.copy< DefaultValue* > (that.value);
        break;

      case symbol_kind::S_uint_range: // uint_range
        value.copy< Range* > (that.value);
        break;

      case symbol_kind::S_range_comparison: // range_comparison
        value.copy< Range::RangeCmp_e > (that.value);
        break;

      case symbol_kind::S_NDIGITS: // NDIGITS
        value.copy< int64_t > (that.value);
        break;

      case symbol_kind::S_FDIGITS: // FDIGITS
        value.copy< long double > (that.value);
        break;

      case symbol_kind::S_IDSTRING: // IDSTRING
      case symbol_kind::S_DQSTRING: // DQSTRING
      case symbol_kind::S_SQSTRING: // SQSTRING
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_namespacestring: // namespacestring
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_DIGITS: // DIGITS
      case symbol_kind::S_PACKSIZE: // PACKSIZE
      case symbol_kind::S_DEFAULTPACKSIZE: // DEFAULTPACKSIZE
        value.copy< uint64_t > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_uint_default: // uint_default
        value.move< DefaultValue* > (that.value);
        break;

      case symbol_kind::S_uint_range: // uint_range
        value.move< Range* > (that.value);
        break;

      case symbol_kind::S_range_comparison: // range_comparison
        value.move< Range::RangeCmp_e > (that.value);
        break;

      case symbol_kind::S_NDIGITS: // NDIGITS
        value.move< int64_t > (that.value);
        break;

      case symbol_kind::S_FDIGITS: // FDIGITS
        value.move< long double > (that.value);
        break;

      case symbol_kind::S_IDSTRING: // IDSTRING
      case symbol_kind::S_DQSTRING: // DQSTRING
      case symbol_kind::S_SQSTRING: // SQSTRING
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_namespacestring: // namespacestring
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_DIGITS: // DIGITS
      case symbol_kind::S_PACKSIZE: // PACKSIZE
      case symbol_kind::S_DEFAULTPACKSIZE: // DEFAULTPACKSIZE
        value.move< uint64_t > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_DEFAULT: // DEFAULT
      case symbol_kind::S_uint_default: // uint_default
        yylhs.value.emplace< DefaultValue* > ();
        break;

      case symbol_kind::S_uint_range: // uint_range
        yylhs.value.emplace< Range* > ();
        break;

      case symbol_kind::S_range_comparison: // range_comparison
        yylhs.value.emplace< Range::RangeCmp_e > ();
        break;

      case symbol_kind::S_NDIGITS: // NDIGITS
        yylhs.value.emplace< int64_t > ();
        break;

      case symbol_kind::S_FDIGITS: // FDIGITS
        yylhs.value.emplace< long double > ();
        break;

      case symbol_kind::S_IDSTRING: // IDSTRING
      case symbol_kind::S_DQSTRING: // DQSTRING
      case symbol_kind::S_SQSTRING: // SQSTRING
      case symbol_kind::S_INCLUDE: // INCLUDE
      case symbol_kind::S_namespacestring: // namespacestring
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_DIGITS: // DIGITS
      case symbol_kind::S_PACKSIZE: // PACKSIZE
      case symbol_kind::S_DEFAULTPACKSIZE: // DEFAULTPACKSIZE
        yylhs.value.emplace< uint64_t > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // protocol: init comments protocoldirectives PROTOCOL IDSTRING LCURLY versions RCURLY SEMI
#line 194 "Parser.yy"
                                                                                          {
				 Global::Log << "Protocol ID: " << yystack_[4].value.as < std::string > ();
						PrintLine();
				 }
#line 888 "Parser.tab.cpp"
    break;

  case 3: // init: %empty
#line 200 "Parser.yy"
              {
		 CurrentScope = CurrentProtocol.GlobalScope;
		 Global::CurrentScope.push_front(CurrentScope);
}
#line 897 "Parser.tab.cpp"
    break;

  case 6: // comment: COMMENT
#line 210 "Parser.yy"
                                {
				/**@todo*/
				}
#line 905 "Parser.tab.cpp"
    break;

  case 7: // gen_cbor: GEN_CBOR
#line 215 "Parser.yy"
                    {
					  Global::Log <<"Generating CBOR Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = true;
						Global::GScope.GenerateCBOR_Server = true;
						}
#line 916 "Parser.tab.cpp"
    break;

  case 8: // no_gen_cbor: NO_GEN_CBOR
#line 223 "Parser.yy"
                           {
					  Global::Log <<"Not generating CBOR Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = false;
						Global::GScope.GenerateCBOR_Server = false;
					 }
#line 927 "Parser.tab.cpp"
    break;

  case 9: // gen_cbor_client: GEN_CBOR_CLIENT
#line 231 "Parser.yy"
                                  {
					  Global::Log <<"Generating CBOR Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = true;
					 }
#line 937 "Parser.tab.cpp"
    break;

  case 10: // no_gen_cbor_client: NO_GEN_CBOR_CLIENT
#line 238 "Parser.yy"
                                        {
					  Global::Log <<"Not generating CBOR Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = false;
					 }
#line 947 "Parser.tab.cpp"
    break;

  case 11: // gen_cbor_server: GEN_CBOR_SERVER
#line 245 "Parser.yy"
                                  {
					  Global::Log <<"Generating CBOR Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Server = true;
					 }
#line 957 "Parser.tab.cpp"
    break;

  case 12: // no_gen_cbor_server: NO_GEN_CBOR_SERVER
#line 252 "Parser.yy"
                                        {
					  Global::Log <<"Not generating CBOR Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Server = false;
					 }
#line 967 "Parser.tab.cpp"
    break;

  case 13: // gen_json: GEN_JSON
#line 259 "Parser.yy"
                    {
						Global::Log << "Generating JSON Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = true;
						Global::GScope.GenerateJSON_Server = true;
					 }
#line 978 "Parser.tab.cpp"
    break;

  case 14: // no_gen_json: NO_GEN_JSON
#line 267 "Parser.yy"
                          {
						Global::Log << "Not generating JSON Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = false;
						Global::GScope.GenerateJSON_Server = false;
					 }
#line 989 "Parser.tab.cpp"
    break;

  case 15: // gen_json_client: GEN_JSON_CLIENT
#line 275 "Parser.yy"
                                 {
						Global::Log << "Generating JSON Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = true;
					 }
#line 999 "Parser.tab.cpp"
    break;

  case 16: // no_gen_json_client: NO_GEN_JSON_CLIENT
#line 282 "Parser.yy"
                                       {
						Global::Log << "Not generating JSON Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = false;
					 }
#line 1009 "Parser.tab.cpp"
    break;

  case 17: // gen_json_server: GEN_JSON_SERVER
#line 289 "Parser.yy"
                                 {
						Global::Log << "Generating JSON Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Server = true;
					 }
#line 1019 "Parser.tab.cpp"
    break;

  case 18: // no_gen_json_server: NO_GEN_JSON_SERVER
#line 296 "Parser.yy"
                                       {
						Global::Log << "Not generating JSON Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Server = false;
					 }
#line 1029 "Parser.tab.cpp"
    break;

  case 19: // gen_native: GEN_NATIVE
#line 303 "Parser.yy"
                        {
					 	Global::Log << "Generating NATIVE Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = true;
						Global::GScope.GenerateNATIVE_Server = true;
					 }
#line 1040 "Parser.tab.cpp"
    break;

  case 20: // no_gen_native: NO_GEN_NATIVE
#line 311 "Parser.yy"
                              {
					 	Global::Log << "Not generating NATIVE Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = false;
						Global::GScope.GenerateNATIVE_Server = false;
					 }
#line 1051 "Parser.tab.cpp"
    break;

  case 21: // gen_native_client: GEN_NATIVE_CLIENT
#line 319 "Parser.yy"
                                     {
					 	Global::Log << "Generating NATIVE Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = true;
					 }
#line 1061 "Parser.tab.cpp"
    break;

  case 22: // no_gen_native_client: NO_GEN_NATIVE_CLIENT
#line 326 "Parser.yy"
                                            {
					 	Global::Log << "Not generating NATIVE Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = false;
					 }
#line 1071 "Parser.tab.cpp"
    break;

  case 23: // gen_native_server: GEN_NATIVE_SERVER
#line 333 "Parser.yy"
                                      {
					 	Global::Log << "Generating NATIVE Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateNATIVE_Server = true;
					 }
#line 1081 "Parser.tab.cpp"
    break;

  case 24: // no_gen_native_server: NO_GEN_NATIVE_SERVER
#line 340 "Parser.yy"
                                            {
						 Global::Log << "Not generating NATIVE Server protocol and code.";
						 PrintLine();
						 Global::GScope.GenerateNATIVE_Server = false;
					 }
#line 1091 "Parser.tab.cpp"
    break;

  case 25: // gen_rest: GEN_REST
#line 347 "Parser.yy"
                    {
						Global::Log << "Global::Generating REST Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = true;
						Global::GScope.GenerateREST_Server = true;
					 }
#line 1102 "Parser.tab.cpp"
    break;

  case 26: // no_gen_rest: NO_GEN_REST
#line 355 "Parser.yy"
                          {
						Global::Log << "Global::Not generating REST Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = false;
						Global::GScope.GenerateREST_Server = false;
					 }
#line 1113 "Parser.tab.cpp"
    break;

  case 27: // gen_rest_client: GEN_REST_CLIENT
#line 363 "Parser.yy"
                                  {
						Global::Log << "Generating REST Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = true;
					 }
#line 1123 "Parser.tab.cpp"
    break;

  case 28: // no_gen_rest_client: NO_GEN_REST_CLIENT
#line 370 "Parser.yy"
                                        {
						Global::Log << "Not generating REST Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = false;
					 }
#line 1133 "Parser.tab.cpp"
    break;

  case 29: // gen_rest_server: GEN_REST_SERVER
#line 377 "Parser.yy"
                                  {
						Global::Log << "Global::Generating REST Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Server = true;
					 }
#line 1143 "Parser.tab.cpp"
    break;

  case 30: // no_gen_rest_server: NO_GEN_REST_SERVER
#line 384 "Parser.yy"
                                        {
						Global::Log << "Global::Not generating REST Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Server = false;
					 }
#line 1153 "Parser.tab.cpp"
    break;

  case 31: // gen_xdr: GEN_XDR
#line 391 "Parser.yy"
                  {
						Global::Log << "Global::Generating XDR Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = true;
						Global::GScope.GenerateXDR_Server = true;
					 }
#line 1164 "Parser.tab.cpp"
    break;

  case 32: // no_gen_xdr: NO_GEN_XDR
#line 399 "Parser.yy"
                        {
						Global::Log << "Global::Not generating XDR Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = false;
						Global::GScope.GenerateXDR_Server = false;
					 }
#line 1175 "Parser.tab.cpp"
    break;

  case 33: // gen_xdr_client: GEN_XDR_CLIENT
#line 407 "Parser.yy"
                                {
						Global::Log << "Generating XDR Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = true;
					 }
#line 1185 "Parser.tab.cpp"
    break;

  case 34: // no_gen_xdr_client: NO_GEN_XDR_CLIENT
#line 414 "Parser.yy"
                                      {
						Global::Log << "Not generating XDR Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = false;
					 }
#line 1195 "Parser.tab.cpp"
    break;

  case 35: // gen_xdr_server: GEN_XDR_SERVER
#line 421 "Parser.yy"
                                {
						Global::Log << "Generating XDR Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Server = true;
					 }
#line 1205 "Parser.tab.cpp"
    break;

  case 36: // no_gen_xdr_server: NO_GEN_XDR_SERVER
#line 428 "Parser.yy"
                                      {
						Global::Log << "Not generating XDR Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Server = false;
					 }
#line 1215 "Parser.tab.cpp"
    break;

  case 37: // gen_xml: GEN_XML
#line 435 "Parser.yy"
                  {
						Global::Log << "Generating XML Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = true;
						Global::GScope.GenerateXML_Server = true;
					 }
#line 1226 "Parser.tab.cpp"
    break;

  case 38: // no_gen_xml: NO_GEN_XML
#line 443 "Parser.yy"
                        {
						Global::Log << "Not generating XML Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = false;
						Global::GScope.GenerateXML_Server = false;
					 }
#line 1237 "Parser.tab.cpp"
    break;

  case 39: // gen_xml_client: GEN_XML_CLIENT
#line 451 "Parser.yy"
                                {
						Global::Log << "Generating XML Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = true;
					 }
#line 1247 "Parser.tab.cpp"
    break;

  case 40: // no_gen_xml_client: NO_GEN_XML_CLIENT
#line 458 "Parser.yy"
                                      {
						Global::Log << "Not generating XML Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = false;
					 }
#line 1257 "Parser.tab.cpp"
    break;

  case 41: // gen_xml_server: GEN_XML_SERVER
#line 465 "Parser.yy"
                                {
						Global::Log << "Generating XML Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Server = true;
					 }
#line 1267 "Parser.tab.cpp"
    break;

  case 42: // no_gen_xml_server: NO_GEN_XML_SERVER
#line 472 "Parser.yy"
                                      {
						Global::Log << "Not generating XML Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Server = false;
					 }
#line 1277 "Parser.tab.cpp"
    break;

  case 43: // packlr: PACKLR
#line 479 "Parser.yy"
                 {
						if (!PackingOrderSet) {
							 Global::GScope.Packing.PackingOrder = Pack::PackLR;
							 PackingOrderSet = true;
							 Global::Log << "Defalt bit Packing Left to Right.";
						} else {
							Global::Log << "Error: Packing order already set at the global level.";
						}
						PrintLine();
					 }
#line 1292 "Parser.tab.cpp"
    break;

  case 44: // packrl: PACKRL
#line 491 "Parser.yy"
                {
						if (!PackingOrderSet) {
							 Global::GScope.Packing.PackingOrder = Pack::PackRL;
							 PackingOrderSet = true;
							 Global::Log << "Defalt bit Packing Right to Left.";
						} else {
							Global::Log << "Error: Packing order already set at the global level.";
						}
						PrintLine();
					 }
#line 1307 "Parser.tab.cpp"
    break;

  case 45: // packnbo: PACKNBO
#line 503 "Parser.yy"
                  {
						if (!PackingOrderSet) {
							 Global::GScope.Packing.PackingOrder = Pack::PackNBO;
							 PackingOrderSet = true;
							 Global::Log << "Defalt bit Packing network byte order.";
						} else {
							Global::Log << "Error: Packing order already set at the global level.";
						}
						PrintLine();
					 }
#line 1322 "Parser.tab.cpp"
    break;

  case 46: // bitstream: BITSTREAM
#line 515 "Parser.yy"
                      {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.BitStream = true;
							 PackingSizeSet = true;
							 Global::Log << "Default Packing set to none - Bit Stream";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
						}
						PrintLine();
					 }
#line 1337 "Parser.tab.cpp"
    break;

  case 47: // defaultpacksize: DEFAULTPACKSIZE COLON DIGITS
#line 528 "Parser.yy"
                                               {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.PackSize = (uint8_t)yystack_[0].value.as < uint64_t > ();
							 PackingSizeSet = true;
							 Global::Log << "Deafult Packing set to " << yystack_[0].value.as < uint64_t > () << " bits";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
						}
						PrintLine();
					 }
#line 1352 "Parser.tab.cpp"
    break;

  case 48: // packsize: PACKSIZE COLON DIGITS
#line 540 "Parser.yy"
                                 {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.PackSize = (uint8_t)yystack_[0].value.as < uint64_t > ();
							 PackingSizeSet = true;
							 Global::Log << "Deafult Packing set to " << yystack_[0].value.as < uint64_t > () << " bits";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
						}
						PrintLine();
					 }
#line 1367 "Parser.tab.cpp"
    break;

  case 49: // gen_cpp_code: GEN_CPP_CODE
#line 552 "Parser.yy"
                            {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCPP = true;
						 Global::Log << "C++ Code generation turned on.";
						PrintLine();
					 }
#line 1379 "Parser.tab.cpp"
    break;

  case 50: // no_gen_cpp_code: NO_GEN_CPP_CODE
#line 561 "Parser.yy"
                                  {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCPP = false;
						 Global::Log << "C++ Code generation turned off.";
						PrintLine();
					 }
#line 1391 "Parser.tab.cpp"
    break;

  case 51: // gen_csharp_code: GEN_CSHARP_CODE
#line 570 "Parser.yy"
                                  {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCSharp = true;
						 Global::Log << "C# Code generation turned on.";
						PrintLine();
					 }
#line 1403 "Parser.tab.cpp"
    break;

  case 52: // no_gen_csharp_code: NO_GEN_CSHARP_CODE
#line 579 "Parser.yy"
                                        {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCSharp = false;
						 Global::Log << "C# Code generation turned off.";
						PrintLine();
					 }
#line 1415 "Parser.tab.cpp"
    break;

  case 53: // document: DOCUMENT
#line 589 "Parser.yy"
                    {
						 Global::Log << "Start Document";
						PrintLine();
					 }
#line 1424 "Parser.tab.cpp"
    break;

  case 54: // no_document: NO_DOCUMENT
#line 595 "Parser.yy"
                          {
						 Global::Log << "No Document";
						PrintLine();
					 }
#line 1433 "Parser.tab.cpp"
    break;

  case 55: // dnamespace: NAMESPACE EQUAL namespacestring
#line 601 "Parser.yy"
                                              {
					 	 Global::Log << "Namespace: " << yystack_[0].value.as < std::string > ();
	 						PrintLine();
						 CurrentScope->Namespace = yystack_[0].value.as < std::string > ();
					 }
#line 1443 "Parser.tab.cpp"
    break;

  case 109: // namespacestring: IDSTRING
#line 668 "Parser.yy"
                           {
									BuildingNS = yystack_[0].value.as < std::string > ();
									yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
              	}
#line 1452 "Parser.tab.cpp"
    break;

  case 110: // namespacestring: namespacestring COLON IDSTRING
#line 672 "Parser.yy"
                                                                                                       {
									BuildingNS = yystack_[2].value.as < std::string > () + ":" + yystack_[0].value.as < std::string > ();
									yylhs.value.as < std::string > () = BuildingNS;
								}
#line 1461 "Parser.tab.cpp"
    break;

  case 113: // version: VERSION IDSTRING LCURLY contents RCURLY SEMI
#line 683 "Parser.yy"
                                {
					Global::Log << "Protocol Version: " << yystack_[4].value.as < std::string > ();
						PrintLine();
				}
#line 1470 "Parser.tab.cpp"
    break;

  case 133: // range_comparison: LESS_THAN
#line 722 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::LessThan_t;
								 }
#line 1478 "Parser.tab.cpp"
    break;

  case 134: // range_comparison: LESS_THAN_OR_EQUAL
#line 727 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::LessThanOrEqualTo_t;
								 }
#line 1486 "Parser.tab.cpp"
    break;

  case 135: // range_comparison: GREATER_THAN_OR_EQUAL
#line 732 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::GreaterThanOrEqualTo_t;
								 }
#line 1494 "Parser.tab.cpp"
    break;

  case 136: // range_comparison: GREATER_THAN
#line 737 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::GreaterThan_t;
								 }
#line 1502 "Parser.tab.cpp"
    break;

  case 140: // uint_default: LCURLY DEFAULT EQUAL DIGITS RCURLY
#line 748 "Parser.yy"
                                                 {
							CurrentDefaultValue = new DefaultValueT<uint64_t>(yystack_[1].value.as < uint64_t > ());
							yylhs.value.as < DefaultValue* > () = CurrentDefaultValue;
						 }
#line 1511 "Parser.tab.cpp"
    break;

  case 141: // uint_default: LCURLY DEFAULT EQUAL IDSTRING RCURLY
#line 754 "Parser.yy"
                                                 {
							CurrentDefaultValue = new DefaultValueT<std::string>(yystack_[1].value.as < std::string > ());
							yylhs.value.as < DefaultValue* > () = CurrentDefaultValue;
						 }
#line 1520 "Parser.tab.cpp"
    break;

  case 142: // uint_range: LCURLY RANGE COLON range_comparison DIGITS COMMA range_comparison DIGITS RCURLY
#line 761 "Parser.yy"
                                         {
							CurrentRange = new RangeT<uint64_t>(yystack_[5].value.as < Range::RangeCmp_e > (), yystack_[4].value.as < uint64_t > (), yystack_[2].value.as < Range::RangeCmp_e > (), yystack_[1].value.as < uint64_t > ());
							yylhs.value.as < Range* > () = CurrentRange;
					 }
#line 1529 "Parser.tab.cpp"
    break;

  case 146: // uintsingle: UINT IDSTRING opt_uint
#line 773 "Parser.yy"
           {
						Global::Log << "UNSIGNED INTEGER: " << yystack_[1].value.as < std::string > ();
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, yystack_[1].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->Default = CurrentDefaultValue;
						CurrentDefaultValue = nullptr;

						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1547 "Parser.tab.cpp"
    break;

  case 147: // uintsingle: UINT COLON DIGITS IDSTRING
#line 788 "Parser.yy"
           {
						Global::Log << "UNSIGNED " << yystack_[1].value.as < uint64_t > ()
						     << "-BIT INTEGER: "
								 << yystack_[0].value.as < std::string > ();
						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, yystack_[1].value.as < uint64_t > (), yystack_[0].value.as < std::string > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1561 "Parser.tab.cpp"
    break;

  case 148: // uintfixedarray: UINT IDSTRING LSQUARE DIGITS RSQUARE
#line 800 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " UNSIGNED INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1575 "Parser.tab.cpp"
    break;

  case 149: // uintfixedarray: UINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
#line 811 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " UNSIGNED " << yystack_[4].value.as < uint64_t > ()
								 << "-BIT INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1590 "Parser.tab.cpp"
    break;

  case 150: // uintvariablearray: UINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 824 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " UNSIGNED INTEGERS: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1605 "Parser.tab.cpp"
    break;

  case 151: // uintvariablearray: UINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 836 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > () << " "
								 << yystack_[6].value.as < uint64_t > () << "-BIT"
						     << " UNSIGNED INTEGERS: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, yystack_[6].value.as < uint64_t > (), yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1621 "Parser.tab.cpp"
    break;

  case 152: // uintvariablearray: UINT IDSTRING LESS_THAN GREATER_THAN
#line 849 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more UNSIGNED INTEGERS: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1634 "Parser.tab.cpp"
    break;

  case 153: // uintvariablearray: UINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
#line 859 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[3].value.as < uint64_t > () << "-BIT UNSIGNED INTEGERS: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::uint_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1648 "Parser.tab.cpp"
    break;

  case 157: // sintsingle: SINT IDSTRING
#line 876 "Parser.yy"
           {
						Global::Log << "SIGNED INTEGER: " << yystack_[0].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[0].value.as < std::string > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1660 "Parser.tab.cpp"
    break;

  case 158: // sintsingle: SINT COLON DIGITS IDSTRING
#line 885 "Parser.yy"
           {
						Global::Log << "SIGNED " << yystack_[1].value.as < uint64_t > ()
						     << "-BIT INTEGER: "
								 << yystack_[0].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[1].value.as < uint64_t > (), yystack_[0].value.as < std::string > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1674 "Parser.tab.cpp"
    break;

  case 159: // sintfixedarray: SINT IDSTRING LSQUARE DIGITS RSQUARE
#line 897 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " SIGNED INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1688 "Parser.tab.cpp"
    break;

  case 160: // sintfixedarray: SINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
#line 908 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " SIGNED " << yystack_[4].value.as < uint64_t > ()
								 << "-BIT INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1703 "Parser.tab.cpp"
    break;

  case 161: // sintvariablearray: SINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 921 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " SIGNED INTEGERS: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1718 "Parser.tab.cpp"
    break;

  case 162: // sintvariablearray: SINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 933 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > () << " "
								 << yystack_[6].value.as < uint64_t > () << "-BIT"
						     << " SIGNED INTEGERS: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[6].value.as < uint64_t > (), yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1734 "Parser.tab.cpp"
    break;

  case 163: // sintvariablearray: SINT IDSTRING LESS_THAN GREATER_THAN
#line 946 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more SIGNED INTEGERS: " << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1746 "Parser.tab.cpp"
    break;

  case 164: // sintvariablearray: SINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
#line 955 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[3].value.as < uint64_t > () << "-BIT SIGNED INTEGERS: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::sint_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1760 "Parser.tab.cpp"
    break;

  case 167: // opaquefixedarray: OPAQUE IDSTRING LSQUARE DIGITS RSQUARE
#line 971 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " OPAQUE: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::opaque_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1774 "Parser.tab.cpp"
    break;

  case 168: // opaquevariablearray: OPAQUE IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 983 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " OPAQUE: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::opaque_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1789 "Parser.tab.cpp"
    break;

  case 169: // opaquevariablearray: OPAQUE IDSTRING LESS_THAN GREATER_THAN
#line 995 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more OPAQUE: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::opaque_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1802 "Parser.tab.cpp"
    break;

  case 173: // floatsingle: FLOAT IDSTRING
#line 1012 "Parser.yy"
           {
						Global::Log << "FLOAT: " << yystack_[0].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[0].value.as < std::string > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1814 "Parser.tab.cpp"
    break;

  case 174: // floatsingle: FLOAT COLON DIGITS IDSTRING
#line 1021 "Parser.yy"
           {
						Global::Log << yystack_[1].value.as < uint64_t > ()
						     << "-BIT FLOAT: "
								 << yystack_[0].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[1].value.as < uint64_t > (), yystack_[0].value.as < std::string > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1828 "Parser.tab.cpp"
    break;

  case 175: // floatfixedarray: FLOAT IDSTRING LSQUARE DIGITS RSQUARE
#line 1033 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " FLOAT: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1842 "Parser.tab.cpp"
    break;

  case 176: // floatfixedarray: FLOAT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
#line 1044 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << yystack_[4].value.as < uint64_t > ()
								 << "-BIT FLOAT: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1857 "Parser.tab.cpp"
    break;

  case 177: // floatvariablearray: FLOAT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1057 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " FLOAT: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1872 "Parser.tab.cpp"
    break;

  case 178: // floatvariablearray: FLOAT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1069 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > () << " "
								 << yystack_[6].value.as < uint64_t > () << "-BIT"
						     << " FLOAT: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[6].value.as < uint64_t > (), yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1888 "Parser.tab.cpp"
    break;

  case 179: // floatvariablearray: FLOAT IDSTRING LESS_THAN GREATER_THAN
#line 1082 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more FLOAT: " << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1900 "Parser.tab.cpp"
    break;

  case 180: // floatvariablearray: FLOAT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
#line 1091 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[3].value.as < uint64_t > () << "-BIT FLOAT: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::float_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1914 "Parser.tab.cpp"
    break;

  case 184: // stringsingle: STRING IDSTRING
#line 1108 "Parser.yy"
           {
						Global::Log << "STRING: " << yystack_[0].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[0].value.as < std::string > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1926 "Parser.tab.cpp"
    break;

  case 185: // stringsingle: STRING COLON DIGITS IDSTRING
#line 1117 "Parser.yy"
           {
						Global::Log << yystack_[1].value.as < uint64_t > ()
						     << "-BIT STRING: "
								 << yystack_[0].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[1].value.as < uint64_t > (), yystack_[0].value.as < std::string > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1940 "Parser.tab.cpp"
    break;

  case 186: // stringfixedarray: STRING IDSTRING LSQUARE DIGITS RSQUARE
#line 1129 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " STRING: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1954 "Parser.tab.cpp"
    break;

  case 187: // stringfixedarray: STRING COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
#line 1140 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << yystack_[4].value.as < uint64_t > ()
								 << "-BIT STRING: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1969 "Parser.tab.cpp"
    break;

  case 188: // stringvariablearray: STRING IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1153 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " STRING: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 1984 "Parser.tab.cpp"
    break;

  case 189: // stringvariablearray: STRING COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1165 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > () << " "
								 << yystack_[6].value.as < uint64_t > () << "-BIT"
						     << " STRING: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[6].value.as < uint64_t > (), yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 2000 "Parser.tab.cpp"
    break;

  case 190: // stringvariablearray: STRING IDSTRING LESS_THAN GREATER_THAN
#line 1178 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more STRING: " << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 2012 "Parser.tab.cpp"
    break;

  case 191: // stringvariablearray: STRING COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
#line 1187 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[3].value.as < uint64_t > () << "-BIT STRING: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						Symbol * NewOne = new Symbol( Symbol::string_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						NewOne->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(NewOne, LineNumber);
					 }
#line 2026 "Parser.tab.cpp"
    break;

  case 192: // methodsig: mreturn IDSTRING LPAREN parameters RPAREN
#line 1199 "Parser.yy"
                                        {
						Global::Log << "Method: " << yystack_[3].value.as < std::string > ()
						     << " Returning: " << yystack_[3].value.as < std::string > (); /**@todo*/
 						PrintLine();
					}
#line 2036 "Parser.tab.cpp"
    break;


#line 2040 "Parser.tab.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // Parser::context.
  Parser::context::context (const Parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  Parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  Parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short Parser::yypact_ninf_ = -144;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
    -144,     6,  -144,  -144,    47,  -144,   -35,  -144,    45,  -144,
    -144,  -144,  -144,    42,    48,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,   115,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,    90,  -144,   -35,
     129,   163,   170,   133,  -144,   -35,   157,   165,  -144,  -144,
    -144,   158,  -144,  -144,   151,   172,   173,   120,  -144,  -144,
     167,   164,  -144,    50,  -144,   175,    86,   176,   127,   128,
     130,  -144,   177,   178,    44,  -144,   168,   169,   171,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,   174,   182,
    -144,   135,   186,   183,   136,   188,   137,   190,   138,   191,
     139,   179,  -144,  -144,  -144,  -144,  -144,   185,   194,    63,
     192,    65,   196,    64,   193,   195,   197,    99,   198,   200,
     105,   199,   203,   114,  -144,    84,   201,   202,  -144,   140,
    -144,    79,  -144,  -144,  -144,   204,   205,  -144,   113,  -144,
    -144,   141,   207,   208,  -144,   149,   210,   211,  -144,   150,
     213,   214,  -144,   175,  -144,  -144,    94,  -144,  -144,   206,
     209,   116,  -144,  -144,  -144,   216,   189,   215,   219,   117,
    -144,   222,   225,   118,  -144,   227,   228,   119,  -144,   230,
    -144,  -144,   184,   220,   221,  -144,   217,    58,   144,   223,
     224,  -144,   218,   226,   229,  -144,   231,   232,   233,  -144,
     234,  -144,  -144,   235,  -144,   237,   239,  -144,  -144,  -144,
    -144,   240,  -144,   242,  -144,  -144,   247,  -144,  -144,   248,
    -144,   236,  -144,  -144,   238,   241,   243,   244,  -144,   144,
    -144,  -144,  -144,   249,   246,  -144
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       3,     0,     4,     1,     0,     6,     0,     5,     0,    45,
      43,    44,    46,     0,     0,     7,     9,    11,     8,    10,
      12,    13,    15,    17,    14,    16,    18,    19,    21,    23,
      20,    22,    24,    25,    27,    29,    26,    28,    30,    31,
      33,    35,    32,    34,    36,    37,    39,    41,    38,    40,
      42,     0,    49,    50,    51,    52,    53,    54,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,     0,    58,     0,
       0,     0,     0,     0,    56,     0,     0,     0,    48,    47,
     109,    55,    59,    57,     0,     0,     0,     0,   111,   110,
       0,     0,   112,     4,     2,     0,     0,     0,     0,     0,
       0,   194,     0,   120,     4,   114,     0,     0,     0,   121,
     137,   138,   139,   122,   154,   155,   156,   125,   165,   166,
     123,   170,   171,   172,   124,   181,   182,   183,     0,     0,
     132,   184,     0,     0,   143,     0,   157,     0,   173,     0,
       0,     0,   115,   116,   117,   118,   119,     0,     0,     0,
       0,     0,     0,     0,   146,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,   195,     0,     0,   190,   185,
     129,     0,   127,   130,   131,     0,     0,   152,     0,   144,
     145,   147,     0,     0,   163,   158,     0,     0,   179,   174,
       0,     0,   169,     0,   198,   199,     0,   196,   186,     0,
       0,     0,   126,   128,   148,     0,     0,     0,     0,     0,
     159,     0,     0,     0,   175,     0,     0,     0,   167,     0,
     192,   197,     0,     0,     0,   191,     0,     0,     0,     0,
       0,   153,     0,     0,     0,   164,     0,     0,     0,   180,
       0,   188,   187,     0,   150,     0,     0,   133,   134,   135,
     136,     0,   149,     0,   161,   160,     0,   177,   176,     0,
     168,     0,   140,   141,     0,     0,     0,     0,   189,     0,
     151,   162,   178,     0,     0,   142
  };

  const short
  Parser::yypgoto_[] =
  {
    -144,  -144,  -144,   212,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,    98,    95,  -144,  -144,    89,
    -144,    69,  -143,  -144,  -144,    24,  -142,   -55,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,  -144,
     -84,  -144,  -144,    21
  };

  const short
  Parser::yydefgoto_[] =
  {
       0,     1,     2,   143,     7,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     8,   107,   108,   121,   127,   128,
     144,   145,   146,   147,   211,   212,   148,   291,   149,   219,
     220,   194,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   236,   237
  };

  const short
  Parser::yytable_[] =
  {
       9,    10,    11,    12,    13,    14,     3,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   210,   213,
     135,   136,    56,    57,   181,     5,   135,   136,   109,   111,
       6,   285,   234,   235,   286,   112,   207,   216,   210,   213,
     110,   135,   136,   137,   138,   139,   140,   141,   142,   137,
     138,   139,   140,   141,   142,   135,   136,   208,   217,   242,
     233,   136,   171,   234,   235,   138,   139,   140,   141,   142,
     233,   136,   223,   172,   114,   115,   260,   214,   227,   138,
     139,   140,   141,   142,   138,   139,   140,   231,   142,   264,
     270,   274,   278,   224,   138,   139,   140,   214,   142,   228,
     131,   246,   247,   174,   176,   117,   178,   113,   232,   120,
     265,   271,   275,   279,   175,   177,   126,   179,   188,   192,
     196,   199,   202,   240,   248,   189,   193,   197,   200,   203,
     241,   249,   252,   256,   287,   288,   118,   289,   290,   253,
     257,   123,   115,   119,   124,   125,   133,   126,   129,   130,
     134,   170,   173,   180,   183,   184,     5,   185,   187,   190,
     186,   195,   191,   198,   201,   204,   205,   206,   209,   215,
     222,   221,   218,   226,   225,   229,   230,   116,   281,   262,
     122,   267,   263,   182,     4,   238,   132,   239,   244,   266,
     245,   250,   269,   251,   254,   272,   255,   258,   273,   259,
     276,   277,   268,   280,   282,   243,   283,   292,   301,   293,
     295,   284,   294,   304,   296,   305,   298,   302,   299,   303,
     306,   307,   314,   309,   313,   297,   315,   261,   300,     0,
     308,     0,     0,     0,     0,   310,     0,   311,   312
  };

  const short
  Parser::yycheck_[] =
  {
      35,    36,    37,    38,    39,    40,     0,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,   191,   191,
       6,     7,    87,    88,    10,     8,     6,     7,    13,    17,
      13,     3,   205,   205,     6,    17,     3,     3,   211,   211,
      25,     6,     7,    29,    30,    31,    32,    33,    34,    29,
      30,    31,    32,    33,    34,     6,     7,    24,    24,    10,
       6,     7,     6,   236,   236,    30,    31,    32,    33,    34,
       6,     7,     3,    17,    14,    15,    12,   191,     3,    30,
      31,    32,    33,    34,    30,    31,    32,     3,    34,     3,
       3,     3,     3,    24,    30,    31,    32,   211,    34,    24,
      10,    18,    19,     6,     6,     6,     6,    22,    24,     6,
      24,    24,    24,    24,    17,    17,    26,    17,    13,    13,
      13,    13,    13,    13,    13,    20,    20,    20,    20,    20,
      20,    20,    13,    13,    20,    21,     3,    23,    24,    20,
      20,    14,    15,     3,     9,    17,     9,    26,     6,     6,
      16,     6,     6,     6,    16,    16,     8,    16,     6,     3,
      16,     3,     9,     3,     3,    16,    11,     3,     6,     3,
       3,     6,     9,     3,     6,     6,     3,   109,    24,     3,
     115,    22,     3,   144,     2,    14,   127,    15,    14,     3,
      15,    14,     3,    15,    14,     3,    15,    14,     3,    15,
       3,     3,    17,     3,    14,   211,    15,    14,     3,    15,
      14,    24,    24,     3,    15,     3,    14,    10,    15,    10,
       3,     3,     3,    15,   309,    24,    10,   236,    24,    -1,
      24,    -1,    -1,    -1,    -1,    24,    -1,    24,    24
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,    90,    91,     0,    92,     8,    13,    93,   143,    35,
      36,    37,    38,    39,    40,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    87,    88,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   144,   145,    13,
      25,    17,    17,    22,    14,    15,   144,     6,     3,     3,
       6,   146,   145,    14,     9,    17,    26,   147,   148,     6,
       6,    10,   148,     9,    16,     6,     7,    29,    30,    31,
      32,    33,    34,    92,   149,   150,   151,   152,   155,   157,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
       6,     6,    17,     6,     6,    17,     6,    17,     6,    17,
       6,    10,   150,    16,    16,    16,    16,     6,    13,    20,
       3,     9,    13,    20,   160,     3,    13,    20,     3,    13,
      20,     3,    13,    20,    16,    11,     3,     3,    24,     6,
     151,   153,   154,   155,   179,     3,     3,    24,     9,   158,
     159,     6,     3,     3,    24,     6,     3,     3,    24,     6,
       3,     3,    24,     6,   151,   155,   181,   182,    14,    15,
      13,    20,    10,   154,    14,    15,    18,    19,    13,    20,
      14,    15,    13,    20,    14,    15,    13,    20,    14,    15,
      12,   182,     3,     3,     3,    24,     3,    22,    17,     3,
       3,    24,     3,     3,     3,    24,     3,     3,     3,    24,
       3,    24,    14,    15,    24,     3,     6,    20,    21,    23,
      24,   156,    14,    15,    24,    14,    15,    24,    14,    15,
      24,     3,    10,    10,     3,     3,     3,     3,    24,    15,
      24,    24,    24,   156,     3,    10
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    89,    90,    91,    92,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   143,   144,   144,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   146,
     146,   147,   147,   148,   149,   149,   150,   150,   150,   150,
     150,   151,   151,   151,   151,   151,   152,   153,   153,   154,
     154,   154,   155,   156,   156,   156,   156,   157,   157,   157,
     158,   158,   159,   160,   160,   160,   161,   161,   162,   162,
     163,   163,   163,   163,   164,   164,   164,   165,   165,   166,
     166,   167,   167,   167,   167,   168,   168,   169,   170,   170,
     171,   171,   171,   172,   172,   173,   173,   174,   174,   174,
     174,   175,   175,   175,   176,   176,   177,   177,   178,   178,
     178,   178,   179,   180,   180,   181,   181,   181,   182,   182
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     9,     0,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     2,     6,     1,     2,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     5,     1,     2,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       5,     5,     9,     0,     2,     2,     3,     4,     5,     7,
       7,     9,     4,     6,     1,     1,     1,     2,     4,     5,
       7,     7,     9,     4,     6,     1,     1,     5,     7,     4,
       1,     1,     1,     2,     4,     5,     7,     7,     9,     4,
       6,     1,     1,     1,     2,     4,     5,     7,     7,     9,
       4,     6,     5,     1,     1,     0,     1,     2,     1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "DIGITS", "NDIGITS",
  "FDIGITS", "IDSTRING", "STRING", "COMMENT", "LCURLY", "RCURLY", "LPAREN",
  "RPAREN", "LSQUARE", "RSQUARE", "COMMA", "SEMI", "COLON", "DEFAULT",
  "RANGE", "LESS_THAN", "LESS_THAN_OR_EQUAL", "EQUAL",
  "GREATER_THAN_OR_EQUAL", "GREATER_THAN", "PROTOCOL", "VERSION",
  "DQSTRING", "SQSTRING", "CLASS", "UINT", "SINT", "FLOAT", "VOID",
  "OPAQUE", "PACKNBO", "PACKLR", "PACKRL", "BITSTREAM", "PACKSIZE",
  "DEFAULTPACKSIZE", "INCLUDE", "GEN_CBOR", "GEN_CBOR_CLIENT",
  "GEN_CBOR_SERVER", "NO_GEN_CBOR", "NO_GEN_CBOR_CLIENT",
  "NO_GEN_CBOR_SERVER", "GEN_JSON", "GEN_JSON_CLIENT", "GEN_JSON_SERVER",
  "NO_GEN_JSON", "NO_GEN_JSON_CLIENT", "NO_GEN_JSON_SERVER", "GEN_NATIVE",
  "GEN_NATIVE_CLIENT", "GEN_NATIVE_SERVER", "NO_GEN_NATIVE",
  "NO_GEN_NATIVE_CLIENT", "NO_GEN_NATIVE_SERVER", "GEN_REST",
  "GEN_REST_CLIENT", "GEN_REST_SERVER", "NO_GEN_REST",
  "NO_GEN_REST_CLIENT", "NO_GEN_REST_SERVER", "GEN_XDR", "GEN_XDR_CLIENT",
  "GEN_XDR_SERVER", "NO_GEN_XDR", "NO_GEN_XDR_CLIENT", "NO_GEN_XDR_SERVER",
  "GEN_XML", "GEN_XML_CLIENT", "GEN_XML_SERVER", "NO_GEN_XML",
  "NO_GEN_XML_CLIENT", "NO_GEN_XML_SERVER", "NAMESPACE", "GEN_CPP_CODE",
  "NO_GEN_CPP_CODE", "GEN_CSHARP_CODE", "NO_GEN_CSHARP_CODE",
  "VISIBILITY_PUBLIC", "VISIBILITY_PROTECTED", "VISIBILITY_INTERNAL",
  "VISIBILITY_PRIVATE", "DOCUMENT", "NO_DOCUMENT", "$accept", "protocol",
  "init", "comments", "comment", "gen_cbor", "no_gen_cbor",
  "gen_cbor_client", "no_gen_cbor_client", "gen_cbor_server",
  "no_gen_cbor_server", "gen_json", "no_gen_json", "gen_json_client",
  "no_gen_json_client", "gen_json_server", "no_gen_json_server",
  "gen_native", "no_gen_native", "gen_native_client",
  "no_gen_native_client", "gen_native_server", "no_gen_native_server",
  "gen_rest", "no_gen_rest", "gen_rest_client", "no_gen_rest_client",
  "gen_rest_server", "no_gen_rest_server", "gen_xdr", "no_gen_xdr",
  "gen_xdr_client", "no_gen_xdr_client", "gen_xdr_server",
  "no_gen_xdr_server", "gen_xml", "no_gen_xml", "gen_xml_client",
  "no_gen_xml_client", "gen_xml_server", "no_gen_xml_server", "packlr",
  "packrl", "packnbo", "bitstream", "defaultpacksize", "packsize",
  "gen_cpp_code", "no_gen_cpp_code", "gen_csharp_code",
  "no_gen_csharp_code", "document", "no_document", "dnamespace",
  "protocoldirectives", "pdirectives", "directive", "namespacestring",
  "versions", "version", "contents", "content", "simpletype", "classdef",
  "classcontents", "classcontent", "classtype", "range_comparison",
  "uinttype", "uint_default", "uint_range", "opt_uint", "uintsingle",
  "uintfixedarray", "uintvariablearray", "sinttype", "sintsingle",
  "sintfixedarray", "sintvariablearray", "opaquetype", "opaquefixedarray",
  "opaquevariablearray", "floattype", "floatsingle", "floatfixedarray",
  "floatvariablearray", "stringtype", "stringsingle", "stringfixedarray",
  "stringvariablearray", "methodsig", "mreturn", "parameters", "paramtype", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   194,   194,   200,   205,   206,   209,   215,   223,   231,
     238,   245,   252,   259,   267,   275,   282,   289,   296,   303,
     311,   319,   326,   333,   340,   347,   355,   363,   370,   377,
     384,   391,   399,   407,   414,   421,   428,   435,   443,   451,
     458,   465,   472,   479,   491,   503,   515,   528,   540,   552,
     561,   570,   579,   589,   595,   601,   608,   609,   613,   614,
     617,   618,   619,   620,   621,   622,   623,   624,   625,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,   651,   652,   653,   654,   655,   656,
     657,   658,   659,   660,   661,   662,   663,   664,   665,   668,
     672,   678,   679,   682,   689,   690,   693,   694,   695,   696,
     697,   700,   701,   702,   703,   704,   707,   709,   710,   713,
     714,   715,   718,   721,   726,   731,   736,   742,   743,   744,
     747,   753,   760,   767,   768,   769,   772,   787,   799,   810,
     823,   835,   848,   858,   870,   871,   872,   875,   884,   896,
     907,   920,   932,   945,   954,   966,   967,   970,   982,   994,
    1006,  1007,  1008,  1011,  1020,  1032,  1043,  1056,  1068,  1081,
    1090,  1102,  1103,  1104,  1107,  1116,  1128,  1139,  1152,  1164,
    1177,  1186,  1198,  1206,  1207,  1210,  1211,  1212,  1215,  1216
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88
    };
    // Last valid token kind.
    const int code_max = 343;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 5 "Parser.yy"
} } } // RiverExplorer::Phoenix::Protocol
#line 2817 "Parser.tab.cpp"

#line 1219 "Parser.yy"

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
