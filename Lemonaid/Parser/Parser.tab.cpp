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
#line 61 "Parser.yy"

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
      case symbol_kind::S_uint_default: // uint_default
        value.copy< DefaultValueT<uint64_t>* > (YY_MOVE (that.value));
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
      case symbol_kind::S_uint_default: // uint_default
        value.move< DefaultValueT<uint64_t>* > (YY_MOVE (s.value));
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
      case symbol_kind::S_uint_default: // uint_default
        value.YY_MOVE_OR_COPY< DefaultValueT<uint64_t>* > (YY_MOVE (that.value));
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
      case symbol_kind::S_uint_default: // uint_default
        value.move< DefaultValueT<uint64_t>* > (YY_MOVE (that.value));
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
      case symbol_kind::S_uint_default: // uint_default
        value.copy< DefaultValueT<uint64_t>* > (that.value);
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
      case symbol_kind::S_uint_default: // uint_default
        value.move< DefaultValueT<uint64_t>* > (that.value);
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
      case symbol_kind::S_uint_default: // uint_default
        yylhs.value.emplace< DefaultValueT<uint64_t>* > ();
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
#line 203 "Parser.yy"
                                                                                          {
				 Global::Log << "Protocol ID: " << yystack_[4].value.as < std::string > ();
						PrintLine();
				 }
#line 881 "Parser.tab.cpp"
    break;

  case 3: // init: %empty
#line 209 "Parser.yy"
              {
		 CurrentScope = CurrentProtocol.GlobalScope;
		 Global::CurrentScope.push_front(CurrentScope);
}
#line 890 "Parser.tab.cpp"
    break;

  case 6: // comment: COMMENT
#line 219 "Parser.yy"
                                {
				/**@todo*/
				}
#line 898 "Parser.tab.cpp"
    break;

  case 7: // gen_cbor: GEN_CBOR
#line 224 "Parser.yy"
                    {
					  Global::Log <<"Generating CBOR Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = true;
						Global::GScope.GenerateCBOR_Server = true;
						}
#line 909 "Parser.tab.cpp"
    break;

  case 8: // no_gen_cbor: NO_GEN_CBOR
#line 232 "Parser.yy"
                           {
					  Global::Log <<"Not generating CBOR Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = false;
						Global::GScope.GenerateCBOR_Server = false;
					 }
#line 920 "Parser.tab.cpp"
    break;

  case 9: // gen_cbor_client: GEN_CBOR_CLIENT
#line 240 "Parser.yy"
                                  {
					  Global::Log <<"Generating CBOR Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = true;
					 }
#line 930 "Parser.tab.cpp"
    break;

  case 10: // no_gen_cbor_client: NO_GEN_CBOR_CLIENT
#line 247 "Parser.yy"
                                        {
					  Global::Log <<"Not generating CBOR Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Client = false;
					 }
#line 940 "Parser.tab.cpp"
    break;

  case 11: // gen_cbor_server: GEN_CBOR_SERVER
#line 254 "Parser.yy"
                                  {
					  Global::Log <<"Generating CBOR Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Server = true;
					 }
#line 950 "Parser.tab.cpp"
    break;

  case 12: // no_gen_cbor_server: NO_GEN_CBOR_SERVER
#line 261 "Parser.yy"
                                        {
					  Global::Log <<"Not generating CBOR Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateCBOR_Server = false;
					 }
#line 960 "Parser.tab.cpp"
    break;

  case 13: // gen_json: GEN_JSON
#line 268 "Parser.yy"
                    {
						Global::Log << "Generating JSON Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = true;
						Global::GScope.GenerateJSON_Server = true;
					 }
#line 971 "Parser.tab.cpp"
    break;

  case 14: // no_gen_json: NO_GEN_JSON
#line 276 "Parser.yy"
                          {
						Global::Log << "Not generating JSON Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = false;
						Global::GScope.GenerateJSON_Server = false;
					 }
#line 982 "Parser.tab.cpp"
    break;

  case 15: // gen_json_client: GEN_JSON_CLIENT
#line 284 "Parser.yy"
                                 {
						Global::Log << "Generating JSON Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = true;
					 }
#line 992 "Parser.tab.cpp"
    break;

  case 16: // no_gen_json_client: NO_GEN_JSON_CLIENT
#line 291 "Parser.yy"
                                       {
						Global::Log << "Not generating JSON Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Client = false;
					 }
#line 1002 "Parser.tab.cpp"
    break;

  case 17: // gen_json_server: GEN_JSON_SERVER
#line 298 "Parser.yy"
                                 {
						Global::Log << "Generating JSON Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Server = true;
					 }
#line 1012 "Parser.tab.cpp"
    break;

  case 18: // no_gen_json_server: NO_GEN_JSON_SERVER
#line 305 "Parser.yy"
                                       {
						Global::Log << "Not generating JSON Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateJSON_Server = false;
					 }
#line 1022 "Parser.tab.cpp"
    break;

  case 19: // gen_native: GEN_NATIVE
#line 312 "Parser.yy"
                        {
					 	Global::Log << "Generating NATIVE Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = true;
						Global::GScope.GenerateNATIVE_Server = true;
					 }
#line 1033 "Parser.tab.cpp"
    break;

  case 20: // no_gen_native: NO_GEN_NATIVE
#line 320 "Parser.yy"
                              {
					 	Global::Log << "Not generating NATIVE Client and Server protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = false;
						Global::GScope.GenerateNATIVE_Server = false;
					 }
#line 1044 "Parser.tab.cpp"
    break;

  case 21: // gen_native_client: GEN_NATIVE_CLIENT
#line 328 "Parser.yy"
                                     {
					 	Global::Log << "Generating NATIVE Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = true;
					 }
#line 1054 "Parser.tab.cpp"
    break;

  case 22: // no_gen_native_client: NO_GEN_NATIVE_CLIENT
#line 335 "Parser.yy"
                                            {
					 	Global::Log << "Not generating NATIVE Client protocol and code.";
						PrintLine();
						Global::GScope.GenerateNATIVE_Client = false;
					 }
#line 1064 "Parser.tab.cpp"
    break;

  case 23: // gen_native_server: GEN_NATIVE_SERVER
#line 342 "Parser.yy"
                                      {
					 	Global::Log << "Generating NATIVE Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateNATIVE_Server = true;
					 }
#line 1074 "Parser.tab.cpp"
    break;

  case 24: // no_gen_native_server: NO_GEN_NATIVE_SERVER
#line 349 "Parser.yy"
                                            {
						 Global::Log << "Not generating NATIVE Server protocol and code.";
						 PrintLine();
						 Global::GScope.GenerateNATIVE_Server = false;
					 }
#line 1084 "Parser.tab.cpp"
    break;

  case 25: // gen_rest: GEN_REST
#line 356 "Parser.yy"
                    {
						Global::Log << "Global::Generating REST Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = true;
						Global::GScope.GenerateREST_Server = true;
					 }
#line 1095 "Parser.tab.cpp"
    break;

  case 26: // no_gen_rest: NO_GEN_REST
#line 364 "Parser.yy"
                          {
						Global::Log << "Global::Not generating REST Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = false;
						Global::GScope.GenerateREST_Server = false;
					 }
#line 1106 "Parser.tab.cpp"
    break;

  case 27: // gen_rest_client: GEN_REST_CLIENT
#line 372 "Parser.yy"
                                  {
						Global::Log << "Generating REST Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = true;
					 }
#line 1116 "Parser.tab.cpp"
    break;

  case 28: // no_gen_rest_client: NO_GEN_REST_CLIENT
#line 379 "Parser.yy"
                                        {
						Global::Log << "Not generating REST Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Client = false;
					 }
#line 1126 "Parser.tab.cpp"
    break;

  case 29: // gen_rest_server: GEN_REST_SERVER
#line 386 "Parser.yy"
                                  {
						Global::Log << "Global::Generating REST Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Server = true;
					 }
#line 1136 "Parser.tab.cpp"
    break;

  case 30: // no_gen_rest_server: NO_GEN_REST_SERVER
#line 393 "Parser.yy"
                                        {
						Global::Log << "Global::Not generating REST Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateREST_Server = false;
					 }
#line 1146 "Parser.tab.cpp"
    break;

  case 31: // gen_xdr: GEN_XDR
#line 400 "Parser.yy"
                  {
						Global::Log << "Global::Generating XDR Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = true;
						Global::GScope.GenerateXDR_Server = true;
					 }
#line 1157 "Parser.tab.cpp"
    break;

  case 32: // no_gen_xdr: NO_GEN_XDR
#line 408 "Parser.yy"
                        {
						Global::Log << "Global::Not generating XDR Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = false;
						Global::GScope.GenerateXDR_Server = false;
					 }
#line 1168 "Parser.tab.cpp"
    break;

  case 33: // gen_xdr_client: GEN_XDR_CLIENT
#line 416 "Parser.yy"
                                {
						Global::Log << "Generating XDR Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = true;
					 }
#line 1178 "Parser.tab.cpp"
    break;

  case 34: // no_gen_xdr_client: NO_GEN_XDR_CLIENT
#line 423 "Parser.yy"
                                      {
						Global::Log << "Not generating XDR Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Client = false;
					 }
#line 1188 "Parser.tab.cpp"
    break;

  case 35: // gen_xdr_server: GEN_XDR_SERVER
#line 430 "Parser.yy"
                                {
						Global::Log << "Generating XDR Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Server = true;
					 }
#line 1198 "Parser.tab.cpp"
    break;

  case 36: // no_gen_xdr_server: NO_GEN_XDR_SERVER
#line 437 "Parser.yy"
                                      {
						Global::Log << "Not generating XDR Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXDR_Server = false;
					 }
#line 1208 "Parser.tab.cpp"
    break;

  case 37: // gen_xml: GEN_XML
#line 444 "Parser.yy"
                  {
						Global::Log << "Generating XML Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = true;
						Global::GScope.GenerateXML_Server = true;
					 }
#line 1219 "Parser.tab.cpp"
    break;

  case 38: // no_gen_xml: NO_GEN_XML
#line 452 "Parser.yy"
                        {
						Global::Log << "Not generating XML Client and Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = false;
						Global::GScope.GenerateXML_Server = false;
					 }
#line 1230 "Parser.tab.cpp"
    break;

  case 39: // gen_xml_client: GEN_XML_CLIENT
#line 460 "Parser.yy"
                                {
						Global::Log << "Generating XML Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = true;
					 }
#line 1240 "Parser.tab.cpp"
    break;

  case 40: // no_gen_xml_client: NO_GEN_XML_CLIENT
#line 467 "Parser.yy"
                                      {
						Global::Log << "Not generating XML Client protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Client = false;
					 }
#line 1250 "Parser.tab.cpp"
    break;

  case 41: // gen_xml_server: GEN_XML_SERVER
#line 474 "Parser.yy"
                                {
						Global::Log << "Generating XML Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Server = true;
					 }
#line 1260 "Parser.tab.cpp"
    break;

  case 42: // no_gen_xml_server: NO_GEN_XML_SERVER
#line 481 "Parser.yy"
                                      {
						Global::Log << "Not generating XML Server protocol and code.";
	 						PrintLine();
						Global::GScope.GenerateXML_Server = false;
					 }
#line 1270 "Parser.tab.cpp"
    break;

  case 43: // packlr: PACKLR
#line 488 "Parser.yy"
                 {
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
#line 1286 "Parser.tab.cpp"
    break;

  case 44: // packrl: PACKRL
#line 501 "Parser.yy"
                {
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
#line 1302 "Parser.tab.cpp"
    break;

  case 45: // packnbo: PACKNBO
#line 514 "Parser.yy"
                  {
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
#line 1318 "Parser.tab.cpp"
    break;

  case 46: // bitstream: BITSTREAM
#line 527 "Parser.yy"
                      {
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
#line 1334 "Parser.tab.cpp"
    break;

  case 47: // defaultpacksize: DEFAULTPACKSIZE COLON DIGITS
#line 541 "Parser.yy"
                                               {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.PackSize = (uint8_t)yystack_[0].value.as < uint64_t > ();
							 PackingSizeSet = true;
							 Global::Log << "Deafult Packing set to " << yystack_[0].value.as < uint64_t > () << " bits";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
#line 1350 "Parser.tab.cpp"
    break;

  case 48: // packsize: PACKSIZE COLON DIGITS
#line 554 "Parser.yy"
                                 {
						if (!PackingSizeSet) {
							 Global::GScope.Packing.PackSize = (uint8_t)yystack_[0].value.as < uint64_t > ();
							 PackingSizeSet = true;
							 Global::Log << "Deafult Packing set to " << yystack_[0].value.as < uint64_t > () << " bits";
						} else {
							Global::Log << "Error: Packing size already set at the global level.";
							YYERROR;
						}
						PrintLine();
					 }
#line 1366 "Parser.tab.cpp"
    break;

  case 49: // gen_cpp_code: GEN_CPP_CODE
#line 567 "Parser.yy"
                            {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCPP = true;
						 Global::Log << "C++ Code generation turned on.";
						PrintLine();
					 }
#line 1378 "Parser.tab.cpp"
    break;

  case 50: // no_gen_cpp_code: NO_GEN_CPP_CODE
#line 576 "Parser.yy"
                                  {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCPP = false;
						 Global::Log << "C++ Code generation turned off.";
						PrintLine();
					 }
#line 1390 "Parser.tab.cpp"
    break;

  case 51: // gen_csharp_code: GEN_CSHARP_CODE
#line 585 "Parser.yy"
                                  {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCSharp = true;
						 Global::Log << "C# Code generation turned on.";
						PrintLine();
					 }
#line 1402 "Parser.tab.cpp"
    break;

  case 52: // no_gen_csharp_code: NO_GEN_CSHARP_CODE
#line 594 "Parser.yy"
                                        {
					   // We are at the global level.
						 //
						 Global::GScope.GenerateCSharp = false;
						 Global::Log << "C# Code generation turned off.";
						PrintLine();
					 }
#line 1414 "Parser.tab.cpp"
    break;

  case 53: // document: DOCUMENT
#line 604 "Parser.yy"
                    {
						 Global::Log << "Start Document";
						PrintLine();
					 }
#line 1423 "Parser.tab.cpp"
    break;

  case 54: // no_document: NO_DOCUMENT
#line 610 "Parser.yy"
                          {
						 Global::Log << "No Document";
						PrintLine();
					 }
#line 1432 "Parser.tab.cpp"
    break;

  case 55: // dnamespace: NAMESPACE EQUAL namespacestring
#line 616 "Parser.yy"
                                              {
					 	 Global::Log << "Namespace: " << yystack_[0].value.as < std::string > ();
	 						PrintLine();
						 CurrentScope->Namespace = yystack_[0].value.as < std::string > ();
					 }
#line 1442 "Parser.tab.cpp"
    break;

  case 109: // begin_public: VISIBILITY_PUBLIC
#line 684 "Parser.yy"
                                                 {
								CurrentVisibility = Symbol::Public_t;
						 }
#line 1450 "Parser.tab.cpp"
    break;

  case 110: // begin_protected: VISIBILITY_PROTECTED
#line 690 "Parser.yy"
                                                 {
								CurrentVisibility = Symbol::Protected_t;
						 }
#line 1458 "Parser.tab.cpp"
    break;

  case 111: // begin_internal: VISIBILITY_INTERNAL
#line 696 "Parser.yy"
                                                 {
								CurrentVisibility = Symbol::Internal_t;
						 }
#line 1466 "Parser.tab.cpp"
    break;

  case 112: // begin_private: VISIBILITY_PRIVATE
#line 702 "Parser.yy"
                                                 {
								CurrentVisibility = Symbol::Private_t;
						 }
#line 1474 "Parser.tab.cpp"
    break;

  case 113: // namespacestring: IDSTRING
#line 707 "Parser.yy"
                           {
									BuildingNS = yystack_[0].value.as < std::string > ();
									yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
              	}
#line 1483 "Parser.tab.cpp"
    break;

  case 114: // namespacestring: namespacestring COLON IDSTRING
#line 711 "Parser.yy"
                                                                                                       {
									BuildingNS = yystack_[2].value.as < std::string > () + ":" + yystack_[0].value.as < std::string > ();
									yylhs.value.as < std::string > () = BuildingNS;
								}
#line 1492 "Parser.tab.cpp"
    break;

  case 117: // version: VERSION IDSTRING LCURLY contents RCURLY SEMI
#line 722 "Parser.yy"
                                {
					Global::Log << "Protocol Version: " << yystack_[4].value.as < std::string > ();
						PrintLine();
				}
#line 1501 "Parser.tab.cpp"
    break;

  case 134: // constuint: CONST UINT IDSTRING EQUAL DIGITS
#line 751 "Parser.yy"
           {
						Global::Log << "CONST UNSIGNED INTEGER: " << yystack_[2].value.as < std::string > ();
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, yystack_[2].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<uint64_t> * NewValue = new SymbolValueT<uint64_t>(yystack_[0].value.as < uint64_t > ());
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1516 "Parser.tab.cpp"
    break;

  case 135: // constuint: CONST UINT COLON DIGITS IDSTRING EQUAL DIGITS
#line 763 "Parser.yy"
           {
						Global::Log << "CONST UNSIGNED " << yystack_[3].value.as < uint64_t > () << "-BIT INTEGER: " << yystack_[2].value.as < std::string > ();
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						SymbolValueT<uint64_t> * NewValue = new SymbolValueT<uint64_t>(yystack_[0].value.as < uint64_t > ());
						CurrentSymbol->Value = NewValue;
						CurrentSymbol->IsConstant = true;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1531 "Parser.tab.cpp"
    break;

  case 161: // range_comparison: LESS_THAN
#line 818 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::LessThan_t;
								 }
#line 1539 "Parser.tab.cpp"
    break;

  case 162: // range_comparison: LESS_THAN_OR_EQUAL
#line 823 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::LessThanOrEqualTo_t;
								 }
#line 1547 "Parser.tab.cpp"
    break;

  case 163: // range_comparison: GREATER_THAN_OR_EQUAL
#line 828 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::GreaterThanOrEqualTo_t;
								 }
#line 1555 "Parser.tab.cpp"
    break;

  case 164: // range_comparison: GREATER_THAN
#line 833 "Parser.yy"
                                                                 {
									yylhs.value.as < Range::RangeCmp_e > () = Range::GreaterThan_t;
								 }
#line 1563 "Parser.tab.cpp"
    break;

  case 168: // uint_default: DEFAULT LPAREN DIGITS RPAREN
#line 844 "Parser.yy"
                                                 {
							CurrentUintDefaultValue = new DefaultValueT<uint64_t>(yystack_[1].value.as < uint64_t > ());
							yylhs.value.as < DefaultValueT<uint64_t>* > () = CurrentUintDefaultValue;
						 }
#line 1572 "Parser.tab.cpp"
    break;

  case 169: // uint_default: DEFAULT LPAREN IDSTRING RPAREN
#line 850 "Parser.yy"
                                                 {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, yystack_[1].value.as < std::string > ());
							if (Found != nullptr) {
								 if (Found->IsConstant) {
								  SymbolValueT<uint64_t> * VarValue = dynamic_cast<SymbolValueT<uint64_t>*>(Found->Value);
								  CurrentUintDefaultValue = new DefaultValueT<uint64_t>(VarValue->Value);
									yylhs.value.as < DefaultValueT<uint64_t>* > () = CurrentUintDefaultValue;
									CurrentSymbol->Default = CurrentUintDefaultValue;
								 } else {
								  Global::Log << "ERROR: Symbol " << yystack_[1].value.as < std::string > () << " was found, it must be a constant to be used in a Default\n";
									YYERROR;
								 }
							} else {
								  Global::Log << "ERROR: Symbol " << yystack_[1].value.as < std::string > () << " was not found, it must be alredy defined and be a constant to be used in a Default\n";
									YYERROR;
							}
						 }
#line 1596 "Parser.tab.cpp"
    break;

  case 170: // uint_range: RANGE LPAREN range_comparison DIGITS COMMA range_comparison DIGITS RPAREN
#line 872 "Parser.yy"
                                         {
							CurrentRange = new RangeT<uint64_t>(yystack_[5].value.as < Range::RangeCmp_e > (), yystack_[4].value.as < uint64_t > (), yystack_[2].value.as < Range::RangeCmp_e > (), yystack_[1].value.as < uint64_t > ());
							yylhs.value.as < Range* > () = CurrentRange;
							CurrentSymbol->ValidRange = CurrentRange;
					 }
#line 1606 "Parser.tab.cpp"
    break;

  case 171: // uint_range: RANGE LPAREN range_comparison IDSTRING COMMA range_comparison DIGITS RPAREN
#line 879 "Parser.yy"
                                         {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, yystack_[4].value.as < std::string > ());
							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<uint> * SValue = dynamic_cast<SymbolValueT<uint>*>(Found->Value);
									CurrentRange = new RangeT<uint64_t>(yystack_[5].value.as < Range::RangeCmp_e > (), SValue->Value, yystack_[2].value.as < Range::RangeCmp_e > (), yystack_[1].value.as < uint64_t > ());
									yylhs.value.as < Range* > () = CurrentRange;
									CurrentSymbol->ValidRange = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << yystack_[4].value.as < std::string > ()
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }
#line 1628 "Parser.tab.cpp"
    break;

  case 172: // uint_range: RANGE LPAREN range_comparison DIGITS COMMA range_comparison IDSTRING RPAREN
#line 898 "Parser.yy"
                                         {
						  /** IDSTRING must alredy be defined as a constant. */

							Symbol * Found = SymbolTable::Find(CurrentScope->Namespace, yystack_[1].value.as < std::string > ());
							if (Found != nullptr && Found->Value != nullptr && Found->IsConstant) {
								 	SymbolValueT<uint> * SValue = dynamic_cast<SymbolValueT<uint>*>(Found->Value);
									CurrentRange = new RangeT<uint64_t>(yystack_[5].value.as < Range::RangeCmp_e > (), yystack_[4].value.as < uint64_t > (), yystack_[2].value.as < Range::RangeCmp_e > (), SValue->Value);
									yylhs.value.as < Range* > () = CurrentRange;
									CurrentSymbol->ValidRange = CurrentRange;
									
							} else {
								  Global::Log << "ERROR: Symbol " << yystack_[1].value.as < std::string > ()
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
					 }
#line 1650 "Parser.tab.cpp"
    break;

  case 173: // uint_range: RANGE LPAREN range_comparison IDSTRING COMMA range_comparison IDSTRING RPAREN
#line 917 "Parser.yy"
                                         {
						  /** IDSTRING must alredy be defined as a constant. */

							SymbolValueT<uint> * SValue4 = nullptr;
							SymbolValueT<uint> * SValue7 = nullptr;
							
							Symbol * Found4 = SymbolTable::Find(CurrentScope->Namespace, yystack_[4].value.as < std::string > ());
							if (Found4 != nullptr && Found4->Value != nullptr && Found4->IsConstant) {
								 	SValue4 = dynamic_cast<SymbolValueT<uint>*>(Found4->Value);
									
							} else {
								  Global::Log << "ERROR: Symbol " << yystack_[4].value.as < std::string > ()
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}

							Symbol * Found7 = SymbolTable::Find(CurrentScope->Namespace, yystack_[1].value.as < std::string > ());
							if (Found7 != nullptr && Found7->Value != nullptr && Found7->IsConstant) {
								 	SValue7 = dynamic_cast<SymbolValueT<uint>*>(Found7->Value);
							} else {
								  Global::Log << "ERROR: Symbol " << yystack_[1].value.as < std::string > ()
															<< " was not found, or not a constant,"
															<< " it must be alredy defined and be a constant to be used in a Range\n";
									YYERROR;
							}
							if (SValue4 != nullptr && SValue7 != nullptr) {
									CurrentRange = new RangeT<uint64_t>(yystack_[5].value.as < Range::RangeCmp_e > (), SValue4->Value, yystack_[2].value.as < Range::RangeCmp_e > (), SValue7->Value);
									yylhs.value.as < Range* > () = CurrentRange;
									CurrentSymbol->ValidRange = CurrentRange;
							}
					 }
#line 1687 "Parser.tab.cpp"
    break;

  case 177: // uintsingle: UINT IDSTRING opt_uint
#line 958 "Parser.yy"
           {
						Global::Log << "UNSIGNED INTEGER: " << yystack_[1].value.as < std::string > ();
						PrintLine();
						CurrentSymbol = new Symbol(Symbol::uint_t, yystack_[1].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;

						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1705 "Parser.tab.cpp"
    break;

  case 178: // uintsingle: UINT COLON DIGITS IDSTRING opt_uint
#line 973 "Parser.yy"
           {
						Global::Log << "UNSIGNED " << yystack_[2].value.as < uint64_t > () << "-BIT INTEGER: " << yystack_[1].value.as < std::string > ();
						PrintLine();
						CurrentSymbol = new Symbol( Symbol::uint_t, yystack_[2].value.as < uint64_t > (), yystack_[1].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentSymbol->Default = CurrentUintDefaultValue;
						CurrentUintDefaultValue = nullptr;
						CurrentSymbol->ValidRange = CurrentRange;
						CurrentRange = nullptr;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1721 "Parser.tab.cpp"
    break;

  case 179: // uintfixedarray: UINT IDSTRING LSQUARE DIGITS RSQUARE opt_uint
#line 987 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[2].value.as < uint64_t > ()
						     << " UNSIGNED INTEGERS: "
								 << yystack_[4].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::uint_t, yystack_[4].value.as < std::string > (), Symbol::FixedArray_t, yystack_[2].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1735 "Parser.tab.cpp"
    break;

  case 180: // uintfixedarray: UINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE opt_uint
#line 998 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[2].value.as < uint64_t > ()
						     << " UNSIGNED " << yystack_[5].value.as < uint64_t > ()
								 << "-BIT INTEGERS: "
								 << yystack_[4].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::uint_t, yystack_[5].value.as < uint64_t > (), yystack_[4].value.as < std::string > (), Symbol::FixedArray_t, yystack_[2].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1750 "Parser.tab.cpp"
    break;

  case 181: // uintvariablearray: UINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_uint
#line 1011 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[4].value.as < uint64_t > ()
						     << " to " << yystack_[2].value.as < uint64_t > ()
						     << " UNSIGNED INTEGERS: "
								 << yystack_[6].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::uint_t, yystack_[6].value.as < std::string > (), Symbol::VariableArray_t, yystack_[4].value.as < uint64_t > (), yystack_[2].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1765 "Parser.tab.cpp"
    break;

  case 182: // uintvariablearray: UINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN opt_uint
#line 1023 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[4].value.as < uint64_t > ()
						     << " to " << yystack_[2].value.as < uint64_t > () << " "
								 << yystack_[7].value.as < uint64_t > () << "-BIT"
						     << " UNSIGNED INTEGERS: "
								 << yystack_[6].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::uint_t, yystack_[7].value.as < uint64_t > (), yystack_[6].value.as < std::string > (), Symbol::VariableArray_t, yystack_[4].value.as < uint64_t > (), yystack_[2].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1781 "Parser.tab.cpp"
    break;

  case 183: // uintvariablearray: UINT IDSTRING LESS_THAN GREATER_THAN opt_uint
#line 1036 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more UNSIGNED INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::uint_t, yystack_[3].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1794 "Parser.tab.cpp"
    break;

  case 184: // uintvariablearray: UINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN opt_uint
#line 1046 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[4].value.as < uint64_t > () << "-BIT UNSIGNED INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::uint_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1808 "Parser.tab.cpp"
    break;

  case 188: // sintsingle: SINT IDSTRING
#line 1063 "Parser.yy"
           {
						Global::Log << "SIGNED INTEGER: " << yystack_[0].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[0].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1820 "Parser.tab.cpp"
    break;

  case 189: // sintsingle: SINT COLON DIGITS IDSTRING
#line 1072 "Parser.yy"
           {
						Global::Log << "SIGNED " << yystack_[1].value.as < uint64_t > ()
						     << "-BIT INTEGER: "
								 << yystack_[0].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[1].value.as < uint64_t > (), yystack_[0].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1834 "Parser.tab.cpp"
    break;

  case 190: // sintfixedarray: SINT IDSTRING LSQUARE DIGITS RSQUARE
#line 1084 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " SIGNED INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1848 "Parser.tab.cpp"
    break;

  case 191: // sintfixedarray: SINT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
#line 1095 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " SIGNED " << yystack_[4].value.as < uint64_t > ()
								 << "-BIT INTEGERS: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1863 "Parser.tab.cpp"
    break;

  case 192: // sintvariablearray: SINT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1108 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " SIGNED INTEGERS: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1878 "Parser.tab.cpp"
    break;

  case 193: // sintvariablearray: SINT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1120 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > () << " "
								 << yystack_[6].value.as < uint64_t > () << "-BIT"
						     << " SIGNED INTEGERS: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[6].value.as < uint64_t > (), yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1894 "Parser.tab.cpp"
    break;

  case 194: // sintvariablearray: SINT IDSTRING LESS_THAN GREATER_THAN
#line 1133 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more SIGNED INTEGERS: " << yystack_[2].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1906 "Parser.tab.cpp"
    break;

  case 195: // sintvariablearray: SINT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
#line 1142 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[3].value.as < uint64_t > () << "-BIT SIGNED INTEGERS: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::sint_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1920 "Parser.tab.cpp"
    break;

  case 198: // opaquefixedarray: OPAQUE IDSTRING LSQUARE DIGITS RSQUARE
#line 1158 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " OPAQUE: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::opaque_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1934 "Parser.tab.cpp"
    break;

  case 199: // opaquevariablearray: OPAQUE IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1170 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " OPAQUE: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::opaque_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1949 "Parser.tab.cpp"
    break;

  case 200: // opaquevariablearray: OPAQUE IDSTRING LESS_THAN GREATER_THAN
#line 1182 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more OPAQUE: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::opaque_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1962 "Parser.tab.cpp"
    break;

  case 204: // floatsingle: FLOAT IDSTRING
#line 1199 "Parser.yy"
           {
						Global::Log << "FLOAT: " << yystack_[0].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[0].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1974 "Parser.tab.cpp"
    break;

  case 205: // floatsingle: FLOAT COLON DIGITS IDSTRING
#line 1208 "Parser.yy"
           {
						Global::Log << yystack_[1].value.as < uint64_t > ()
						     << "-BIT FLOAT: "
								 << yystack_[0].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[1].value.as < uint64_t > (), yystack_[0].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 1988 "Parser.tab.cpp"
    break;

  case 206: // floatfixedarray: FLOAT IDSTRING LSQUARE DIGITS RSQUARE
#line 1220 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " FLOAT: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2002 "Parser.tab.cpp"
    break;

  case 207: // floatfixedarray: FLOAT COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
#line 1231 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << yystack_[4].value.as < uint64_t > ()
								 << "-BIT FLOAT: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2017 "Parser.tab.cpp"
    break;

  case 208: // floatvariablearray: FLOAT IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1244 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " FLOAT: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2032 "Parser.tab.cpp"
    break;

  case 209: // floatvariablearray: FLOAT COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1256 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > () << " "
								 << yystack_[6].value.as < uint64_t > () << "-BIT"
						     << " FLOAT: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[6].value.as < uint64_t > (), yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2048 "Parser.tab.cpp"
    break;

  case 210: // floatvariablearray: FLOAT IDSTRING LESS_THAN GREATER_THAN
#line 1269 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more FLOAT: " << yystack_[2].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2060 "Parser.tab.cpp"
    break;

  case 211: // floatvariablearray: FLOAT COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
#line 1278 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[3].value.as < uint64_t > () << "-BIT FLOAT: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::float_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2074 "Parser.tab.cpp"
    break;

  case 215: // stringsingle: STRING IDSTRING
#line 1295 "Parser.yy"
           {
						Global::Log << "STRING: " << yystack_[0].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[0].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2086 "Parser.tab.cpp"
    break;

  case 216: // stringsingle: STRING COLON DIGITS IDSTRING
#line 1304 "Parser.yy"
           {
						Global::Log << yystack_[1].value.as < uint64_t > ()
						     << "-BIT STRING: "
								 << yystack_[0].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[1].value.as < uint64_t > (), yystack_[0].value.as < std::string > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2100 "Parser.tab.cpp"
    break;

  case 217: // stringfixedarray: STRING IDSTRING LSQUARE DIGITS RSQUARE
#line 1316 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << " STRING: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2114 "Parser.tab.cpp"
    break;

  case 218: // stringfixedarray: STRING COLON DIGITS IDSTRING LSQUARE DIGITS RSQUARE
#line 1327 "Parser.yy"
           {
						Global::Log << "Fixed Array of " << yystack_[1].value.as < uint64_t > ()
						     << yystack_[4].value.as < uint64_t > ()
								 << "-BIT STRING: "
								 << yystack_[3].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[4].value.as < uint64_t > (), yystack_[3].value.as < std::string > (), Symbol::FixedArray_t, yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2129 "Parser.tab.cpp"
    break;

  case 219: // stringvariablearray: STRING IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1340 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > ()
						     << " STRING: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2144 "Parser.tab.cpp"
    break;

  case 220: // stringvariablearray: STRING COLON DIGITS IDSTRING LESS_THAN DIGITS COMMA DIGITS GREATER_THAN
#line 1352 "Parser.yy"
                                         {
						Global::Log << "Variable Array of " << yystack_[3].value.as < uint64_t > ()
						     << " to " << yystack_[1].value.as < uint64_t > () << " "
								 << yystack_[6].value.as < uint64_t > () << "-BIT"
						     << " STRING: "
								 << yystack_[5].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[6].value.as < uint64_t > (), yystack_[5].value.as < std::string > (), Symbol::VariableArray_t, yystack_[3].value.as < uint64_t > (), yystack_[1].value.as < uint64_t > ());
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2160 "Parser.tab.cpp"
    break;

  case 221: // stringvariablearray: STRING IDSTRING LESS_THAN GREATER_THAN
#line 1365 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more STRING: " << yystack_[2].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2172 "Parser.tab.cpp"
    break;

  case 222: // stringvariablearray: STRING COLON DIGITS IDSTRING LESS_THAN GREATER_THAN
#line 1374 "Parser.yy"
                                         {
						Global::Log << "Variable Array of zero or more "
						     << yystack_[3].value.as < uint64_t > () << "-BIT STRING: "
								 << yystack_[2].value.as < std::string > ();
 						PrintLine();
						CurrentSymbol = new Symbol( Symbol::string_t, yystack_[3].value.as < uint64_t > (), yystack_[2].value.as < std::string > (), Symbol::VariableArray_t, 0, 0);
						CurrentSymbol->Namespace = CurrentScope->Namespace;
						CurrentScope->Symbols.Add(CurrentSymbol, LineNumber);
					 }
#line 2186 "Parser.tab.cpp"
    break;

  case 223: // methodsig: mreturn IDSTRING LPAREN parameters RPAREN
#line 1386 "Parser.yy"
                                        {
						Global::Log << "Method: " << yystack_[3].value.as < std::string > ()
						     << " Returning: " << yystack_[3].value.as < std::string > (); /**@todo*/
 						PrintLine();
					}
#line 2196 "Parser.tab.cpp"
    break;


#line 2200 "Parser.tab.cpp"

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


  const short Parser::yypact_ninf_ = -244;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
    -244,    13,  -244,  -244,     1,  -244,    86,  -244,    -1,  -244,
    -244,  -244,  -244,     6,    48,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,    37,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,    36,  -244,    86,
      73,    15,    58,    91,  -244,    86,    55,   119,  -244,  -244,
    -244,    75,  -244,  -244,   143,   198,   220,    -6,  -244,  -244,
     193,   211,  -244,    10,  -244,   222,     2,   223,     4,    47,
      49,  -244,   224,    89,  -244,  -244,  -244,  -244,   225,  -244,
    -244,  -244,  -244,    -4,  -244,   215,  -244,  -244,  -244,  -244,
     216,   218,   221,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,   226,   229,  -244,    96,   233,   230,   164,   235,
     165,   237,   168,   238,   176,   239,   112,   169,   170,   227,
    -244,  -244,  -244,  -244,  -244,  -244,   236,   241,    -2,   240,
      74,   245,    12,   196,   243,   247,    34,   246,   248,    43,
     249,   250,    44,   232,   234,   254,   242,   255,   244,   256,
    -244,   161,   251,   252,  -244,   177,  -244,    42,  -244,  -244,
    -244,   257,   253,  -244,   258,   259,  -244,  -244,   181,   260,
     261,  -244,   185,   263,   264,  -244,   186,   266,   267,  -244,
     155,   269,   272,   217,   275,   107,   277,   222,  -244,  -244,
      83,  -244,  -244,   270,   281,    53,  -244,  -244,  -244,   282,
     196,    88,   187,   283,    54,   196,  -244,   284,   285,    59,
    -244,   286,   287,    60,  -244,   288,  -244,  -244,  -244,  -244,
     271,  -244,  -244,   273,  -244,  -244,  -244,   274,  -244,  -244,
     268,   280,   289,  -244,   196,   276,   290,   291,  -244,  -244,
    -244,  -244,   197,   292,   293,  -244,   294,   295,   296,  -244,
     297,   298,   299,  -244,   300,   302,   219,   214,  -244,  -244,
     304,  -244,  -244,  -244,   301,   305,  -244,   307,   196,  -244,
    -244,   310,  -244,  -244,   312,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,   303,   196,   187,   187,   196,   306,   308,   309,
    -244,   206,   210,  -244,  -244,  -244,   311,   313,   314,   316,
     196,  -244,  -244,  -244,  -244
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
     113,    55,    59,    57,     0,     0,     0,     0,   115,   114,
       0,     0,   116,     4,     2,     0,     0,     0,     0,     0,
       0,   225,     0,     0,   109,   110,   111,   112,   124,   126,
     127,   128,   129,     4,   118,     0,   130,   131,   132,   133,
       0,     0,     0,   149,   165,   166,   167,   150,   185,   186,
     187,   153,   196,   197,   151,   201,   202,   203,   152,   212,
     213,   214,     0,     0,   160,   215,     0,     0,   174,     0,
     188,     0,   204,     0,     0,     0,     0,     0,     0,     0,
     119,   125,   120,   121,   122,   123,     0,     0,     0,     0,
       0,     0,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     117,   226,     0,     0,   221,   216,   157,     0,   155,   158,
     159,     0,     0,   174,     0,     0,   175,   176,   174,     0,
       0,   194,   189,     0,     0,   210,   205,     0,     0,   200,
       0,     0,     0,     0,     0,     0,     0,     0,   229,   230,
       0,   227,   217,     0,     0,     0,   154,   156,   174,     0,
     183,     0,     0,     0,     0,   178,   190,     0,     0,     0,
     206,     0,     0,     0,   198,     0,   146,   147,   148,   134,
       0,   136,   138,     0,   140,   142,   144,     0,   223,   228,
       0,     0,     0,   222,   179,     0,     0,     0,   161,   162,
     163,   164,     0,     0,     0,   174,     0,     0,     0,   195,
       0,     0,     0,   211,     0,     0,     0,     0,   219,   218,
       0,   174,   168,   169,     0,     0,   174,     0,   184,   192,
     191,     0,   208,   207,     0,   199,   135,   137,   139,   141,
     143,   145,     0,   181,     0,     0,   180,     0,     0,     0,
     220,     0,     0,   174,   193,   209,     0,     0,     0,     0,
     182,   170,   172,   171,   173
  };

  const short
  Parser::yypgoto_[] =
  {
    -244,  -244,  -244,   315,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,   151,   146,  -244,  -244,  -244,
    -244,  -244,  -244,   135,  -244,   110,  -244,  -244,  -244,  -244,
    -244,  -244,  -199,  -244,  -244,    38,  -177,  -140,  -244,  -244,
    -244,  -243,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,  -244,
    -203,  -244,  -244,    27
  };

  const short
  Parser::yydefgoto_[] =
  {
       0,     1,     2,   148,     7,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,     8,   107,   108,   149,   150,   151,
     152,   121,   127,   128,   153,   154,   155,   156,   157,   158,
     298,   159,   160,   161,   237,   238,   162,   322,   163,   246,
     247,   213,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   270,   271
  };

  const short
  Parser::yytable_[] =
  {
     280,   233,   135,   136,   131,   285,   199,   240,   185,     5,
     188,   236,   109,     3,     6,   242,   135,   136,   118,   186,
     126,   189,   234,   111,   110,   137,   138,   139,   140,   141,
     142,   143,   268,   239,   240,   314,   243,   250,   236,   137,
     138,   139,   140,   141,   142,   143,   254,   258,   135,   136,
     114,   115,   276,   190,   269,   192,   312,   324,   251,   113,
     239,   119,   328,   332,   191,   112,   193,   255,   259,   123,
     115,   268,   138,   139,   140,   141,   142,   313,   325,   117,
     135,   136,   348,   329,   333,   144,   145,   146,   147,   267,
     136,   316,   125,   269,   317,   308,   195,   120,   363,   144,
     145,   146,   147,   366,   138,   139,   140,   141,   142,   207,
     304,   305,   306,   138,   139,   140,   208,   142,   224,   196,
     197,   198,     9,    10,    11,    12,    13,    14,   124,   225,
     380,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,   267,   136,   126,
      51,    52,    53,    54,    55,   226,   228,   211,   215,    56,
      57,   218,   296,   297,   212,   216,   227,   229,   219,   221,
     274,   138,   139,   140,   283,   142,   222,   275,   288,   292,
     344,   284,   133,   345,   129,   289,   293,   318,   319,   376,
     320,   321,   377,   378,   244,   245,   379,   359,   360,   361,
     301,   302,   357,   358,   371,   372,   130,   134,   184,   187,
     194,   201,   202,     5,   203,   206,   209,   204,   214,   210,
     217,   220,   205,   230,   232,   223,   235,   231,   241,   248,
     249,   253,   252,   257,   260,   256,   261,   262,   264,   266,
     116,   122,   132,   200,   263,   272,   265,   273,   279,   281,
     282,   278,   299,   310,   286,   277,   287,   290,   300,   291,
     294,   303,   295,   307,   311,   315,   323,   326,   327,   330,
     331,   334,   338,   335,   339,   336,   337,   309,     0,     0,
     341,     0,   342,   343,   340,   356,   346,   362,   347,   350,
     367,   351,   353,   368,   354,   369,   364,     4,   349,     0,
     365,   352,     0,   381,   355,   382,   383,   370,   384,     0,
     373,     0,   374,   375
  };

  const short
  Parser::yycheck_[] =
  {
     243,     3,     6,     7,    10,   248,    10,   210,     6,     8,
       6,   210,    13,     0,    13,     3,     6,     7,     3,    17,
      26,    17,    24,    17,    25,    29,    30,    31,    32,    33,
      34,    35,   231,   210,   237,   278,    24,     3,   237,    29,
      30,    31,    32,    33,    34,    35,     3,     3,     6,     7,
      14,    15,    10,     6,   231,     6,     3,     3,    24,    22,
     237,     3,     3,     3,    17,    17,    17,    24,    24,    14,
      15,   270,    30,    31,    32,    33,    34,    24,    24,     6,
       6,     7,   325,    24,    24,    89,    90,    91,    92,     6,
       7,     3,    17,   270,     6,    12,     7,     6,   341,    89,
      90,    91,    92,   346,    30,    31,    32,    33,    34,    13,
       3,     4,     5,    30,    31,    32,    20,    34,     6,    30,
      31,    32,    36,    37,    38,    39,    40,    41,     9,    17,
     373,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,     6,     7,    26,
      84,    85,    86,    87,    88,     6,     6,    13,    13,    93,
      94,    13,    27,    28,    20,    20,    17,    17,    20,    13,
      13,    30,    31,    32,    13,    34,    20,    20,    13,    13,
       3,    20,     9,     6,     6,    20,    20,    20,    21,     3,
      23,    24,     6,     3,    18,    19,     6,     3,     4,     5,
       3,     4,     3,     4,   364,   365,     6,    16,     6,     6,
       6,    16,    16,     8,    16,     6,     3,    16,     3,     9,
       3,     3,    16,    16,     3,     6,     6,    11,     3,     6,
       3,     3,     6,     3,    22,     6,    22,     3,     3,     3,
     109,   115,   127,   153,    22,    14,    22,    15,    15,    11,
      11,    14,     3,     3,    14,   237,    15,    14,     6,    15,
      14,     6,    15,     6,     3,     3,     3,     3,     3,     3,
       3,     3,    24,    22,    14,    22,    22,   270,    -1,    -1,
      24,    -1,    12,    12,    15,     3,    14,     3,    15,    14,
       3,    15,    14,     3,    15,     3,    15,     2,    24,    -1,
      15,    24,    -1,    12,    24,    12,    12,    24,    12,    -1,
      24,    -1,    24,    24
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,    96,    97,     0,    98,     8,    13,    99,   149,    36,
      37,    38,    39,    40,    41,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    84,    85,    86,    87,    88,    93,    94,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   150,   151,    13,
      25,    17,    17,    22,    14,    15,   150,     6,     3,     3,
       6,   156,   151,    14,     9,    17,    26,   157,   158,     6,
       6,    10,   158,     9,    16,     6,     7,    29,    30,    31,
      32,    33,    34,    35,    89,    90,    91,    92,    98,   152,
     153,   154,   155,   159,   160,   161,   162,   163,   164,   166,
     167,   168,   171,   173,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,     6,     6,    17,     6,     6,    17,
       6,    17,     6,    17,     6,     7,    30,    31,    32,    10,
     160,    16,    16,    16,    16,    16,     6,    13,    20,     3,
       9,    13,    20,   176,     3,    13,    20,     3,    13,    20,
       3,    13,    20,     6,     6,    17,     6,    17,     6,    17,
      16,    11,     3,     3,    24,     6,   167,   169,   170,   171,
     195,     3,     3,    24,    18,    19,   174,   175,     6,     3,
       3,    24,     6,     3,     3,    24,     6,     3,     3,    24,
      22,    22,     3,    22,     3,    22,     3,     6,   167,   171,
     197,   198,    14,    15,    13,    20,    10,   170,    14,    15,
     176,    11,    11,    13,    20,   176,    14,    15,    13,    20,
      14,    15,    13,    20,    14,    15,    27,    28,   165,     3,
       6,     3,     4,     6,     3,     4,     5,     6,    12,   198,
       3,     3,     3,    24,   176,     3,     3,     6,    20,    21,
      23,    24,   172,     3,     3,    24,     3,     3,     3,    24,
       3,     3,     3,    24,     3,    22,    22,    22,    24,    14,
      15,    24,    12,    12,     3,     6,    14,    15,   176,    24,
      14,    15,    24,    14,    15,    24,     3,     3,     4,     3,
       4,     5,     3,   176,    15,    15,   176,     3,     3,     3,
      24,   172,   172,    24,    24,    24,     3,     6,     3,     6,
     176,    12,    12,    12,    12
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    95,    96,    97,    98,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   149,   150,   150,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   152,
     153,   154,   155,   156,   156,   157,   157,   158,   159,   159,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     161,   161,   161,   161,   162,   162,   163,   163,   163,   163,
     164,   164,   164,   164,   164,   164,   165,   165,   166,   167,
     167,   167,   167,   167,   168,   169,   169,   170,   170,   170,
     171,   172,   172,   172,   172,   173,   173,   173,   174,   174,
     175,   175,   175,   175,   176,   176,   176,   177,   177,   178,
     178,   179,   179,   179,   179,   180,   180,   180,   181,   181,
     182,   182,   183,   183,   183,   183,   184,   184,   185,   186,
     186,   187,   187,   187,   188,   188,   189,   189,   190,   190,
     190,   190,   191,   191,   191,   192,   192,   193,   193,   194,
     194,   194,   194,   195,   196,   196,   197,   197,   197,   198,
     198
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
       1,     1,     1,     1,     3,     1,     2,     6,     1,     2,
       2,     2,     2,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     7,     5,     7,     5,     7,
       5,     7,     5,     7,     5,     7,     1,     1,     5,     1,
       1,     1,     1,     1,     5,     1,     2,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       8,     8,     8,     8,     0,     2,     2,     3,     5,     6,
       8,     8,    10,     5,     7,     1,     1,     1,     2,     4,
       5,     7,     7,     9,     4,     6,     1,     1,     5,     7,
       4,     1,     1,     1,     2,     4,     5,     7,     7,     9,
       4,     6,     1,     1,     1,     2,     4,     5,     7,     7,
       9,     4,     6,     5,     1,     1,     0,     1,     2,     1,
       1
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
  "OPAQUE", "CONST", "PACKNBO", "PACKLR", "PACKRL", "BITSTREAM",
  "PACKSIZE", "DEFAULTPACKSIZE", "INCLUDE", "SERIALIZE", "NO_SERIALIZE",
  "GEN_CBOR", "GEN_CBOR_CLIENT", "GEN_CBOR_SERVER", "NO_GEN_CBOR",
  "NO_GEN_CBOR_CLIENT", "NO_GEN_CBOR_SERVER", "GEN_JSON",
  "GEN_JSON_CLIENT", "GEN_JSON_SERVER", "NO_GEN_JSON",
  "NO_GEN_JSON_CLIENT", "NO_GEN_JSON_SERVER", "GEN_NATIVE",
  "GEN_NATIVE_CLIENT", "GEN_NATIVE_SERVER", "NO_GEN_NATIVE",
  "NO_GEN_NATIVE_CLIENT", "NO_GEN_NATIVE_SERVER", "GEN_REST",
  "GEN_REST_CLIENT", "GEN_REST_SERVER", "NO_GEN_REST",
  "NO_GEN_REST_CLIENT", "NO_GEN_REST_SERVER", "GEN_XDR", "GEN_XDR_CLIENT",
  "GEN_XDR_SERVER", "NO_GEN_XDR", "NO_GEN_XDR_CLIENT", "NO_GEN_XDR_SERVER",
  "GEN_XML", "GEN_XML_CLIENT", "GEN_XML_SERVER", "NO_GEN_XML",
  "NO_GEN_XML_CLIENT", "NO_GEN_XML_SERVER", "XML_NAMSPACE",
  "XML_ELEMENT_NAME", "XML_ATTRIBUTE_NAME", "NAMESPACE", "GEN_CPP_CODE",
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
  "protocoldirectives", "pdirectives", "directive", "begin_public",
  "begin_protected", "begin_internal", "begin_private", "namespacestring",
  "versions", "version", "contents", "content", "consttype", "constuint",
  "constsint", "constfloat", "anystring", "conststring", "simpletype",
  "classdef", "classcontents", "classcontent", "classtype",
  "range_comparison", "uinttype", "uint_default", "uint_range", "opt_uint",
  "uintsingle", "uintfixedarray", "uintvariablearray", "sinttype",
  "sintsingle", "sintfixedarray", "sintvariablearray", "opaquetype",
  "opaquefixedarray", "opaquevariablearray", "floattype", "floatsingle",
  "floatfixedarray", "floatvariablearray", "stringtype", "stringsingle",
  "stringfixedarray", "stringvariablearray", "methodsig", "mreturn",
  "parameters", "paramtype", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   203,   203,   209,   214,   215,   218,   224,   232,   240,
     247,   254,   261,   268,   276,   284,   291,   298,   305,   312,
     320,   328,   335,   342,   349,   356,   364,   372,   379,   386,
     393,   400,   408,   416,   423,   430,   437,   444,   452,   460,
     467,   474,   481,   488,   501,   514,   527,   541,   554,   567,
     576,   585,   594,   604,   610,   616,   623,   624,   628,   629,
     632,   633,   634,   635,   636,   637,   638,   639,   640,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,   679,   680,   683,
     689,   695,   701,   707,   711,   717,   718,   721,   728,   729,
     732,   733,   734,   735,   736,   737,   738,   739,   740,   741,
     744,   745,   746,   747,   750,   762,   775,   776,   777,   778,
     781,   782,   783,   784,   785,   786,   789,   790,   793,   796,
     797,   798,   799,   800,   803,   805,   806,   809,   810,   811,
     814,   817,   822,   827,   832,   838,   839,   840,   843,   849,
     871,   878,   897,   916,   952,   953,   954,   957,   972,   986,
     997,  1010,  1022,  1035,  1045,  1057,  1058,  1059,  1062,  1071,
    1083,  1094,  1107,  1119,  1132,  1141,  1153,  1154,  1157,  1169,
    1181,  1193,  1194,  1195,  1198,  1207,  1219,  1230,  1243,  1255,
    1268,  1277,  1289,  1290,  1291,  1294,  1303,  1315,  1326,  1339,
    1351,  1364,  1373,  1385,  1393,  1394,  1397,  1398,  1399,  1402,
    1403
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
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94
    };
    // Last valid token kind.
    const int code_max = 349;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 5 "Parser.yy"
} } } // RiverExplorer::Phoenix::Protocol
#line 3031 "Parser.tab.cpp"

#line 1406 "Parser.yy"

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
	Symbol::Visibility_e CurrentVisibility = Symbol::Public_t;
	Range					*		CurrentRange = nullptr;
	DefaultValueT<uint64_t>	*		CurrentUintDefaultValue = nullptr;
	DefaultValueT<int64_t>	*		CurrentSintDefaultValue = nullptr;

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
