#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "ParameterList.h"
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
  FunctionDeclaration(std::unique_ptr<Type> returnType, std::string name,
                      std::unique_ptr<ParameterList> params)
      : m_returnType(std::move(returnType)), m_name(std::move(name)), m_params(std::move(params)) {}
  [[nodiscard]] const Type &returnType() const { return *m_returnType; }
  [[nodiscard]] const std::string &name() const { return m_name; }
  [[nodiscard]] const ParameterList &params() const { return *m_params; }
  void codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<Type> m_returnType;
  std::string m_name;
  std::unique_ptr<ParameterList> m_params;
};

class FunctionDefinition final : public FunctionDeclaration {
public:
  FunctionDefinition(std::unique_ptr<Type> returnType, std::string name,
                     std::unique_ptr<ParameterList> params, std::unique_ptr<StatementBlock> body)
      : FunctionDeclaration(std::move(returnType), std::move(name), std::move(params)),
        m_body(std::move(body)) {}
  [[nodiscard]] const StatementBlock &body() const { return *m_body; }
  void codegen(ASTBuilder &a) const override;

private:
  std::unique_ptr<StatementBlock> m_body;
};
}

#endif
