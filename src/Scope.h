#ifndef SCOPE_H
#define SCOPE_H

#include <llvm/IR/Instructions.h>
#include <unordered_map>

namespace zpp {
class Scope {
public:
  using VariableInfo = std::pair<llvm::Type *, llvm::AllocaInst *>;

  void declareVariable(const std::string &name, llvm::Type *type, llvm::AllocaInst *alloca);
  bool lookupVariable(const std::string &name, VariableInfo &info) const;

private:
  std::unordered_map<std::string, VariableInfo> variables;
};
}

#endif
