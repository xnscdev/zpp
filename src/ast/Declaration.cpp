#include "Declaration.h"
#include "ParserError.h"

#include <llvm/IR/Verifier.h>

using namespace zpp::ast;

void FunctionDeclaration::codegen(ASTBuilder &a) const {
  std::vector<llvm::Type *> types;
  for (const auto &[name, type] : params().list())
    types.push_back(type->resolve(a));
  llvm::FunctionType *funcType = llvm::FunctionType::get(returnType().resolve(a), types, false);

  if (const llvm::Function *func = a.module().getFunction(name())) {
    if (func->getFunctionType() != funcType)
      throw ParserError("Function named '" + name() +
                        "' is already declared with a different signature");
  } else {
    llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name(), a.module());
  }
}

void FunctionDefinition::codegen(ASTBuilder &a) const {
  std::vector<llvm::Type *> types;
  for (const auto &[name, type] : params().list())
    types.push_back(type->resolve(a));
  llvm::FunctionType *funcType = llvm::FunctionType::get(returnType().resolve(a), types, false);

  llvm::Function *func = a.module().getFunction(name());
  if (func) {
    if (func->getFunctionType() != funcType)
      throw ParserError("Function named '" + name() +
                        "' is already declared with a different signature");
  } else {
    func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name(), a.module());
  }

  a.pushScope();
  llvm::BasicBlock *block = llvm::BasicBlock::Create(a.context(), "entry", func);
  a.builder().SetInsertPoint(block);
  auto argIt = func->arg_begin();
  for (const auto &[name, type] : params().list()) {
    a.declareVariable(name, type->resolve(a), argIt);
    ++argIt;
  }
  body().genStatements(a);
  a.popScope();

  if (returnType().resolve(a) == llvm::Type::getVoidTy(a.context()))
    a.builder().CreateRetVoid();
  else if (!block->getTerminator())
    throw ParserError("Missing return statement from function returning non-void");

  if (verifyFunction(*func, &llvm::errs()))
    throw ParserError("Invalid LLVM IR for function");
  a.optimizeFunction(func);
}