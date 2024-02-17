#include "ASTBuilder.h"
#include "ParserError.h"

#include <ranges>

#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

zpp::ASTBuilder::ASTBuilder()
    : m_context(std::make_unique<llvm::LLVMContext>()),
      m_module(std::make_unique<llvm::Module>("z++", *m_context)),
      m_builder(std::make_unique<llvm::IRBuilder<>>(*m_context)),
      fpm(std::make_unique<llvm::FunctionPassManager>()),
      lam(std::make_unique<llvm::LoopAnalysisManager>()),
      fam(std::make_unique<llvm::FunctionAnalysisManager>()),
      cgam(std::make_unique<llvm::CGSCCAnalysisManager>()),
      mam(std::make_unique<llvm::ModuleAnalysisManager>()),
      pic(std::make_unique<llvm::PassInstrumentationCallbacks>()),
      si(std::make_unique<llvm::StandardInstrumentations>(context(), true)) {
  si->registerCallbacks(*pic, mam.get());
  fpm->addPass(llvm::InstCombinePass());
  fpm->addPass(llvm::ReassociatePass());
  fpm->addPass(llvm::GVNPass());
  fpm->addPass(llvm::SimplifyCFGPass());

  llvm::PassBuilder pb;
  pb.registerModuleAnalyses(*mam);
  pb.registerFunctionAnalyses(*fam);
  pb.crossRegisterProxies(*lam, *fam, *cgam, *mam);

  pushScope(); // Create global scope
}

void zpp::ASTBuilder::pushScope() {
  scopes.emplace_back();
}

void zpp::ASTBuilder::popScope() {
  scopes.pop_back();
}

void zpp::ASTBuilder::declareVariable(const std::string &name, llvm::Type *type,
                                      llvm::Value *value) {
  if (scopes.back().hasVariable(name))
    throw ParserError("Variable '" + name + "' was already defined in this scope");
  llvm::AllocaInst *alloca = builder().CreateAlloca(type);
  if (value)
    builder().CreateStore(value, alloca);
  scopes.back().declareVariable(name, type, alloca);
}

llvm::Value *zpp::ASTBuilder::getVariable(const std::string &name, const bool pointer) const {
  Scope::VariableInfo info;
  for (const auto &scope : std::ranges::reverse_view(scopes)) {
    if (scope.lookupVariable(name, info)) {
      if (pointer)
        return info.second;
      return builder().CreateLoad(info.first, info.second);
    }
  }
  throw ParserError("Use of undeclared identifier: " + name);
}

void zpp::ASTBuilder::optimizeFunction(llvm::Function *func) const {
  fpm->run(*func, *fam);
}