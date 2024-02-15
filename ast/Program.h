#ifndef AST_PROGRAM_H
#define AST_PROGRAM_H

#include "Declaration.h"
#include <vector>

namespace zpp::ast {
class Program {
public:
  void addDeclaration(std::unique_ptr<Declaration> declaration) {
    declarations.push_back(std::move(declaration));
  }

private:
  std::vector<std::unique_ptr<Declaration>> declarations;
};
}

#endif
