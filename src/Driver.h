#ifndef DRIVER_H
#define DRIVER_H

#include "ASTBuilder.h"
#include "ZppScanner.h"
#include "parser.hpp"

namespace zpp {
class Driver {
public:
  Driver() : scanner(nullptr), parser(nullptr) {}
  void jitExecute(const std::string &filename);
  ASTBuilder parse(const std::string &filename);

private:
  std::unique_ptr<Scanner> scanner;
  std::unique_ptr<Parser> parser;
  ASTBuilder builder;
};
}

#endif