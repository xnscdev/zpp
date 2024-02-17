#include "Expression.h"
#include "ParserError.h"

using namespace zpp::ast;

llvm::Value *Expression::lvalue(ASTBuilder &a) const {
  return codegenNoVoid(a);
}

llvm::Value *Expression::codegenNoVoid(ASTBuilder &a) const {
  llvm::Value *value = codegen(a);
  if (value->getType() == llvm::Type::getVoidTy(a.context()))
    throw ParserError("Expression of void type is invalid here");
  return value;
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
  llvm::Value *leftVal =
      op() == BinaryOperator::Assign ? left().lvalue(a) : left().codegenNoVoid(a);
  llvm::Value *rightVal = right().codegenNoVoid(a);
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

llvm::Value *FunctionCall::codegen(ASTBuilder &a) const {
  llvm::Function *func = a.module().getFunction(name());
  if (!func)
    throw ParserError("Function '" + name() + "' was not previously declared");
  std::vector<llvm::Value *> values;
  for (auto &arg : args())
    values.push_back(arg->codegenNoVoid(a));
  return a.builder().CreateCall(func->getFunctionType(), func, values);
}