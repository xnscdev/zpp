#include "Statement.h"

using namespace zpp::ast;

bool ExpressionStatement::codegen(ASTBuilder &a) const {
  expr()._codegen(a);
  return true;
}

bool ReturnStatement::codegen(ASTBuilder &a) const {
  if (hasValue()) {
    llvm::Value *val = value().codegen(a);
    a.builder().CreateRet(val);
  } else {
    a.builder().CreateRetVoid();
  }
  return false;
}

void StatementBlock::addStatement(std::unique_ptr<Statement> statement) {
  statements.push_back(std::move(statement));
}

bool StatementBlock::codegen(ASTBuilder &a) const {
  a.pushScope();
  const bool shouldContinue = genStatements(a);
  a.popScope();
  return shouldContinue;
}

bool StatementBlock::genStatements(ASTBuilder &a) const {
  for (const auto &ptr : statements) {
    if (!ptr->codegen(a))
      return false;
  }
  return true;
}

bool VariableDeclaration::codegen(ASTBuilder &a) const {
  llvm::Value *val = hasValue() ? value().codegen(a) : nullptr;
  a.declareVariable(name(), type().resolve(a), val);
  return true;
}