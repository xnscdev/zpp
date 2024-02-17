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
%token <std::string> INT10 INT8 INT16
%token PLUS MINUS STAR SLASH EQUAL
%token LPAREN RPAREN LBRACE RBRACE
%token SEMI COMMA ELLIPSIS
%token RETURN
%token VOID VERY LONG NOT AT ALL MEDIUM
%token UNKNOWN

%locations

%type <std::unique_ptr<ast::Program>> topLevel
%type <std::unique_ptr<ast::Declaration>> declaration functionDeclaration
%type <std::unique_ptr<ast::StatementBlock>> statementList
%type <std::unique_ptr<ast::Statement>> statement returnStatement variableDeclaration
%type <std::unique_ptr<ast::Expression>> expression baseExpression functionCall
%type <std::unique_ptr<ast::Type>> type typeOrVoid
%type <std::unique_ptr<ast::ParameterList>> parameterList nonVariadicParameterList
%type <std::vector<std::unique_ptr<ast::Expression>>> argumentList
%type <unsigned int> longType

%right EQUAL
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

functionDeclaration : typeOrVoid ID LPAREN parameterList RPAREN SEMI { $$ = std::make_unique<ast::FunctionDeclaration>(std::move($1), $2, std::move($4)); }
                    | typeOrVoid ID LPAREN parameterList RPAREN LBRACE statementList RBRACE { $$ = std::make_unique<ast::FunctionDefinition>(std::move($1), $2, std::move($4), std::move($7)); }
                    ;

parameterList : %empty { $$ = std::make_unique<ast::ParameterList>(); }
              | ELLIPSIS { $$ = std::make_unique<ast::ParameterList>(); $$->markVariadic(); }
              | nonVariadicParameterList COMMA ELLIPSIS { $$ = std::move($1); $$->markVariadic(); }
              | nonVariadicParameterList { $$ = std::move($1); }
              ;

nonVariadicParameterList : type { $$ = std::make_unique<ast::ParameterList>(std::string(), std::move($1)); }
                         | type ID { $$ = std::make_unique<ast::ParameterList>($2, std::move($1)); }
                         | nonVariadicParameterList COMMA type { $1->addParameter(std::string(), std::move($3)); $$ = std::move($1); }
                         | nonVariadicParameterList COMMA type ID { $1->addParameter($4, std::move($3)); $$ = std::move($1); }
                         ;

statement : expression SEMI { $$ = std::make_unique<ast::ExpressionStatement>(std::move($1)); }
          | returnStatement { $$ = std::move($1); }
          | variableDeclaration { $$ = std::move($1); }
          | LBRACE statementList RBRACE { $$ = std::move($2); }
          ;

returnStatement : RETURN SEMI { $$ = std::make_unique<ast::ReturnStatement>(nullptr); }
                | RETURN expression SEMI { $$ = std::make_unique<ast::ReturnStatement>(std::move($2)); }
                ;

variableDeclaration : type ID SEMI { $$ = std::make_unique<ast::VariableDeclaration>(std::move($1), $2, nullptr); }
                    | type ID EQUAL expression SEMI { $$ = std::make_unique<ast::VariableDeclaration>(std::move($1), $2, std::move($4)); }
                    ;

statementList : %empty { $$ = std::make_unique<ast::StatementBlock>(); }
              | statementList statement { $1->addStatement(std::move($2)); $$ = std::move($1); }
              ;

expression : expression PLUS expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Add); }
           | expression MINUS expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Sub); }
           | expression STAR expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Mul); }
           | expression SLASH expression { $$ = std::make_unique<ast::BinaryExpression>(std::move($1), std::move($3), ast::BinaryOperator::Div); }
           | expression EQUAL expression { $$ = std::make_unique<ast::AssignExpression>(std::move($1), std::move($3), ast::AssignOperator::Normal); }
           | baseExpression { $$ = std::move($1); }
           ;

baseExpression : INT10 { $$ = std::make_unique<ast::IntegerLiteral>($1, 10); }
               | INT8 { $$ = std::make_unique<ast::IntegerLiteral>($1, 8); }
               | INT16 { $$ = std::make_unique<ast::IntegerLiteral>($1, 16); }
               | ID { $$ = std::make_unique<ast::Identifier>($1); }
               | LPAREN expression RPAREN { $$ = std::move($2); }
               | functionCall { $$ = std::move($1); }
               ;

functionCall : ID LPAREN RPAREN { $$ = std::make_unique<ast::FunctionCall>($1, std::vector<std::unique_ptr<ast::Expression>>()); }
             | ID LPAREN argumentList RPAREN { $$ = std::make_unique<ast::FunctionCall>($1, std::move($3)); }
             ;

argumentList : expression { $$ = std::vector<std::unique_ptr<ast::Expression>>(); $$.push_back(std::move($1)); }
             | argumentList COMMA expression { $1.push_back(std::move($3)); $$ = std::move($1); }
             ;

type : NOT LONG AT ALL { $$ = std::make_unique<ast::IntegerType>(8); }
     | NOT VERY LONG { $$ = std::make_unique<ast::IntegerType>(16); }
     | MEDIUM LONG { $$ = std::make_unique<ast::IntegerType>(32); }
     | longType { $$ = std::make_unique<ast::IntegerType>($1); }
     | type STAR { $$ = std::make_unique<ast::PointerType>(std::move($1)); }
     ;

typeOrVoid : type { $$ = std::move($1); }
           | VOID { $$ = std::make_unique<ast::VoidType>(); }
           ;

longType : LONG { $$ = 64; }
         | VERY longType { $$ = $2 + 64; }
         ;

%%

void zpp::Parser::error(const location_type &l, const std::string &err) {
  std::cerr << "Error: " << l << ": " << err << std::endl;
}