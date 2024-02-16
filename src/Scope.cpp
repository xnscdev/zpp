#include "Scope.h"

llvm::Value *zpp::Scope::lookupVariable(const std::string &name) const {
  const auto it = variables.find(name);
  if (it == variables.end())
    return nullptr;
  return it->second;
}