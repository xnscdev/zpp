#ifndef AST_PROGRAM_H
#define AST_PROGRAM_H

#include "Declaration.h"
#include <vector>

namespace zpp::ast {
class Program {
public:
  [[nodiscard]] const std::vector<std::unique_ptr<Declaration>> &declarations() const {
    return m_declarations;
  }

  void addDeclaration(std::unique_ptr<Declaration> declaration) {
    m_declarations.push_back(std::move(declaration));
  }

private:
  std::vector<std::unique_ptr<Declaration>> m_declarations;
};
}

#endif
