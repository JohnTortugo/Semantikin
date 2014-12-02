// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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


// First part of user declarations.
#line 26 "Parsin.yy" // lalr1.cc:399

	#include <cstdio>
	#include <iostream>
	#include <memory>
	#include <string>
	#include "Driver.h"
	#include "FlexScanner.h"

	void Parser::BisonParser::error(const location_type& loc, const std::string& msg) {
		std::cerr << "Location was <Line, Col> = <" << loc.begin.line << "," << loc.begin.column << ">\n";
	}
	
	static int yylex(Parser::BisonParser::semantic_type *tokenVal, Parser::BisonParser::location_type *location, Parser::FlexScanner &scanner, Parser::Driver &driver) {
		return scanner.yylex(tokenVal);	
	}

#line 53 "Parsin.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Parsin.tab.hh"

// User implementation prologue.

#line 67 "Parsin.tab.cc" // lalr1.cc:407


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
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      *yycdebug_ << std::endl;                  \
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
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "Parsin.yy" // lalr1.cc:474
namespace Parser {
#line 153 "Parsin.tab.cc" // lalr1.cc:474

  /// Build a parser object.
  BisonParser::BisonParser (FlexScanner &scanner_yyarg, Driver &driver_yyarg, CompilationUnit*& module_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg),
      module (module_yyarg)
  {}

  BisonParser::~BisonParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  BisonParser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  BisonParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  BisonParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  BisonParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  BisonParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  BisonParser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  BisonParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  BisonParser::by_type::by_type ()
     : type (empty)
  {}

  inline
  BisonParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  BisonParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  BisonParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  BisonParser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  BisonParser::by_state::by_state ()
    : state (empty)
  {}

  inline
  BisonParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  BisonParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  BisonParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  BisonParser::symbol_number_type
  BisonParser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  BisonParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  BisonParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  BisonParser::stack_symbol_type&
  BisonParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  BisonParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  BisonParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  BisonParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  BisonParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  BisonParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  BisonParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  BisonParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  BisonParser::debug_level_type
  BisonParser::debug_level () const
  {
    return yydebug_;
  }

  void
  BisonParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline BisonParser::state_type
  BisonParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  BisonParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  BisonParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  BisonParser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
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

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, scanner, driver));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 149 "Parsin.yy" // lalr1.cc:847
    { module = new CompilationUnit((yystack_[0].value.listFuncs)); }
#line 566 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 152 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listFuncs))->push_front(std::shared_ptr<Function>((yystack_[1].value.function))); (yylhs.value.listFuncs) = (yystack_[0].value.listFuncs); }
#line 572 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 153 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listFuncs) = new list<std::shared_ptr<Function>>(); }
#line 578 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 156 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.function) = new Function((yystack_[5].value.str), (yystack_[4].value.str), shared_ptr<list<shared_ptr<ParamDecl>>>((yystack_[2].value.formParams)), (yystack_[0].value.codeBlock)); }
#line 584 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 159 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.formParams))->push_front(std::shared_ptr<ParamDecl>(new ParamDecl((yystack_[3].value.str), (yystack_[2].value.str), shared_ptr<list<shared_ptr<Expression>>>((yystack_[1].value.listExpr))))); (yylhs.value.formParams) = (yystack_[0].value.formParams); }
#line 590 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 160 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.formParams) = new list<std::shared_ptr<ParamDecl>>(); }
#line 596 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 163 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.formParams))->push_front(std::shared_ptr<ParamDecl>(new ParamDecl((yystack_[3].value.str), (yystack_[2].value.str), shared_ptr<list<shared_ptr<Expression>>>((yystack_[1].value.listExpr))))); (yylhs.value.formParams) = (yystack_[0].value.formParams); }
#line 602 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 164 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.formParams) = new list<std::shared_ptr<ParamDecl>>(); }
#line 608 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 167 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.codeBlock) = new CodeBlock((yystack_[1].value.listStmts)); }
#line 614 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 169 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[2].value.varDecl))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 620 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 170 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[1].value.loopStmt))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 626 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 171 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[1].value.ifStmt))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 632 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 172 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[2].value.ret))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 638 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 173 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[2].value.expr))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 644 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 174 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listStmts) = new list<std::shared_ptr<Statement>>(); }
#line 650 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 177 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[2].value.str), (yystack_[1].value.listExpr), nullptr))); (yylhs.value.varDecl) = new VarDecl((yystack_[3].value.str), (yystack_[0].value.listVarSpec)); }
#line 656 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 178 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[3].value.str), nullptr, (yystack_[1].value.expr)))); (yylhs.value.varDecl) = new VarDecl((yystack_[4].value.str), (yystack_[0].value.listVarSpec)); }
#line 662 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 181 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[2].value.str), (yystack_[1].value.listExpr), nullptr))); (yylhs.value.listVarSpec) = (yystack_[0].value.listVarSpec); }
#line 668 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 182 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[3].value.str), nullptr, (yystack_[1].value.expr)))); (yylhs.value.listVarSpec) = (yystack_[0].value.listVarSpec); }
#line 674 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 183 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listVarSpec) = new list<std::shared_ptr<VarSpec>>(); }
#line 680 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 186 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listExpr))->push_front(std::shared_ptr<Expression>((yystack_[2].value.expr))); (yylhs.value.listExpr) = (yystack_[0].value.listExpr); }
#line 686 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 187 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listExpr) = new list<std::shared_ptr<Expression>>(); }
#line 692 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 190 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.funCall) = new FunctionCall((yystack_[3].value.str), (yystack_[1].value.listExpr)); }
#line 698 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 193 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.loopStmt) = new LoopStmt((yystack_[2].value.expr), (yystack_[0].value.codeBlock)); }
#line 704 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 196 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.ifStmt) = new IfStmt((yystack_[3].value.expr), (yystack_[1].value.codeBlock), nullptr, (yystack_[0].value.listElseIf)); }
#line 710 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 197 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.ifStmt) = new IfStmt((yystack_[5].value.expr), (yystack_[3].value.codeBlock), (yystack_[0].value.codeBlock), (yystack_[2].value.listElseIf)); }
#line 716 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 200 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listElseIf))->push_front(std::shared_ptr<ElseIfStmt>(new ElseIfStmt((yystack_[3].value.expr), (yystack_[1].value.codeBlock)))); (yylhs.value.listElseIf) = (yystack_[0].value.listElseIf); }
#line 722 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 201 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listElseIf) = new list<std::shared_ptr<ElseIfStmt>>(); }
#line 728 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 204 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.ret) = new ReturnStmt((yystack_[0].value.expr)); }
#line 734 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 207 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listExpr))->push_front(std::shared_ptr<Expression>((yystack_[1].value.expr))); (yylhs.value.listExpr) = (yystack_[0].value.listExpr); }
#line 740 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 208 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listExpr) = new list<std::shared_ptr<Expression>>(); }
#line 746 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 211 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listExpr))->push_front(std::shared_ptr<Expression>((yystack_[1].value.expr))); (yylhs.value.listExpr) = (yystack_[0].value.listExpr); }
#line 752 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 212 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listExpr) = new list<std::shared_ptr<Expression>>(); }
#line 758 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 215 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::COMPARE, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 764 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 216 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::ASSIGN, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 770 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 217 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LOG_AND, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 776 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 218 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LOG_OR, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 782 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 219 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::BIT_AND, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 788 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 220 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::BIT_OR, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 794 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 221 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::BIT_XOR, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 800 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 222 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LT, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 806 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 223 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LTE, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 812 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 224 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::GT, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 818 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 225 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::GTE, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 824 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 226 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::ADDITION, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 830 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 227 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::SUBTRACTION, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 836 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 228 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::TIMES, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 842 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 229 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::DIV, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 848 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 230 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::MOD, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 854 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 231 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::PLUS_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 860 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 232 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::MINUS_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 866 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 233 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::TIMES_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 872 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 234 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::DIV_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 878 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 235 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::MOD_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 884 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 236 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = (yystack_[1].value.expr); }
#line 890 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 237 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::NOT, (yystack_[0].value.expr)); }
#line 896 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 238 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::MINUS, (yystack_[0].value.expr)); }
#line 902 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 239 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::PLUS, (yystack_[0].value.expr)); }
#line 908 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 240 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::ADDR, (yystack_[0].value.expr)); }
#line 914 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 241 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::INCREMENT, (yystack_[1].value.expr)); }
#line 920 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 242 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::DECREMENT, (yystack_[1].value.expr)); }
#line 926 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 243 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new IntegerExpr((yystack_[0].value.integer)); }
#line 932 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 244 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new FloatExpr((yystack_[0].value.floating)); }
#line 938 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 245 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new StringExpr((yystack_[0].value.str)); }
#line 944 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 246 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new IdentifierExpr((yystack_[1].value.str), (yystack_[0].value.listExpr)); }
#line 950 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 247 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = (yystack_[0].value.funCall); }
#line 956 "Parsin.tab.cc" // lalr1.cc:847
    break;


#line 960 "Parsin.tab.cc" // lalr1.cc:847
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  BisonParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  BisonParser::yysyntax_error_ (state_type, symbol_number_type) const
  {
    return YY_("syntax error");
  }


  const signed char BisonParser::yypact_ninf_ = -106;

  const signed char BisonParser::yytable_ninf_ = -1;

  const short int
  BisonParser::yypact_[] =
  {
      -8,    -7,     7,  -106,    -8,    -3,  -106,  -106,    21,    22,
      31,    48,    51,   498,    60,   183,  -106,  -106,  -106,  -106,
     -18,   498,   498,   498,   498,   498,  -106,   195,    63,  -106,
      80,    81,   498,    85,    79,    93,   183,   183,    95,   107,
     498,  -106,  -106,   225,    86,    90,    90,   498,   498,   498,
      48,   498,   498,   498,   498,   498,   498,   498,   498,   498,
     498,   498,   498,   498,  -106,  -106,   498,   498,   498,   498,
     498,    99,   498,   498,   345,    41,  -106,   183,  -106,  -106,
     183,   183,    92,    50,  -106,  -106,   -14,   -14,  -106,   -14,
     -14,   375,   375,   405,   405,   405,   405,    90,    90,   -10,
     -10,   148,   435,   435,   465,   465,   502,    48,   255,   285,
     498,   100,  -106,  -106,  -106,  -106,   498,  -106,    60,    51,
      51,   140,   104,  -106,    50,  -106,   111,  -106,  -106,    42,
    -106,    98,   114,   498,   100,   498,    51,   140,  -106,   315,
    -106,  -106,    51,   111,  -106
  };

  const unsigned char
  BisonParser::yydefact_[] =
  {
       4,     0,     0,     2,     4,     0,     1,     3,     7,     0,
       0,    23,     0,     0,     9,    16,     5,    65,    63,    64,
      23,     0,     0,     0,     0,     0,    67,     0,     0,     6,
       0,     0,     0,    23,     0,     0,    16,    16,     0,     0,
      32,    66,    60,     0,    57,    59,    58,     0,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    30,    23,    10,    16,    12,    13,
      16,    16,     0,    34,    56,    39,    40,    41,    22,    37,
      38,    36,    35,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    23,     0,     0,
       0,    21,    11,    14,    15,    24,     0,    31,     9,     0,
       0,    21,     0,    17,    34,     8,    29,    25,    18,    23,
      33,     0,    26,     0,    21,     0,     0,    21,    19,     0,
      27,    20,     0,    29,    28
  };

  const signed char
  BisonParser::yypgoto_[] =
  {
    -106,  -106,   117,  -106,  -106,    16,  -105,   -19,  -106,   -23,
       2,  -106,  -106,  -106,    11,  -106,  -106,    35,   -13
  };

  const short int
  BisonParser::yydefgoto_[] =
  {
      -1,     2,     3,     4,    10,    29,    16,    34,    35,   123,
      41,    26,    36,    37,   132,    38,    82,   117,    39
  };

  const unsigned char
  BisonParser::yytable_[] =
  {
      27,    40,    47,     1,     5,    13,    47,     6,    42,    43,
      44,    45,    46,    14,   126,   127,     8,    78,    79,    74,
      59,    60,    61,    62,    63,    64,    65,    83,    63,    64,
      65,   140,     9,    11,    85,    86,    87,   143,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    12,    88,   102,   103,   104,   105,   106,   112,   108,
     109,   113,   114,   116,    13,    13,    47,    48,    49,   110,
     133,    13,    15,    28,    71,    51,    52,   111,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    75,   121,   128,    72,
      73,    76,    47,   124,    40,    77,    47,    80,    13,   118,
     107,   138,   115,   122,   141,   129,   131,   135,   136,    81,
     137,     7,   139,    47,    48,    49,    61,    62,    63,    64,
      65,   134,    51,    52,   125,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,   122,   144,     0,    47,    48,    49,   130,
       0,     0,     0,     0,    47,    51,    52,     0,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    30,    64,    65,    31,
      32,    17,    18,    19,    33,     0,     0,     0,     0,    21,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
      23,    47,    48,    49,     0,     0,     0,    24,    25,    50,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,    84,     0,     0,     0,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,   119,     0,     0,     0,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,   120,     0,     0,     0,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,   142,     0,     0,     0,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,     0,     0,     0,     0,     0,
      51,    52,     0,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,     0,     0,     0,     0,     0,
      51,    52,     0,     0,     0,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,     0,     0,     0,     0,     0,
      51,    52,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    47,    48,    49,     0,     0,     0,     0,     0,     0,
      51,    52,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,    64,    65,     0,     0,    68,    69,
      70,    47,    48,    49,     0,     0,     0,     0,     0,     0,
      51,    52,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,    63,    64,    65,    17,    18,    19,    20,
      70,     0,     0,     0,    21,     0,     0,    22,    47,    48,
      49,     0,     0,     0,     0,    23,     0,    51,    52,     0,
       0,     0,    24,    25,     0,     0,    59,    60,    61,    62,
      63,    64,    65
  };

  const short int
  BisonParser::yycheck_[] =
  {
      13,    19,    16,    11,    11,    23,    16,     0,    21,    22,
      23,    24,    25,    11,   119,   120,    19,    36,    37,    32,
      34,    35,    36,    37,    38,    39,    40,    40,    38,    39,
      40,   136,    11,    11,    47,    48,    49,   142,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    20,    50,    66,    67,    68,    69,    70,    77,    72,
      73,    80,    81,    13,    23,    23,    16,    17,    18,    28,
      28,    23,    21,    13,    11,    25,    26,    75,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    11,   110,   121,    19,
      19,    22,    16,   116,    19,    12,    16,    12,    23,   107,
      11,   134,    20,    13,   137,    11,     5,    19,     4,    12,
     133,     4,   135,    16,    17,    18,    36,    37,    38,    39,
      40,   129,    25,    26,   118,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    13,   143,    -1,    16,    17,    18,   124,
      -1,    -1,    -1,    -1,    16,    25,    26,    -1,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     3,    39,    40,     6,
       7,     8,     9,    10,    11,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    16,    17,    18,    -1,    -1,    -1,    34,    35,    24,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    44,
      45,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      35,    36,    37,    38,    39,    40,     8,     9,    10,    11,
      45,    -1,    -1,    -1,    16,    -1,    -1,    19,    16,    17,
      18,    -1,    -1,    -1,    -1,    27,    -1,    25,    26,    -1,
      -1,    -1,    34,    35,    -1,    -1,    34,    35,    36,    37,
      38,    39,    40
  };

  const unsigned char
  BisonParser::yystos_[] =
  {
       0,    11,    47,    48,    49,    11,     0,    48,    19,    11,
      50,    11,    20,    23,    56,    21,    52,     8,     9,    10,
      11,    16,    19,    27,    34,    35,    57,    64,    13,    51,
       3,     6,     7,    11,    53,    54,    58,    59,    61,    64,
      19,    56,    64,    64,    64,    64,    64,    16,    17,    18,
      24,    25,    26,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    11,    19,    19,    64,    11,    22,    12,    53,    53,
      12,    12,    62,    64,    20,    64,    64,    64,    56,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    11,    64,    64,
      28,    56,    53,    53,    53,    20,    13,    63,    56,    20,
      20,    64,    13,    55,    64,    51,    52,    52,    55,    11,
      63,     5,    60,    28,    56,    19,     4,    64,    55,    64,
      52,    55,    20,    52,    60
  };

  const unsigned char
  BisonParser::yyr1_[] =
  {
       0,    46,    47,    48,    48,    49,    50,    50,    51,    51,
      52,    53,    53,    53,    53,    53,    53,    54,    54,    55,
      55,    55,    56,    56,    57,    58,    59,    59,    60,    60,
      61,    62,    62,    63,    63,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64
  };

  const unsigned char
  BisonParser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     6,     4,     0,     5,     0,
       3,     3,     2,     2,     3,     3,     0,     4,     5,     4,
       5,     0,     4,     0,     4,     5,     6,     8,     6,     0,
       2,     2,     0,     3,     0,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     2,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BisonParser::yytname_[] =
  {
  "$end", "error", "$undefined", "TK_IF", "TK_ELSE", "TK_ELSEIF",
  "TK_WHILE", "TK_RETURN", "TK_STRING", "TK_INTEGER", "TK_FLOAT", "TK_ID",
  "TK_SEMICOLON", "TK_COMMA", "TK_DOUBLE_QUOTE", "TK_SINGLE_QUOTE",
  "TK_AMPERSAND", "TK_BIT_OR", "TK_BIT_XOR", "TK_L_PAREN", "TK_R_PAREN",
  "TK_L_BRACE", "TK_R_BRACE", "TK_L_SQBRACE", "TK_R_SQBRACE", "TK_AND",
  "TK_OR", "TK_NOT", "TK_ASSIGN", "TK_COMPARE", "TK_LT", "TK_LTE", "TK_GT",
  "TK_GTE", "TK_PLUS", "TK_MINUS", "TK_TIMES", "TK_DIV", "TK_MOD",
  "TK_DPLUS", "TK_DMINUS", "TK_PLUS_EQUAL", "TK_MINUS_EQUAL",
  "TK_TIMES_EQUAL", "TK_DIV_EQUAL", "TK_MOD_EQUAL", "$accept", "MODULE",
  "FUNCTIONS", "FUNCTION", "ARGS", "MARGS", "CODE_BLOCK", "CODE",
  "VAR_DECL", "MULT_VARS_DECL", "MAT_DIMS", "FUN_CALL", "LOOP", "COND",
  "COND_ELSES", "RETURN", "FORM_PARAMS", "MFORM_PARAMS", "EXPR", YY_NULLPTR
  };


  const unsigned char
  BisonParser::yyrline_[] =
  {
       0,   149,   149,   152,   153,   156,   159,   160,   163,   164,
     167,   169,   170,   171,   172,   173,   174,   177,   178,   181,
     182,   183,   186,   187,   190,   193,   196,   197,   200,   201,
     204,   207,   208,   211,   212,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247
  };

  // Print the state stack on the debug stream.
  void
  BisonParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  BisonParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  BisonParser::token_number_type
  BisonParser::yytranslate_ (int t)
  {
    static
    const token_number_type
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
      45
    };
    const unsigned int user_token_number_max_ = 300;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 5 "Parsin.yy" // lalr1.cc:1155
} // Parser
#line 1466 "Parsin.tab.cc" // lalr1.cc:1155
#line 250 "Parsin.yy" // lalr1.cc:1156



