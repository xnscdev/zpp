#include "Type.h"

#include <llvm/IR/DerivedTypes.h>

using namespace zpp::ast;

llvm::Type *VoidType::resolve(ASTBuilder &a) const {
  return llvm::Type::getVoidTy(a.context());
}

llvm::Type *IntegerType::resolve(ASTBuilder &a) const {
  return llvm::IntegerType::get(a.context(), size());
}