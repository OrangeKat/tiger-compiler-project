                                                                // -*- C++ -*-
%require "3.8"
%language "c++"
// Set the namespace name to `parse', instead of `yy'.
%define api.prefix {parse}
%define api.namespace {parse}
%define api.parser.class {parser}
%define api.value.type variant
%define api.token.constructor

// We use a GLR parser because it is able to handle Shift-Reduce and
// Reduce-Reduce conflicts by forking and testing each way at runtime. GLR
// permits, by allowing few conflicts, more readable and maintainable grammars.
%glr-parser
%skeleton "glr2.cc"

// In TC, we expect the GLR to resolve one Shift-Reduce and zero Reduce-Reduce
// conflict at runtime. Use %expect and %expect-rr to tell Bison about it.
  // FIXME: Some code was deleted here (Other directives).
%expect 1
%expect-rr 0

%define parse.error verbose
%defines
%debug
// Prefix all the tokens with TOK_ to avoid colisions.
%define api.token.prefix {TOK_}

/* We use pointers to store the filename in the locations.  This saves
   space (pointers), time (no deep copy), but leaves the problem of
   deallocation.  This would be a perfect job for a misc::symbol
   object (passed by reference), however Bison locations require the
   filename to be passed as a pointer, thus forcing us to handle the
   allocation and deallocation of this object.

   Nevertheless, all is not lost: we can still use a misc::symbol
   object to allocate a flyweight (constant) string in the pool of
   symbols, extract it from the misc::symbol object, and use it to
   initialize the location.  The allocated data will be freed at the
   end of the program (see the documentation of misc::symbol and
   misc::unique).  */
%define api.filename.type {const std::string}
%locations

/*---------------------.
| Support for tokens.  |
`---------------------*/
%code requires
{
#include <string>
#include <misc/algorithm.hh>
#include <misc/separator.hh>
#include <misc/symbol.hh>
#include <parse/fwd.hh>

  // Pre-declare parse::parse to allow a ``reentrant'' parsing within
  // the parser.
  namespace parse
  {
    ast_type parse(Tweast& input);
  }
}

// The parsing context.
%param { ::parse::TigerDriver& td }
%parse-param { ::parse::Lexer& lexer }

%printer { yyo << $$; } <int> <std::string> <misc::symbol>;

%token <std::string>    STRING "string"
%token <misc::symbol>   ID     "identifier"
%token <int>            INT    "integer"


/*--------------------------------.
| Support for the non-terminals.  |
`--------------------------------*/

%code requires
{
# include <ast/fwd.hh>
// Provide the declarations of the following classes for the
// %destructor clauses below to work properly.
# include <ast/exp.hh>
# include <ast/var.hh>
# include <ast/ty.hh>
# include <ast/name-ty.hh>
# include <ast/field.hh>
# include <ast/field-init.hh>
# include <ast/function-dec.hh>
# include <ast/type-dec.hh>
# include <ast/var-dec.hh>
# include <ast/chunk.hh>
# include <ast/chunk-list.hh>
}

  // FIXME: Some code was deleted here (Printers and destructors).

%destructor { free ($$); } varchunk
%destructor { free ($$); } funchunk
%destructor { free ($$); } tychunk
%destructor { free ($$); } fundec
%destructor { free ($$); } tydec
%destructor { free ($$); } vardec
%destructor { free ($$); } exps
%destructor { free ($$); } lvalue
%destructor { free ($$); } ty
%destructor { free ($$); } typeid
%destructor { free ($$); } chunks
%destructor { free ($$); } exp
%destructor { free ($$); } tyfield
%destructor { free ($$); } tyfields tyfields.1

/*-----------------------------------------.
| Code output in the implementation file.  |
`-----------------------------------------*/

%code
{
# include <parse/tweast.hh>
# include <misc/separator.hh>
# include <misc/symbol.hh>
# include <ast/all.hh>
# include <ast/libast.hh>
# include <parse/tiger-factory.hh>

  namespace
  {

    /// Get the metavar from the specified map.
    template <typename T>
    T*
    metavar(parse::TigerDriver& td, unsigned key)
    {
      parse::Tweast* input = td.input_;
      return input->template take<T>(key);
    }

  }
}

%code
{
  #include <parse/scantiger.hh>  // header file generated with reflex --header-file
  #undef yylex
  #define yylex lexer.lex  // Within bison's parse() we should invoke lexer.lex(), not the global lex()
}

// Definition of the tokens, and their pretty-printing.
%token AND          "&"
       ARRAY        "array"
       ASSIGN       ":="
       BREAK        "break"
       CAST         "_cast"
       CLASS        "class"
       COLON        ":"
       COMMA        ","
       DIVIDE       "/"
       DO           "do"
       DOT          "."
       ELSE         "else"
       END          "end"
       EQ           "="
       EXTENDS      "extends"
       FOR          "for"
       FUNCTION     "function"
       GE           ">="
       GT           ">"
       IF           "if"
       IMPORT       "import"
       IN           "in"
       LBRACE       "{"
       LBRACK       "["
       LE           "<="
       LET          "let"
       LPAREN       "("
       LT           "<"
       MINUS        "-"
       METHOD       "method"
       NE           "<>"
       NEW          "new"
       NIL          "nil"
       OF           "of"
       OR           "|"
       PLUS         "+"
       PRIMITIVE    "primitive"
       RBRACE       "}"
       RBRACK       "]"
       RPAREN       ")"
       SEMI         ";"
       THEN         "then"
       TIMES        "*"
       TO           "to"
       TYPE         "type"
       VAR          "var"
       WHILE        "while"
       EOF 0        "end of file"

%type <ast::Exp*>             exp
%type <ast::ChunkList*>       chunks classfields

%type <ast::TypeChunk*>       tychunk
%type <ast::TypeDec*>         tydec
%type <ast::NameTy*>          typeid
%type <ast::Ty*>              ty

%type <ast::Field*>           tyfield
%type <ast::fields_type*>     tyfields tyfields.1

  // FIXME: Some code was deleted here (More %types).
%type <ast::FunctionChunk*>   funchunk
%type <ast::FunctionDec*>     fundec
%type <ast::VarChunk*>        varchunk
%type <ast::VarDec*>          vardec
%type <ast::Var*>             lvalue
%type <ast::exps_type*>       exps
%type <ast::fieldinits_type*> exp_list
%type <ast::exps_type*>       id_val
%type <ast::VarDec*>          funcfield
%type <ast::VarChunk*>        funcfields funcfields.1
%type <ast::MethodChunk*>     methodchunk
%type <ast::MethodDec*>       methoddec


  // FIXME: Some code was deleted here (Priorities/associativities).
%precedence THEN
%precedence ELSE DO OF ASSIGN
%left OR
%left AND
%nonassoc LT LE EQ NE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
// Solving conflicts on:
// let type foo = bar
//     type baz = bat
// which can be understood as a list of two TypeChunk containing
// a unique TypeDec each, or a single TypeChunk containing two TypeDec.
// We want the latter.
%precedence CHUNKS
%precedence TYPE
  // FIXME: Some code was deleted here (Other declarations).
%precedence VAR
%precedence FUNCTION
%precedence PRIMITIVE
%precedence CLASS
%precedence METHOD

%start program

%%
program:
  /* Parsing a source program.  */
  exp { td.ast_ = $1; }
| /* Parsing an imported file.  */
  chunks { td.ast_ = $1; }
;

%token EXP "_exp";
exp:
    INT                                 { $$ = make_IntExp(@$, $1); }
  // FIXME: Some code was deleted here (More rules).
  | STRING                              { $$ = make_StringExp(@$, $1); }
  | NIL                                 { $$ = make_NilExp(@$); }

  | ID "[" exp "]" "of" exp             { $$ = make_ArrayExp(@$, make_NameTy(@1, $1), $3, $6); }
  | typeid "{" "}"                      { $$ = make_RecordExp(@$, $1, make_fieldinits_type()); }
  | typeid "{" exp_list "}"             { $$ = make_RecordExp(@$, $1, $3); }

  | lvalue                              { $$ = $1; }

  | ID "(" ")"                          { $$ = make_CallExp(@$, $1, make_exps_type()); }
  | ID "(" id_val ")"                   { $$ = make_CallExp(@$, $1, $3); }

  | "-" exp                             { $$ = make_OpExp(@$, make_IntExp(@$, 0), ast::OpExp::Oper::sub, $2); }
  | exp "+" exp                         { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::add, $3); }
  | exp "-" exp                         { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::sub, $3); }
  | exp "*" exp                         { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::mul, $3); }
  | exp "/" exp                         { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::div, $3); }
  | exp "=" exp                         { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::eq, $3); }
  | exp "<>" exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::ne, $3); }
  | exp ">" exp                         { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::gt, $3); }
  | exp "<" exp                         { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::lt, $3); }
  | exp ">=" exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::ge, $3); }
  | exp "<=" exp                        { $$ = make_OpExp(@$, $1, ast::OpExp::Oper::le, $3); }
  | exp "&" exp                         { $$ = parse::parse(Tweast() << "if " << $1 << " then " << $3 << " <> 0 else 0 ");}
  | exp "|" exp                         { $$ = td.enable_extensions().parse(Tweast() << "if " << $1 << " then 1 else " << $3);}

  | "(" ")"                             { $$ = make_SeqExp(@$, make_exps_type(nullptr)); }
  | "(" exps ")"                        { $$ = make_SeqExp(@$, $2); }

  | lvalue ASSIGN exp                   { $$ = make_AssignExp(@$, $1, $3); }

  | "if" exp "then" exp                 { $$ = make_IfExp(@$, $2, $4); }
  | "if" exp "then" exp "else" exp      { $$ = make_IfExp(@$, $2, $4, $6); }
  | "while" exp "do" exp                { $$ = make_WhileExp(@$, $2, $4); }
  | "for" ID ":=" exp "to" exp "do" exp { $$ = make_ForExp(@$, make_VarDec(@2, $2, nullptr, $4), $6, $8); }
  | "break"                             { $$ = make_BreakExp(@$); }
  | "let" chunks "in" exps "end"        { $$ = make_LetExp(@$, $2, make_SeqExp(@$,$4)); }
  | "let" chunks "in" "end"             { $$ = make_LetExp(@$, $2, make_SeqExp(@$, make_exps_type(nullptr))); }

  | "new" typeid                        { $$ = make_ObjectExp(@$, $2); }
  | lvalue "." ID "(" ")"               { $$ = make_MethodCallExp(@$, $3, make_exps_type(nullptr), $1); }
  | lvalue "." ID "(" id_val ")"        { $$ = make_MethodCallExp(@$, $3, $5, $1); }

  | CAST "(" exp "," ty ")"             { $$ = make_CastExp(@$, $3, $5); }
  | EXP "(" INT ")"                     { $$ = metavar<ast::Exp>(td, $3); }
  ;

exps:
    exp                       { $$ = make_exps_type($1); }
  | exp ";" exps              { $$ = $3; $$->push_back($1);}
  ;

exp_list:
    ID "=" exp              { $$ = make_fieldinits_type(make_FieldInit(@$,$1,$3)); }
  | ID "=" exp "," exp_list { $$ = $5; $$->push_back(make_FieldInit(@$, $1, $3)); }
  ;

id_val:
    exp                     { $$ = make_exps_type($1);}
  | id_val "," exp          { $$ = $1; $$->push_back($3);}
  ;

%token LVALUE "_lvalue";
lvalue:
    ID                      { $$ = make_SimpleVar(@$, $1); }
  | lvalue "." ID           { $$ = make_FieldVar(@$, $1, $3); }
  | lvalue "[" exp "]"      { $$ = make_SubscriptVar(@$, $1, $3); }
  | LVALUE "(" INT ")"      { $$ = metavar<ast::Var>(td, $3); }
  ;


/*---------------.
| Declarations.  |
`---------------*/

%token CHUNKS "_chunks";
chunks:
  /* Chunks are contiguous series of declarations of the same type
     (TypeDec, FunctionDec...) to which we allow certain specfic behavior like
     self referencing.
     They are held by a ChunkList, that can be empty, like in this case:
        let
        in
            ..
        end
     which is why we end the recursion with a %empty. */
  %empty                    { $$ = make_ChunkList(@$); }
| tychunk   chunks          { $$ = $2; $$->push_front($1); }
  // FIXME: Some code was deleted here (More rules).
| funchunk  chunks          { $$ = $2; $$->push_front($1); }
| varchunk  chunks          { $$ = $2; $$->push_front($1); }

| CHUNKS "(" INT ")" chunks { $$ = $5; $$->splice_front(*metavar<ast::ChunkList>(td, $3)); }
| "import" STRING chunks    { $$ = $3; auto var = td.parse_import($2, @$); if (var != nullptr) { $$->splice_front(*var); }; }
;

/*--------------------.
| Type Declarations.  |
`--------------------*/

tychunk:
  /* Use `%prec CHUNKS' to do context-dependent precedence and resolve a
     shift-reduce conflict. */
  tydec %prec CHUNKS  { $$ = make_TypeChunk(@1); $$->push_front(*$1); }
| tydec tychunk       { $$ = $2; $$->push_front(*$1); }
;

varchunk:
  vardec %prec CHUNKS { $$ = make_VarChunk(@1); $$->push_front(*$1); }
;

funchunk:
  fundec %prec CHUNKS { $$ = make_FunctionChunk(@1); $$->push_front(*$1); }
| fundec funchunk     { $$ = $2; $$->push_front(*$1); }
;

tydec:
  "type" ID "=" ty                                  { $$ = make_TypeDec(@$, $2, $4); }
| "class" ID "{" classfields "}"                    { $$ = make_TypeDec(@$, $2, make_ClassTy(@$, nullptr, $4)); }
| "class" ID "extends" typeid "{" classfields "}"   { $$ = make_TypeDec(@$, $2, make_ClassTy(@$, $4, $6)); }
;

fundec:
  FUNCTION ID "(" funcfields ")" "=" exp              { $$ = make_FunctionDec(@$, $2, $4, nullptr, $7); }
| FUNCTION ID "(" funcfields ")" ":" typeid "=" exp   { $$ = make_FunctionDec(@$, $2, $4, $7, $9); }
| PRIMITIVE ID "(" funcfields ")"                     { $$ = make_FunctionDec(@$, $2, $4, nullptr, nullptr); }
| PRIMITIVE ID "(" funcfields ")" ":" typeid          { $$ = make_FunctionDec(@$, $2, $4, $7, nullptr); }
;

vardec:
  "var" ID ":=" exp             { $$ = make_VarDec(@$, $2, nullptr, $4); }
| "var" ID ":" typeid ":=" exp  { $$ = make_VarDec(@$, $2, $4, $6); }
;

ty:
  typeid                                    { $$ = $1; }
| "{" tyfields "}"                          { $$ = make_RecordTy(@$, $2); }
| "array" "of" typeid                       { $$ = make_ArrayTy(@$, $3); }
| "class" "{" classfields "}"                 { $$ = make_ClassTy(@$, nullptr, $3); }
| "class" "extends" typeid "{" classfields "}"  { $$ = make_ClassTy(@$, $3, $5); }
;

classfields:
  %empty                                { $$ = make_ChunkList(@$); }
| varchunk classfields                  { $$ = $2; $$->emplace_back($1); }
| methodchunk classfields               { $$ = $2; $$->emplace_back($1); }

methodchunk:
  methoddec %prec "class" { $$ = make_MethodChunk(@$); $$->emplace_back(*$1); }
| methoddec methodchunk   { $$ = $2; $$->emplace_back(*$1); }

methoddec:
  "method" ID "(" funcfields ")" "=" exp            { $$ = make_MethodDec(@$, $2, $4, nullptr, $7); }
| "method" ID "(" funcfields ")" ":" typeid "=" exp { $$ = make_MethodDec(@$, $2, $4, $7, $9); }
; 

tyfields:
  %empty               { $$ = make_fields_type(); }
| tyfields.1           { $$ = $1; }
;

tyfields.1:
  tyfields.1 "," tyfield { $$ = $1; $$->emplace_back($3); }
| tyfield                { $$ = make_fields_type($1); }
;

tyfield:
  ID ":" typeid     { $$ = make_Field(@$, $1, $3); }
;

funcfields:
  %empty               { $$ = make_VarChunk(@$); }
| funcfields.1           { $$ = $1; }
;

funcfields.1:
  funcfields.1 "," funcfield { $$ = $1; $$->emplace_back(*$3); }
| funcfield                { $$ = make_VarChunk(@$); $$->emplace_back(*$1); }
;

funcfield:
  ID ":" typeid     { $$ = make_VarDec(@$,$1,$3,nullptr); }
;

%token NAMETY "_namety";
typeid:
  ID                    { $$ = make_NameTy(@$, $1); }
  /* This is a metavariable. It it used internally by TWEASTs to retrieve
     already parsed nodes when given an input to parse. */
| NAMETY "(" INT ")"    { $$ = metavar<ast::NameTy>(td, $3); }
;

%%

void
parse::parser::error(const location_type& l, const std::string& m)
{
  // FIXME: Some code was deleted here.
  td.error_ << misc::error::error_type::parse << "Error: " << m << " at location " << l << std::endl;
  
}
