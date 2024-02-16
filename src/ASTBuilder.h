#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "Scope.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <deque>

namespace zpp {
class ASTBuilder {
public:
  ASTBuilder();
  [[nodiscard]] llvm::LLVMContext &context() const { return *m_context; }
  [[nodiscard]] llvm::Module &module() const { return *m_module; }
  [[nodiscard]] llvm::IRBuilder<> &builder() const { return *m_builder; }
  void pushScope();
  void popScope();
  void declareVariable(const std::string &name, llvm::Value *value);
  [[nodiscard]] llvm::Value *getVariable(const std::string &name) const;
  void optimizeFunction(llvm::Function *func) const;

private:
  std::unique_ptr<llvm::LLVMContext> m_context;
  std::unique_ptr<llvm::Module> m_module;
  std::unique_ptr<llvm::IRBuilder<>> m_builder;
  std::unique_ptr<llvm::FunctionPassManager> fpm;
  std::unique_ptr<llvm::LoopAnalysisManager> lam;
  std::unique_ptr<llvm::FunctionAnalysisManager> fam;
  std::unique_ptr<llvm::CGSCCAnalysisManager> cgam;
  std::unique_ptr<llvm::ModuleAnalysisManager> mam;
  std::unique_ptr<llvm::PassInstrumentationCallbacks> pic;
  std::unique_ptr<llvm::StandardInstrumentations> si;
  std::deque<Scope> scopes;
};
}

#endif