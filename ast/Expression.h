#ifndef AST_EXPRESSION_H
#define AST_EXPRESSION_H

#include <string>

namespace zpp::ast {
class Expression {
public:
  Expression() = default;
  Expression(const Expression &) = delete;
  virtual ~Expression() = default;
  Expression &operator=(const Expression &) = delete;
};

class IntegerLiteral final : public Expression {
public:
  explicit IntegerLiteral(const unsigned long value) : m_value(value) {}
  [[nodiscard]] unsigned long value() const { return m_value; }

private:
  unsigned long m_value;
};

class Identifier final : public Expression {
public:
  explicit Identifier(std::string name) : m_name(std::move(name)) {}
  [[nodiscard]] const std::string &name() const { return m_name; }

private:
  std::string m_name;
};

enum class BinaryOperator { Add, Sub, Mul, Div };

class BinaryExpression final : public Expression {
public:
  BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
                   BinaryOperator op)
      : m_left(std::move(left)), m_right(std::move(right)), m_op(op) {}
  [[nodiscard]] const Expression &left() const { return *m_left; }
  [[nodiscard]] const Expression &right() const { return *m_right; }
  [[nodiscard]] BinaryOperator op() const { return m_op; }

private:
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
  BinaryOperator m_op;
};
}

#endif
