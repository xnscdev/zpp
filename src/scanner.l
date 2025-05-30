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
"="         return token::EQUAL;
"("         return token::LPAREN;
")"         return token::RPAREN;
"{"         return token::LBRACE;
"}"         return token::RBRACE;
";"         return token::SEMI;
","         return token::COMMA;
"..."       return token::ELLIPSIS;
"return"    return token::RETURN;
"void"      return token::VOID;
"very"      return token::VERY;
"long"      return token::LONG;
"not"       return token::NOT;
"at"        return token::AT;
"all"       return token::ALL;
"medium"    return token::MEDIUM;
[a-zA-Z_][a-zA-Z0-9_]*      yylval->emplace<std::string>(yytext); return token::ID;
[1-9][0-9]*[lL]*            yylval->emplace<std::string>(yytext); return token::INT10;
0[0-7]*[lL]*                yylval->emplace<std::string>(yytext); return token::INT8;
0[xX][0-9a-fA-F]+[lL]*      { std::string s(yytext); s.erase(0, 2); yylval->emplace<std::string>(s); return token::INT16; }
\"(\\.|[^"\\])*\"           { std::string s(yytext); s.erase(s.begin()); s.erase(s.end() - 1); yylval->emplace<std::string>(s); return token::STR; }
.           return token::UNKNOWN;

%%