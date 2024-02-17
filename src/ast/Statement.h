#ifndef AST_STATEMENT_H
#define AST_STATEMENT_H

#include "Expression.h"
#include "Type.h"

namespace zpp::ast {
class Statement {
public:
  Statement() = default;
  Statement(const Statement &) = delete;
  virtual ~Statement() = default;
  Statement &operator=(const Statement &) = delete;
  virtual void codegen(ASTBuilder &a) const = 0;
};

class ExpressionStatement final : public Statement {
public:
  explicit ExpressionStatement(std::unique_ptr<Expression> expr) : m_expr(std::move(expr)) {}
  [[nodiscard]] const Expression &expr() const { return *m_expr; }
  void codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<Expression> m_expr;
};

class ReturnStatement final : public Statement {
public:
  explicit ReturnStatement(std::unique_ptr<Expression> value) : m_value(std::move(value)) {}
  [[nodiscard]] bool hasValue() const { return m_value != nullptr; }
  [[nodiscard]] const Expression &value() const { return *m_value; }
  void codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<Expression> m_value;
};

class StatementBlock final : public Statement {
public:
  void addStatement(std::unique_ptr<Statement> statement);
  void codegen(ASTBuilder &a) const override;
  void genStatements(ASTBuilder &a) const;

private:
  std::vector<std::unique_ptr<Statement>> statements;
};

class VariableDeclaration final : public Statement {
public:
  VariableDeclaration(std::unique_ptr<Type> type, std::string name,
                      std::unique_ptr<Expression> value)
      : m_type(std::move(type)), m_name(std::move(name)), m_value(std::move(value)) {}
  [[nodiscard]] const Type &type() const { return *m_type; }
  [[nodiscard]] const std::string &name() const { return m_name; }
  [[nodiscard]] bool hasValue() const { return m_value != nullptr; }
  [[nodiscard]] const Expression &value() const { return *m_value; }
  void codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<Type> m_type;
  std::string m_name;
  std::unique_ptr<Expression> m_value;
};
}

#endif
