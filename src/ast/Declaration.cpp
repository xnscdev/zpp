#include "Declaration.h"
#include "ParserError.h"

using namespace zpp::ast;

void FunctionDeclaration::codegen(ASTBuilder &a) const {
  std::vector<llvm::Type *> types;
  for (std::size_t i = 0; i < params().size(); i++)
    types.push_back(llvm::Type::getInt64Ty(a.context()));
  llvm::FunctionType *funcType =
      llvm::FunctionType::get(llvm::Type::getInt64Ty(a.context()), types, false);

  if (llvm::Function *func = a.module().getFunction(name())) {
    if (func->getFunctionType() != funcType)
      throw ParserError("Function named '" + name() +
                        "' is already declared with a different signature");
  } else {
    llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name(), a.module());
  }
}

void FunctionDefinition::codegen(ASTBuilder &a) const {
}