                                                            /* -*- C++ -*- */
// %option defines the parameters with which the reflex will be launched
%option noyywrap
// To enable compatibility with bison
%option bison-complete
%option bison-cc-parser=parser
%option bison_cc_namespace=parse
%option bison-locations

%option lex=lex
// Add a param of function lex() generate in Lexer class
%option params="::parse::TigerDriver& td"
%option namespace=parse
// Name of the class generate by reflex
%option lexer=Lexer

%top{

#define YY_EXTERN_C extern "C" // For linkage rule

#include <cerrno>
#include <climits>
#include <regex>
#include <string>

#include <boost/lexical_cast.hpp>

#include <misc/contract.hh>
  // Using misc::escape is very useful to quote non printable characters.
  // For instance
  //
  //    std::cerr << misc::escape('\n') << '\n';
  //
  // reports about `\n' instead of an actual new-line character.
#include <misc/escape.hh>
#include <misc/symbol.hh>
#include <parse/parsetiger.hh>
#include <parse/tiger-driver.hh>

  // FIXME: Some code was deleted here (Define YY_USER_ACTION to update locations).
#define YY_USER_ACTION  \                        
do { \                                
    td.location_.columns(size()); \              
} while (false);                                  

#define TOKEN(Type)                             \
  parser::make_ ## Type(td.location_)

#define TOKEN_VAL(Type, Value)                  \
  parser::make_ ## Type(Value, td.location_)

# define CHECK_EXTENSION()                              \
  do {                                                  \
    if (!td.enable_extensions_p_)                       \
      td.error_ << misc::error::error_type::scan        \
                << td.location_                         \
                << ": invalid identifier: `"            \
                << misc::escape(text()) << "'\n";       \
  } while (false)

# define CHECK_OBJ_EXTENSION()                          \
  do {                                                  \
    if (!td.enable_object_extensions_p_)                       \
      td.error_ << misc::error::error_type::scan        \
                << td.location_                         \
                << ": invalid identifier: `"            \
                << misc::escape(text()) << "'\n";       \
  } while (false)



%}

%x SC_COMMENT SC_STRING

/* Abbreviations.  */
int             [0-9]+
space           [\s\t]+
eol             [\r\n]+
id              ([a-zA-Z][a-zA-Z0-9_]*)|(_main)
reserved_id     (_[a-zA-Z0-9_]*)
  /* FIXME: Some code was deleted here. */

%class{
  // FIXME: Some code was deleted here (Local variables).
  std::string str;
  int len = 0;
}

%%
/* The rules.  */


"/*" {start(SC_COMMENT);len++;}
"\"" {str = ""; start(SC_STRING);}

<SC_STRING>{
\\a                            { str += "\a";}
\\b                            { str += "\b";}
\\f                            { str += "\f";}
\\n                            { str += "\n";}
\\r                            { str += "\r";}
\\t                            { str += "\t";}
\\v                            { str += "\v";}
\\\"                           { str += "\"";}
\\\\                           { str += "\\";}

\\[0-7]{3}  {
  if(strtol(text()+1,0,8) > 255 || strtol(text()+1,0,8) < 0)
  {
    td.error_ << misc::error::error_type::scan                                  \
                                << td.location_                                 \
                                << ": LEXER: error string \\num "               \
                                << misc::escape(text()) << "\n"; start(INITIAL);
  }
  else
  {
    str+= strtol(text()+1,0,8);
  }
}
\\x[0-9a-fA-F0]{2}  {
  str += strtol(text()+2,0,16);
}
"\""                           { start(INITIAL); return TOKEN_VAL(STRING, str); }
\\.                  {
                            td.error_ << misc::error::error_type::scan                                  \
                                                      << td.location_                                   \
                                                      << ": LEXER: error string \\. "     \
                                                      << misc::escape(text()) << "\n"; start(INITIAL);
                     }
<<EOF>>                         { td.error_ << misc::error::error_type::scan                             \
                                            << td.location_                                              \
                                            << ": LEXER: unexpected EOF "                                \
                                            << misc::escape("no end to multilines comment") << "\n"; start(INITIAL);}
.                   { str += text();}
}

<SC_COMMENT>{
"/*" {len++;}
"*/" {
    len--;
    if (len == 0)
    {start(INITIAL);}
}
{eol}       {}
<<EOF>>                         { td.error_ << misc::error::error_type::scan                \
                                            << td.location_                                 \
                                            << ": LEXER: unexpected EOF "                   \
                                            << misc::escape("comment error expected end of comment") << "\n"; start(INITIAL);}
.                               {}
}
{space}       {}
{eol}         {}
{int}         {
                int val = 0;
                try { val = std::stoi(text()); }
                catch (...)
                {
                     td.error_ << misc::error::error_type::scan        \
                              << td.location_                         \
                              << "LEXER : convertion error in rule int `"            \
                              << misc::escape(text()) << "'\n";       \
                }
                return TOKEN_VAL(INT, val);
              }
"&"             return TOKEN(AND);
"array"         return TOKEN(ARRAY);
":="            return TOKEN(ASSIGN);
"break"         return TOKEN(BREAK);
"_cast"         {CHECK_EXTENSION(); return TOKEN(CAST);}
"_exp"          {CHECK_EXTENSION(); return TOKEN(EXP);}
"_lvalue"       {CHECK_EXTENSION();return TOKEN(LVALUE);}
"_namety"       {CHECK_EXTENSION();return TOKEN(NAMETY);}
"_chunks"       {CHECK_EXTENSION();return TOKEN(CHUNKS);}
"class"         {CHECK_OBJ_EXTENSION(); return TOKEN(CLASS);}
":"             return TOKEN(COLON);
","             return TOKEN(COMMA);
"/"             return TOKEN(DIVIDE);
"do"            return TOKEN(DO);
"."             return TOKEN(DOT);
"else"          return TOKEN(ELSE);
"end"           return TOKEN(END);
"="             return TOKEN(EQ);
"extends"       {CHECK_OBJ_EXTENSION(); return TOKEN(EXTENDS);}
"for"           return TOKEN(FOR);
"function"      return TOKEN(FUNCTION);
">="            return TOKEN(GE);
">"             return TOKEN(GT);
"if"            return TOKEN(IF);
"import"        return TOKEN(IMPORT);
"in"            return TOKEN(IN);
"{"             return TOKEN(LBRACE);
"["             return TOKEN(LBRACK);
"<="            return TOKEN(LE);
"let"           return TOKEN(LET);
"("             return TOKEN(LPAREN);
"<"             return TOKEN(LT);
"-"             return TOKEN(MINUS);
"method"        {CHECK_OBJ_EXTENSION(); return TOKEN(METHOD);}
"<>"            return TOKEN(NE);
"new"           {CHECK_OBJ_EXTENSION(); return TOKEN(NEW);}
"nil"           return TOKEN(NIL);
"of"            return TOKEN(OF);
"|"             return TOKEN(OR);
"+"             return TOKEN(PLUS);
"primitive"     return TOKEN(PRIMITIVE);
"}"             return TOKEN(RBRACE);
"]"             return TOKEN(RBRACK);
")"             return TOKEN(RPAREN);
";"             return TOKEN(SEMI);
"then"          return TOKEN(THEN);
"*"             return TOKEN(TIMES);
"to"            return TOKEN(TO);
"type"          return TOKEN(TYPE);
"var"           return TOKEN(VAR);
"while"         return TOKEN(WHILE);
{id}            return TOKEN_VAL(ID,misc::symbol(text()));
{reserved_id}   {CHECK_EXTENSION(); return TOKEN_VAL(ID,misc::symbol(text()));}
<<EOF>>         return TOKEN(EOF);
.               {td.error_ << misc::error::error_type::scan        \
                           << td.location_                         \
                           << ": LEXER: invalid character in RULES "             \
                           << misc::escape(text()) << "\n";}

// FIXME: Some code was deleted here (Decode, and check the value).
/* FIXME: Some code was deleted here. */
%%



