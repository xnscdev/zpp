%{

#include <string>
#include "ZppScanner.h"

#undef YY_DECL
#define YY_DECL int zpp::Scanner::yylex(zpp::Parser::semantic_type *const lval, zpp::Parser::location_type *location)

using token = zpp::Parser::token;

#define yyterminate() return token::END

#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="zpp::Scanner"
%option noyywrap
%option c++

%%

%{
yylval = lval;
%}

[ \t\v\r]   ;
\n          loc->lines();
"+"         return token::PLUS;
"-"         return token::MINUS;
"*"         return token::STAR;
"/"         return token::SLASH;
"("         return token::LPAREN;
")"         return token::RPAREN;
"{"         return token::LBRACE;
"}"         return token::RBRACE;
";"         return token::SEMI;
","         return token::COMMA;
"return"    return token::RETURN;
"very"      return token::VERY;
"long"      return token::LONG;
"not"       return token::NOT;
"at"        return token::AT;
"all"       return token::ALL;
"medium"    return token::MEDIUM;
[a-zA-Z_][a-zA-Z0-9_]*      yylval->emplace<std::string>(yytext); return token::ID;
[1-9][0-9]*                 yylval->emplace<unsigned long>(std::stoul(yytext)); return token::INT10;
.           return token::UNKNOWN;

%%