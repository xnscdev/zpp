#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <llvm/IR/Value.h>

namespace zpp {
class Scope {
public:
  void declareVariable(const std::string &name, llvm::Value *value);
  [[nodiscard]] llvm::Value *lookupVariable(const std::string &name) const;

private:
  std::map<std::string, llvm::Value *> variables;
};
}

#endif
