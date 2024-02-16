#include "Scope.h"

void zpp::Scope::declareVariable(const std::string &name, llvm::Value *value) {
  variables[name] = value;
}

llvm::Value *zpp::Scope::lookupVariable(const std::string &name) const {
  const auto it = variables.find(name);
  if (it == variables.end())
    return nullptr;
  return it->second;
}