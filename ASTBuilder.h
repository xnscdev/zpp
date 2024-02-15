#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "ZppScanner.h"
#include "parser.hpp"

namespace zpp {
class ASTBuilder {
public:
  ASTBuilder() : scanner(nullptr), parser(nullptr) {}
  void parse(const std::string &filename);
  void parse(std::istream &is);

private:
  std::unique_ptr<Scanner> scanner;
  std::unique_ptr<Parser> parser;
};
}

#endif