#include "ASTBuilder.h"
#include "ParserError.h"

#include <ranges>

llvm::Value *zpp::ASTBuilder::getVariable(const std::string &name) const {
  for (const auto &scope : std::ranges::reverse_view(scopes)) {
    if (llvm::Value *val = scope.lookupVariable(name))
      return val;
  }
  throw ParserError("Use of undeclared identifier: " + name);
}