#ifndef AST_EXPRESSION_H
#define AST_EXPRESSION_H

#include "ASTBuilder.h"

#include <string>

namespace zpp::ast {
class Expression {
public:
  Expression() = default;
  Expression(const Expression &) = delete;
  virtual ~Expression() = default;
  Expression &operator=(const Expression &) = delete;
  virtual llvm::Value *_codegen(ASTBuilder &a) const = 0;
  virtual llvm::Value *lvalue(ASTBuilder &a) const;
  llvm::Value *codegen(ASTBuilder &a) const;
};

class IntegerLiteral final : public Expression {
public:
  IntegerLiteral(std::string value, const unsigned char radix)
      : m_value(std::move(value)), m_radix(radix) {}
  [[nodiscard]] const std::string &value() const { return m_value; }
  [[nodiscard]] unsigned char radix() const { return m_radix; }
  llvm::Value *_codegen(ASTBuilder &a) const override;

private:
  std::string m_value;
  unsigned char m_radix;
};

class Identifier final : public Expression {
public:
  explicit Identifier(std::string name) : m_name(std::move(name)) {}
  [[nodiscard]] const std::string &name() const { return m_name; }
  llvm::Value *_codegen(ASTBuilder &a) const override;
  llvm::Value *lvalue(ASTBuilder &a) const override;

private:
  std::string m_name;
};

enum class BinaryOperator { Add, Sub, Mul, Div };

class BinaryExpression final : public Expression {
public:
  BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
                   const BinaryOperator op)
      : m_left(std::move(left)), m_right(std::move(right)), m_op(op) {}
  [[nodiscard]] const Expression &left() const { return *m_left; }
  [[nodiscard]] const Expression &right() const { return *m_right; }
  [[nodiscard]] BinaryOperator op() const { return m_op; }
  llvm::Value *_codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
  BinaryOperator m_op;
};

enum class AssignOperator { Normal };

class AssignExpression final : public Expression {
public:
  AssignExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
                   const AssignOperator op)
      : m_left(std::move(left)), m_right(std::move(right)), m_op(op) {}
  [[nodiscard]] const Expression &left() const { return *m_left; }
  [[nodiscard]] const Expression &right() const { return *m_right; }
  [[nodiscard]] AssignOperator op() const { return m_op; }
  llvm::Value *_codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
  AssignOperator m_op;
};

class FunctionCall final : public Expression {
public:
  FunctionCall(std::string name, std::vector<std::unique_ptr<Expression>> args)
      : m_name(std::move(name)), m_args(std::move(args)) {}
  [[nodiscard]] const std::string &name() const { return m_name; }
  [[nodiscard]] const std::vector<std::unique_ptr<Expression>> &args() const { return m_args; }
  llvm::Value *_codegen(ASTBuilder &a) const override;

private:
  std::string m_name;
  std::vector<std::unique_ptr<Expression>> m_args;
};
}

#endif
