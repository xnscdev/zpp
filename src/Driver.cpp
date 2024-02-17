#include "Driver.h"
#include "Options.h"
#include "ParserError.h"

#include <fstream>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/IR/Verifier.h>

void zpp::Driver::jitExecute(const std::string &filename) {
  ASTBuilder a = parse(filename);
  if (Options::instance().verbose())
    llvm::outs() << "\n\n===== JIT EXECUTION OUTPUT =====\n";
  auto exJit = llvm::orc::LLJITBuilder().create();
  if (auto err = exJit.takeError())
    throw ParserError("Failed to construct JIT");
  const auto jit = std::move(*exJit);
  if (!jit)
    throw ParserError("Failed to construct JIT");
  if (jit->addIRModule(a.exportModule()))
    throw ParserError("Failed to add JIT module");

  auto sym = jit->lookup("main");
  if (!sym)
    throw ParserError(
        "Entry function symbol lookup failed. Ensure your code defines a main() function.");
  auto *func = sym->toPtr<void (*)()>();
  func();
}

zpp::ASTBuilder zpp::Driver::parse(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.good())
    throw ParserError("Failed to open file");
  scanner = std::make_unique<Scanner>(&file);
  std::unique_ptr<ast::Program> program;
  parser = std::make_unique<Parser>(*scanner, program);
  if (parser->parse())
    throw ParserError("Parser error");
  ASTBuilder a;
  for (auto &ptr : program->declarations())
    ptr->codegen(a);
  if (Options::instance().verbose()) {
    llvm::outs() << "===== MODULE IR =====\n";
    a.module().print(llvm::outs(), nullptr);
  }
  if (verifyModule(a.module(), &llvm::errs()))
    throw ParserError("Error in generated LLVM IR");
  return a;
}