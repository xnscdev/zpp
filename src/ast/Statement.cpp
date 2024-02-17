#include "Statement.h"

using namespace zpp::ast;

void ExpressionStatement::codegen(ASTBuilder &a) const {
  expr().codegen(a);
}

void ReturnStatement::codegen(ASTBuilder &a) const {
  if (hasValue()) {
    llvm::Value *val = value().codegen(a);
    a.builder().CreateRet(val);
  } else {
    a.builder().CreateRetVoid();
  }
}

void StatementBlock::addStatement(std::unique_ptr<Statement> statement) {
  statements.push_back(std::move(statement));
}

void StatementBlock::codegen(ASTBuilder &a) const {
  a.pushScope();
  genStatements(a);
  a.popScope();
}

void StatementBlock::genStatements(ASTBuilder &a) const {
  for (const auto &ptr : statements) {
    ptr->codegen(a);
  }
}

void VariableDeclaration::codegen(ASTBuilder &a) const {
  llvm::Value *val = hasValue() ? value().codegen(a) : nullptr;
  a.declareVariable(name(), type().resolve(a), val);
}