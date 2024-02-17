%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {zpp}
%define api.parser.class {Parser}

%code requires {
#include "ast/Program.h"

namespace zpp {
class Scanner;
}
}

%parse-param {Scanner &scanner}
%parse-param {std::unique_ptr<ast::Program> &program}

%code {
#include <iostream>
#include <fstream>
#include "ZppScanner.h"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token END 0 "end of file"
%token <std::string> ID
%token <unsigned long> INT10
%token PLUS MINUS STAR SLASH
%token LPAREN RPAREN LBRACE RBRACE
%token SEMI COMMA
%token RETURN VERY LONG NOT AT ALL MEDIUM
%token UNKNOWN

%locations

%type <std::unique_ptr<ast::Program>> topLevel
%type <std::unique_ptr<ast::Declaration>> declaration functionDeclaration
%type <std::unique_ptr<ast::StatementBlock>> statementList
%type <std::unique_ptr<ast::Statement>> statement returnStatement
%type <std::unique_ptr<ast::Expression>> expression baseExpression
%type <std::unique_ptr<ast::Type>> type
%type <std::unique_ptr<ast::ParameterList>> parameterList
%type <unsigned int> longType

%left PLUS MINUS
%left STAR SLASH

%%

program : topLevel END { program = std::move($1); }
        ;

topLevel : %empty { $$ = std::make_unique<ast::Program>(); }
         | topLevel declaration { $1->addDeclaration(std::move($2)); $$ = std::move($1); }
         ;

declaration : functionDeclaration { $$ = std::move($1); }
            ;

functionDeclaration : type ID LPAREN RPAREN SEMI { $$ = std::make_unique<ast::FunctionDeclaration>(std::move($1), $2, std::make_unique<ast::ParameterList>()); }
                    | type ID LPAREN parameterList RPAREN SEMI { $$ = std::make_unique<ast::FunctionDeclaration>(std::move($1), $2, std::move($4)); }
                    | type ID LPAREN RPAREN LBRACE statementList RBRACE { $$ = std::make_unique<ast::FunctionDefinition>(std::move($1), $2, std::make_unique<ast::ParameterList>(), std::move($6)); }
                    | type ID LPAREN parameterList RPAREN LBRACE statementList RBRACE { $$ = std::make_unique<ast::FunctionDefinition>(std::move($1), $2, std::move($4), std::move($7)); }
                    ;

parameterList : type { $$ = std::make_unique<ast::ParameterList>(std::string(), std::move($1)); }
              | type ID { $$ = std::make_unique<ast::ParameterList>($2, std::move($1)); }
              | parameterList COMMA type { $1->addParameter(std::string(), std::move($3)); $$ = std::move($1); }
              | parameterList COMMA type ID { $1->addParameter($4, std::move($3)); $$ = std::move($1); }
              ;

statement : expression SEMI { $$ = std::make_unique<ast::ExpressionStatement>(std::move($1)); }
          | returnStatement { $$ = std::move($1); }
          | LBRACE statementList RBRACE { $$ = std::move($2); }
          ;

returnStatement : RETURN SEMI { $$ = std::make_unique<ast::ReturnStatement>(nullptr); }
                | RETURN expression SEMI { $$ = std::make_unique<ast::ReturnStatement>(std::move($2)); }
                ;

statementList : %empty { $$ = std::make_unique<ast::StatementBlock>(); }
              | statementList statement { $1->addStatement(std::move($2)); $$ = std::move($1); }
              ;

expression : expression PLUS expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Add); }
           | expression MINUS expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Sub); }
           | expression STAR expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Mul); }
           | expression SLASH expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Div); }
           | baseExpression { $$ = std::move($1); }
           ;

baseExpression : INT10 { $$ = std::make_unique<ast::IntegerLiteral>($1); }
               | ID { $$ = std::make_unique<ast::Identifier>($1); }
               | LPAREN expression RPAREN { $$ = std::move($2); }
               ;

type : NOT LONG AT ALL { $$ = std::make_unique<ast::IntegerType>(8); }
     | NOT VERY LONG { $$ = std::make_unique<ast::IntegerType>(16); }
     | MEDIUM LONG { $$ = std::make_unique<ast::IntegerType>(32); }
     | longType { $$ = std::make_unique<ast::IntegerType>($1); }
     ;

longType : LONG { $$ = 64; }
         | VERY longType { $$ = $2 + 64; }
         ;

%%

void zpp::Parser::error(const location_type &l, const std::string &err) {
  std::cerr << "Error: " << l << ": " << err << std::endl;
}