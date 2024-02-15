#ifndef ZPPSCANNER_H
#define ZPPSCANNER_H

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include "location.hh"
#include "parser.hpp"

namespace zpp {
class Scanner : public yyFlexLexer {
public:
  explicit Scanner(std::istream *in) : yyFlexLexer(in) { loc = new zpp::Parser::location_type; }

  virtual int yylex(zpp::Parser::semantic_type *lval, zpp::Parser::location_type *location);

private:
  zpp::Parser::semantic_type *yylval = nullptr;
  zpp::Parser::location_type *loc = nullptr;
};
}

#endif
