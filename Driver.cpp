#include "Driver.h"
#include "ParserError.h"

#include <fstream>
#include <llvm/IR/Verifier.h>

void zpp::Driver::parse(const std::string &filename) {
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
  a.module().print(llvm::outs(), nullptr);
  if (verifyModule(a.module(), &llvm::errs()))
    throw ParserError("Error in generated LLVM IR");
}