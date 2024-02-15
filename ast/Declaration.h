#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "Statement.h"

namespace zpp::ast {
class Declaration {
public:
  virtual ~Declaration() = default;
};

class FunctionDeclaration : public Declaration {
public:
  FunctionDeclaration(std::string name, const std::vector<std::string> &params)
      : m_name(std::move(name)), m_params(params) {}
  [[nodiscard]] const std::string &name() const { return m_name; }
  [[nodiscard]] const std::vector<std::string> &params() const { return m_params; }

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

private:
  std::unique_ptr<StatementBlock> m_body;
};
}

#endif
