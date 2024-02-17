#include "Expression.h"
#include "ParserError.h"

using namespace zpp::ast;

llvm::Value *Expression::lvalue(ASTBuilder &a) const {
  return codegen(a);
}

llvm::Value *IntegerLiteral::codegen(ASTBuilder &a) const {
  return llvm::ConstantInt::get(llvm::Type::getInt64Ty(a.context()), llvm::APInt(64, value()));
}

llvm::Value *Identifier::codegen(ASTBuilder &a) const {
  return a.getVariable(name(), false);
}

llvm::Value *Identifier::lvalue(ASTBuilder &a) const {
  return a.getVariable(name(), true);
}

llvm::Value *BinaryExpression::codegen(ASTBuilder &a) const {
  llvm::Value *leftVal = op() == BinaryOperator::Assign ? left().lvalue(a) : left().codegen(a);
  llvm::Value *rightVal = right().codegen(a);
  switch (op()) {
  case BinaryOperator::Add:
    return a.builder().CreateAdd(leftVal, rightVal);
  case BinaryOperator::Sub:
    return a.builder().CreateSub(leftVal, rightVal);
  case BinaryOperator::Mul:
    return a.builder().CreateMul(leftVal, rightVal);
  case BinaryOperator::Div:
    return a.builder().CreateSDiv(leftVal, rightVal);
  case BinaryOperator::Assign:
    a.builder().CreateStore(rightVal, leftVal);
    return rightVal;
  default:
    throw ParserError("Invalid binary operation");
  }
}