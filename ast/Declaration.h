#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "Statement.h"

namespace zpp::ast {
class Declaration {
public:
  Declaration() = default;
  Declaration(const Declaration &) = delete;
  virtual ~Declaration() = default;
  Declaration &operator=(const Declaration &) = delete;
  virtual void codegen(ASTBuilder &a) const = 0;
};

class FunctionDeclaration : public Declaration {
public:
  FunctionDeclaration(std::string name, const std::vector<std::string> &params)
      : m_name(std::move(name)), m_params(params) {}
  [[nodiscard]] const std::string &name() const { return m_name; }
  [[nodiscard]] const std::vector<std::string> &params() const { return m_params; }
  void codegen(ASTBuilder &a) const override;

private:
  std::string m_name;
  std::vector<std::string> m_params;
};

class FunctionDefinition final : public FunctionDeclaration {
public:
  FunctionDefinition(std::string name, const std::vector<std::string> &params,
                     std::unique_ptr<StatementBlock> body)
      : FunctionDeclaration(std::move(name), params), m_body(std::move(body)) {}
  [[nodiscard]] const StatementBlock &body() const { return *m_body; }
  void codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<StatementBlock> m_body;
};
}

#endif
