#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "Scope.h"
#include <llvm/IR/IRBuilder.h>
#include <deque>

namespace zpp {
class ASTBuilder {
public:
  ASTBuilder()
      : m_context(std::make_unique<llvm::LLVMContext>()),
        m_module(std::make_unique<llvm::Module>("z++", *m_context)),
        m_builder(std::make_unique<llvm::IRBuilder<>>(*m_context)) {}
  [[nodiscard]] llvm::LLVMContext &context() const { return *m_context; }
  [[nodiscard]] llvm::Module &module() const { return *m_module; }
  [[nodiscard]] llvm::IRBuilder<> &builder() const { return *m_builder; }
  [[nodiscard]] llvm::Value *getVariable(const std::string &name) const;

private:
  std::unique_ptr<llvm::LLVMContext> m_context;
  std::unique_ptr<llvm::Module> m_module;
  std::unique_ptr<llvm::IRBuilder<>> m_builder;
  std::deque<Scope> scopes;
};
}

#endif