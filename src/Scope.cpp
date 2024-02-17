#include "Scope.h"

void zpp::Scope::declareVariable(const std::string &name, llvm::Type *type, llvm::AllocaInst *alloca) {
  variables[name] = std::make_pair(type, alloca);
}

bool zpp::Scope::lookupVariable(const std::string &name, VariableInfo &info) const {
  const auto it = variables.find(name);
  if (it == variables.end())
    return false;
  info = it->second;
  return true;
}