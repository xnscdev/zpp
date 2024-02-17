#include "Expression.h"

#include "ParserError.h"
#include <ranges>

using namespace zpp::ast;

llvm::Value *Expression::lvalue(ASTBuilder &a) const {
  return codegen(a);
}

llvm::Value *Expression::codegen(ASTBuilder &a) const {
  llvm::Value *value = _codegen(a);
  if (value->getType() == llvm::Type::getVoidTy(a.context()))
    throw ParserError("Expression of void type is invalid here");
  return value;
}

llvm::Value *IntegerLiteral::_codegen(ASTBuilder &a) const {
  int count = 0;
  std::string string = value();
  for (auto it = string.rbegin(); it != string.rend(); ++it) {
    if (std::tolower(*it) == 'l') {
      count++;
    } else {
      string.erase(it.base(), string.end());
      break;
    }
  }
  unsigned int bits = 8 << std::min(count, 3);
  bits += 64 * std::max(count - 3, 0);
  return llvm::ConstantInt::get(llvm::IntegerType::get(a.context(), bits),
                                llvm::APInt(bits, string, radix()));
}

StringLiteral::StringLiteral(const std::string &text) {
  std::size_t i = 0;
  while (i < text.size()) {
    if (text[i] == '\\' && i + 1 < text.size()) {
      switch (text[i + 1]) {
      case 'n':
        m_text += '\n';
        break;
      case 't':
        m_text += '\t';
        break;
      case 'r':
        m_text += '\r';
        break;
      default:
        m_text += text[i + 1];
      }
      i += 2;
    } else {
      m_text += text[i];
      ++i;
    }
  }
}

llvm::Value *StringLiteral::_codegen(ASTBuilder &a) const {
  return a.builder().CreateGlobalStringPtr(text());
}

llvm::Value *Identifier::_codegen(ASTBuilder &a) const {
  return a.getVariable(name(), false);
}

llvm::Value *Identifier::lvalue(ASTBuilder &a) const {
  return a.getVariable(name(), true);
}

llvm::Value *BinaryExpression::_codegen(ASTBuilder &a) const {
  llvm::Value *leftVal = left().codegen(a);
  llvm::Value *rightVal = right().codegen(a);
  if (leftVal->getType() != rightVal->getType())
    throw ParserError("Operands of binary expression must have same type");
  switch (op()) {
  case BinaryOperator::Add:
    return a.builder().CreateAdd(leftVal, rightVal);
  case BinaryOperator::Sub:
    return a.builder().CreateSub(leftVal, rightVal);
  case BinaryOperator::Mul:
    return a.builder().CreateMul(leftVal, rightVal);
  case BinaryOperator::Div:
    return a.builder().CreateSDiv(leftVal, rightVal);
  default:
    throw ParserError("Invalid binary operation");
  }
}

llvm::Value *AssignExpression::_codegen(ASTBuilder &a) const {
  llvm::Value *leftVal = left().lvalue(a);
  llvm::Value *rightVal = right().codegen(a);
  if (leftVal->getType() != rightVal->getType()->getPointerTo())
    throw ParserError("Operands of assignment expression must have same type");
  switch (op()) {
  case AssignOperator::Normal:
    a.builder().CreateStore(rightVal, leftVal);
    return rightVal;
  default:
    throw ParserError("Invalid assignment operation");
  }
}

llvm::Value *FunctionCall::_codegen(ASTBuilder &a) const {
  llvm::Function *func = a.module().getFunction(name());
  if (!func)
    throw ParserError("Function '" + name() + "' was not previously declared");
  std::vector<llvm::Value *> values;
  for (auto &arg : args())
    values.push_back(arg->codegen(a));
  return a.builder().CreateCall(func->getFunctionType(), func, values);
}