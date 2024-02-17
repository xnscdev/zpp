#include "Type.h"

#include <llvm/IR/DerivedTypes.h>

using namespace zpp::ast;

llvm::Type *IntegerType::resolve(ASTBuilder &a) const {
  return llvm::IntegerType::get(a.context(), size());
}