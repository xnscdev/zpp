#ifndef AST_STATEMENT_H
#define AST_STATEMENT_H

#include "Expression.h"

namespace zpp::ast {
class Statement {
public:
  Statement() = default;
  Statement(const Statement &) = delete;
  virtual ~Statement() = default;
  Statement &operator=(const Statement &) = delete;
};

class ExpressionStatement final : public Statement {
public:
  explicit ExpressionStatement(std::unique_ptr<Expression> expr) : m_expr(std::move(expr)) {}
  [[nodiscard]] const Expression &expr() const { return *m_expr; }

private:
  std::unique_ptr<Expression> m_expr;
};

class ReturnStatement final : public Statement {
public:
  explicit ReturnStatement(std::unique_ptr<Expression> value) : m_value(std::move(value)) {}
  [[nodiscard]] bool hasValue() const { return m_value != nullptr; }
  [[nodiscard]] const Expression &value() const { return *m_value; }

private:
  std::unique_ptr<Expression> m_value;
};

class StatementBlock final : public Statement {
public:
  void addStatement(std::unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }

private:
  std::vector<std::unique_ptr<Statement>> statements;
};
}

#endif
