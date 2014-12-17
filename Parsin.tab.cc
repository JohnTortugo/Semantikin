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
#line 29 "Parsin.yy" // lalr1.cc:399

	#include <cstdio>
	#include <iostream>
	#include <memory>
	#include <string>
	#include <iomanip>
	#include "Driver.h"
	#include "FlexScanner.h"

	static int findPos(const std::string& msg, int virtPos) {
		int actPos = 0;

		for (int i=0; i<virtPos; i++) {
			if (msg[i] == '\t') actPos += 4;
			else actPos++;
		}

		return actPos;
	}

	void Parser::BisonParser::error(const location_type& loc, const std::string& msg) {
		int beg = findPos(scanner.currentLine(), loc.begin.column) - 1;
		int end = findPos(scanner.currentLine(), loc.end.column);
		int dif = end - beg;
		
		std::cerr << loc.begin.line << ":" << loc.begin.column << "-" << loc.end.column << ": " << msg << endl;
		std::cerr << scanner.currentLine() << endl;
		std::cerr << std::setw(beg) << std::setfill('.') << "" << std::setw(dif) << std::setfill('^') << "" << endl;
	}
	
	static int yylex(Parser::BisonParser::semantic_type *tokenVal, Parser::BisonParser::location_type *location, Parser::FlexScanner &scanner, Parser::Driver &driver) {
		return scanner.yylex(tokenVal, location);	
	}

#line 71 "Parsin.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Parsin.tab.hh"

// User implementation prologue.

#line 85 "Parsin.tab.cc" // lalr1.cc:407


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
#line 171 "Parsin.tab.cc" // lalr1.cc:474

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  BisonParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
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
              // Fall through.
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
#line 175 "Parsin.yy" // lalr1.cc:847
    { module = new CompilationUnit((yystack_[0].value.listFuncs)); }
#line 622 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 178 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listFuncs))->push_front(std::shared_ptr<Function>((yystack_[1].value.function))); (yylhs.value.listFuncs) = (yystack_[0].value.listFuncs); }
#line 628 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 179 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listFuncs) = new list<std::shared_ptr<Function>>(); }
#line 634 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 182 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.function) = new Function((yystack_[5].value.str), (yystack_[4].value.str), shared_ptr<list<shared_ptr<ParamDecl>>>((yystack_[2].value.formParams)), (yystack_[0].value.codeBlock)); }
#line 640 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 185 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.formParams))->push_front(std::shared_ptr<ParamDecl>(new ParamDecl((yystack_[3].value.str), (yystack_[2].value.str), shared_ptr<list<shared_ptr<Expression>>>((yystack_[1].value.listExpr))))); (yylhs.value.formParams) = (yystack_[0].value.formParams); }
#line 646 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 186 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.formParams) = new list<std::shared_ptr<ParamDecl>>(); }
#line 652 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 189 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.formParams))->push_front(std::shared_ptr<ParamDecl>(new ParamDecl((yystack_[3].value.str), (yystack_[2].value.str), shared_ptr<list<shared_ptr<Expression>>>((yystack_[1].value.listExpr))))); (yylhs.value.formParams) = (yystack_[0].value.formParams); }
#line 658 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 190 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.formParams) = new list<std::shared_ptr<ParamDecl>>(); }
#line 664 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 193 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.codeBlock) = new CodeBlock((yystack_[1].value.listStmts)); }
#line 670 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 195 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[2].value.varDecl))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 676 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 196 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[1].value.loopStmt))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 682 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 197 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[1].value.ifStmt))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 688 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 198 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[2].value.ret))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 694 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 199 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listStmts))->push_front(std::shared_ptr<Statement>((yystack_[2].value.expr))); (yylhs.value.listStmts) = (yystack_[0].value.listStmts); }
#line 700 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 200 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listStmts) = new list<std::shared_ptr<Statement>>(); }
#line 706 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 203 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[2].value.str), (yystack_[1].value.listExpr), nullptr))); (yylhs.value.varDecl) = new VarDecl((yystack_[3].value.str), (yystack_[0].value.listVarSpec)); }
#line 712 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 204 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[3].value.str), nullptr, (yystack_[1].value.expr)))); (yylhs.value.varDecl) = new VarDecl((yystack_[4].value.str), (yystack_[0].value.listVarSpec)); }
#line 718 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 207 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[2].value.str), (yystack_[1].value.listExpr), nullptr))); (yylhs.value.listVarSpec) = (yystack_[0].value.listVarSpec); }
#line 724 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 208 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listVarSpec))->push_front(std::shared_ptr<VarSpec>(new VarSpec((yystack_[3].value.str), nullptr, (yystack_[1].value.expr)))); (yylhs.value.listVarSpec) = (yystack_[0].value.listVarSpec); }
#line 730 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 209 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listVarSpec) = new list<std::shared_ptr<VarSpec>>(); }
#line 736 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 212 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listExpr))->push_front(std::shared_ptr<Expression>((yystack_[2].value.expr))); (yylhs.value.listExpr) = (yystack_[0].value.listExpr); }
#line 742 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 213 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listExpr) = new list<std::shared_ptr<Expression>>(); }
#line 748 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 216 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.funCall) = new FunctionCall((yystack_[3].value.str), (yystack_[1].value.listExpr)); }
#line 754 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 25:
#line 219 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.loopStmt) = new LoopStmt((yystack_[2].value.expr), (yystack_[0].value.codeBlock)); }
#line 760 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 26:
#line 222 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.ifStmt) = new IfStmt((yystack_[3].value.expr), (yystack_[1].value.codeBlock), nullptr, (yystack_[0].value.listElseIf)); }
#line 766 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 27:
#line 223 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.ifStmt) = new IfStmt((yystack_[5].value.expr), (yystack_[3].value.codeBlock), (yystack_[0].value.codeBlock), (yystack_[2].value.listElseIf)); }
#line 772 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 28:
#line 226 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listElseIf))->push_front(std::shared_ptr<ElseIfStmt>(new ElseIfStmt((yystack_[3].value.expr), (yystack_[1].value.codeBlock)))); (yylhs.value.listElseIf) = (yystack_[0].value.listElseIf); }
#line 778 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 29:
#line 227 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listElseIf) = new list<std::shared_ptr<ElseIfStmt>>(); }
#line 784 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 30:
#line 230 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.ret) = new ReturnStmt((yystack_[0].value.expr)); }
#line 790 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 31:
#line 233 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listExpr))->push_front(std::shared_ptr<Expression>((yystack_[1].value.expr))); (yylhs.value.listExpr) = (yystack_[0].value.listExpr); }
#line 796 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 32:
#line 234 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listExpr) = new list<std::shared_ptr<Expression>>(); }
#line 802 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 33:
#line 237 "Parsin.yy" // lalr1.cc:847
    { ((yystack_[0].value.listExpr))->push_front(std::shared_ptr<Expression>((yystack_[1].value.expr))); (yylhs.value.listExpr) = (yystack_[0].value.listExpr); }
#line 808 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 34:
#line 238 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.listExpr) = new list<std::shared_ptr<Expression>>(); }
#line 814 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 35:
#line 241 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::COMPARE, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 820 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 36:
#line 242 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::DIFFERENCE, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 826 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 37:
#line 243 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::ASSIGN, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 832 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 38:
#line 244 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LOG_AND, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 838 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 39:
#line 245 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LOG_OR, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 844 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 40:
#line 246 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::BIT_AND, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 850 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 41:
#line 247 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::BIT_OR, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 856 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 42:
#line 248 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::BIT_XOR, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 862 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 43:
#line 249 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LT, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 868 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 44:
#line 250 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::LTE, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 874 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 45:
#line 251 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::GT, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 880 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 46:
#line 252 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::GTE, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 886 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 47:
#line 253 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::ADDITION, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 892 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 48:
#line 254 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::SUBTRACTION, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 898 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 49:
#line 255 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::TIMES, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 904 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 50:
#line 256 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::DIV, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 910 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 51:
#line 257 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::MOD, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 916 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 52:
#line 258 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::PLUS_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 922 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 53:
#line 259 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::MINUS_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 928 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 54:
#line 260 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::TIMES_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 934 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 55:
#line 261 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::DIV_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 940 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 56:
#line 262 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new BinaryExpr(BinaryExpr::MOD_EQUAL, (yystack_[2].value.expr), (yystack_[0].value.expr)); }
#line 946 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 57:
#line 263 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = (yystack_[1].value.expr); }
#line 952 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 58:
#line 264 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::BIT_NOT, (yystack_[0].value.expr)); }
#line 958 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 59:
#line 265 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::NOT, (yystack_[0].value.expr)); }
#line 964 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 60:
#line 266 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::MINUS, (yystack_[0].value.expr)); }
#line 970 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 61:
#line 267 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::PLUS, (yystack_[0].value.expr)); }
#line 976 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 62:
#line 268 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::ADDR, (yystack_[0].value.expr)); }
#line 982 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 63:
#line 269 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::INCREMENT, (yystack_[1].value.expr)); }
#line 988 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 64:
#line 270 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new UnaryExpr(UnaryExpr::DECREMENT, (yystack_[1].value.expr)); }
#line 994 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 65:
#line 271 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new IntegerExpr((yystack_[0].value.integer)); }
#line 1000 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 66:
#line 272 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new FloatExpr((yystack_[0].value.floating)); }
#line 1006 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 67:
#line 273 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new StringExpr((yystack_[0].value.str)); }
#line 1012 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 68:
#line 274 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = new IdentifierExpr((yystack_[1].value.str), (yystack_[0].value.listExpr)); }
#line 1018 "Parsin.tab.cc" // lalr1.cc:847
    break;

  case 69:
#line 275 "Parsin.yy" // lalr1.cc:847
    { (yylhs.value.expr) = (yystack_[0].value.funCall); }
#line 1024 "Parsin.tab.cc" // lalr1.cc:847
    break;


#line 1028 "Parsin.tab.cc" // lalr1.cc:847
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
  BisonParser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char BisonParser::yypact_ninf_ = -123;

  const signed char BisonParser::yytable_ninf_ = -1;

  const short int
  BisonParser::yypact_[] =
  {
      -7,     6,    31,  -123,    -7,    13,  -123,  -123,    23,    24,
      18,    29,    32,   573,    48,    67,  -123,  -123,  -123,  -123,
     -17,   573,   573,   573,   573,   573,   573,  -123,   216,    54,
    -123,    46,    47,   573,    68,    57,    69,    67,    67,    70,
     117,   573,  -123,   -11,   -26,   247,   -26,   445,   445,   573,
     573,   573,    29,   573,   573,   573,   573,   573,   573,   573,
     573,   573,   573,   573,   573,   573,  -123,  -123,   573,   573,
     573,   573,   573,   573,    58,   573,   573,   371,    -8,  -123,
      67,  -123,  -123,    67,    67,    64,   151,  -123,   -11,   513,
     539,  -123,   487,    80,   461,   579,   607,   607,   590,   590,
     445,   445,   -11,   -11,   -11,   433,   433,   402,   402,   371,
     579,    29,   278,   309,   573,    73,  -123,  -123,  -123,  -123,
     573,  -123,    48,    32,    32,   185,    78,  -123,   151,  -123,
      86,  -123,  -123,     3,  -123,    74,    89,   573,    73,   573,
      32,   185,  -123,   340,  -123,  -123,    32,    86,  -123
  };

  const unsigned char
  BisonParser::yydefact_[] =
  {
       4,     0,     0,     2,     4,     0,     1,     3,     7,     0,
       0,    23,     0,     0,     9,    16,     5,    67,    65,    66,
      23,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       6,     0,     0,     0,    23,     0,     0,    16,    16,     0,
       0,    32,    68,    62,    58,     0,    59,    61,    60,     0,
       0,     0,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    30,    23,    10,
      16,    12,    13,    16,    16,     0,    34,    57,    40,    41,
      42,    22,    38,    39,    37,    35,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      36,    23,     0,     0,     0,    21,    11,    14,    15,    24,
       0,    31,     9,     0,     0,    21,     0,    17,    34,     8,
      29,    25,    18,    23,    33,     0,    26,     0,    21,     0,
       0,    21,    19,     0,    27,    20,     0,    29,    28
  };

  const signed char
  BisonParser::yypgoto_[] =
  {
    -123,  -123,    93,  -123,  -123,   -22,  -122,   -12,  -123,  -119,
      12,  -123,  -123,  -123,   -43,  -123,  -123,   -23,   -13
  };

  const short int
  BisonParser::yydefgoto_[] =
  {
      -1,     2,     3,     4,    10,    30,    16,    35,    36,   127,
      42,    27,    37,    38,   136,    39,    85,   121,    40
  };

  const unsigned char
  BisonParser::yytable_[] =
  {
      28,   130,   131,    41,     1,    49,   132,    13,    43,    44,
      45,    46,    47,    48,    66,    67,    13,     5,   144,   142,
      77,   114,   145,    14,   147,    81,    82,    13,    86,    66,
      67,     6,   137,     8,     9,    11,    88,    89,    90,    12,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    13,    15,   105,   106,   107,   108,   109,
     110,    29,   112,   113,    91,    74,    75,    76,   116,   111,
      31,   117,   118,    32,    33,    17,    18,    19,    34,    78,
      79,    80,    83,    21,    22,   119,   126,    23,    41,   133,
     115,   135,    13,   140,   139,    24,    49,     7,    50,    51,
     129,   125,    25,    26,   148,   134,    53,   128,     0,     0,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,     0,   122,   141,     0,   143,     0,    73,    84,
       0,     0,     0,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    53,    54,   138,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,   120,    73,     0,    49,     0,    50,
      51,     0,     0,     0,     0,     0,     0,    53,    54,     0,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,   126,    73,
       0,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    53,    54,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    49,    73,    50,    51,     0,     0,     0,     0,
       0,    52,    53,    54,     0,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    49,    73,    50,    51,     0,    87,     0,
       0,     0,     0,    53,    54,     0,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    49,    73,    50,    51,     0,   123,
       0,     0,     0,     0,    53,    54,     0,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    49,    73,    50,    51,     0,
     124,     0,     0,     0,     0,    53,    54,     0,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    49,    73,    50,    51,
       0,   146,     0,     0,     0,     0,    53,    54,     0,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    49,    73,    50,
      51,     0,     0,     0,     0,     0,     0,    53,    54,     0,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    49,    73,
      50,    51,     0,     0,     0,     0,     0,     0,    53,    54,
       0,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,     0,    49,
      73,    50,    51,     0,     0,     0,     0,     0,     0,    53,
      54,    49,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    49,     0,    50,
      51,    73,    63,    64,    65,    66,    67,    53,    54,     0,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    49,     0,    50,    51,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    49,
       0,     0,    51,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    49,     0,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    17,    18,    19,    20,     0,     0,    73,     0,    21,
      22,     0,     0,    23,     0,    49,     0,     0,     0,     0,
       0,    24,     0,     0,     0,     0,    49,     0,    25,    26,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    57,    58,    49,     0,    61,    62,    63,    64,    65,
      66,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,    65,    66,    67
  };

  const short int
  BisonParser::yycheck_[] =
  {
      13,   123,   124,    20,    11,    16,   125,    24,    21,    22,
      23,    24,    25,    26,    40,    41,    24,    11,   140,   138,
      33,    29,   141,    11,   146,    37,    38,    24,    41,    40,
      41,     0,    29,    20,    11,    11,    49,    50,    51,    21,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    24,    22,    68,    69,    70,    71,    72,
      73,    13,    75,    76,    52,    11,    20,    20,    80,    11,
       3,    83,    84,     6,     7,     8,     9,    10,    11,    11,
      23,    12,    12,    16,    17,    21,    13,    20,    20,    11,
      78,     5,    24,     4,    20,    28,    16,     4,    18,    19,
     122,   114,    35,    36,   147,   128,    26,   120,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,   111,   137,    -1,   139,    -1,    48,    12,
      -1,    -1,    -1,    16,    -1,    18,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,   133,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    13,    48,    -1,    16,    -1,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    13,    48,
      -1,    16,    -1,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    16,    48,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    16,    48,    18,    19,    -1,    21,    -1,
      -1,    -1,    -1,    26,    27,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    16,    48,    18,    19,    -1,    21,
      -1,    -1,    -1,    -1,    26,    27,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    16,    48,    18,    19,    -1,
      21,    -1,    -1,    -1,    -1,    26,    27,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    16,    48,    18,    19,
      -1,    21,    -1,    -1,    -1,    -1,    26,    27,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    16,    48,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    16,    48,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    16,
      48,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    16,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    16,    -1,    18,
      19,    48,    37,    38,    39,    40,    41,    26,    27,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    16,    -1,    18,    19,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    16,
      -1,    -1,    19,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    16,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,     8,     9,    10,    11,    -1,    -1,    48,    -1,    16,
      17,    -1,    -1,    20,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    16,    -1,    35,    36,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    31,    32,    16,    -1,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    37,    38,    39,    40,    41
  };

  const unsigned char
  BisonParser::yystos_[] =
  {
       0,    11,    50,    51,    52,    11,     0,    51,    20,    11,
      53,    11,    21,    24,    59,    22,    55,     8,     9,    10,
      11,    16,    17,    20,    28,    35,    36,    60,    67,    13,
      54,     3,     6,     7,    11,    56,    57,    61,    62,    64,
      67,    20,    59,    67,    67,    67,    67,    67,    67,    16,
      18,    19,    25,    26,    27,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    48,    11,    20,    20,    67,    11,    23,
      12,    56,    56,    12,    12,    65,    67,    21,    67,    67,
      67,    59,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    11,    67,    67,    29,    59,    56,    56,    56,    21,
      13,    66,    59,    21,    21,    67,    13,    58,    67,    54,
      55,    55,    58,    11,    66,     5,    63,    29,    59,    20,
       4,    67,    58,    67,    55,    58,    21,    55,    63
  };

  const unsigned char
  BisonParser::yyr1_[] =
  {
       0,    49,    50,    51,    51,    52,    53,    53,    54,    54,
      55,    56,    56,    56,    56,    56,    56,    57,    57,    58,
      58,    58,    59,    59,    60,    61,    62,    62,    63,    63,
      64,    65,    65,    66,    66,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67
  };

  const unsigned char
  BisonParser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     6,     4,     0,     5,     0,
       3,     3,     2,     2,     3,     3,     0,     4,     5,     4,
       5,     0,     4,     0,     4,     5,     6,     8,     6,     0,
       2,     2,     0,     3,     0,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     1,     2,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const BisonParser::yytname_[] =
  {
  "$end", "error", "$undefined", "TK_IF", "TK_ELSE", "TK_ELSEIF",
  "TK_WHILE", "TK_RETURN", "TK_STRING", "TK_INTEGER", "TK_FLOAT", "TK_ID",
  "TK_SEMICOLON", "TK_COMMA", "TK_DOUBLE_QUOTE", "TK_SINGLE_QUOTE",
  "TK_AMPERSAND", "TK_BIT_NOT", "TK_BIT_OR", "TK_BIT_XOR", "TK_L_PAREN",
  "TK_R_PAREN", "TK_L_BRACE", "TK_R_BRACE", "TK_L_SQBRACE", "TK_R_SQBRACE",
  "TK_AND", "TK_OR", "TK_NOT", "TK_ASSIGN", "TK_COMPARE", "TK_LT",
  "TK_LTE", "TK_GT", "TK_GTE", "TK_PLUS", "TK_MINUS", "TK_TIMES", "TK_DIV",
  "TK_MOD", "TK_DPLUS", "TK_DMINUS", "TK_PLUS_EQUAL", "TK_MINUS_EQUAL",
  "TK_TIMES_EQUAL", "TK_DIV_EQUAL", "TK_MOD_EQUAL", "TK_BIT_AND",
  "TK_DIFFERENCE", "$accept", "MODULE", "FUNCTIONS", "FUNCTION", "ARGS",
  "MARGS", "CODE_BLOCK", "CODE", "VAR_DECL", "MULT_VARS_DECL", "MAT_DIMS",
  "FUN_CALL", "LOOP", "COND", "COND_ELSES", "RETURN", "FORM_PARAMS",
  "MFORM_PARAMS", "EXPR", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  BisonParser::yyrline_[] =
  {
       0,   175,   175,   178,   179,   182,   185,   186,   189,   190,
     193,   195,   196,   197,   198,   199,   200,   203,   204,   207,
     208,   209,   212,   213,   216,   219,   222,   223,   226,   227,
     230,   233,   234,   237,   238,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275
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
      45,    46,    47,    48
    };
    const unsigned int user_token_number_max_ = 303;
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
#line 1643 "Parsin.tab.cc" // lalr1.cc:1155
#line 278 "Parsin.yy" // lalr1.cc:1156



