#include "Driver.h"
#include "ParserError.h"

#include <llvm/Support/TargetSelect.h>

int main(const int argc, char **argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++)
    args.emplace_back(argv[i]);

  std::string sourceFile;
  for (const auto &arg : args) {
    if (arg.empty())
      continue;

    if (sourceFile.empty())
      sourceFile = arg;
    else
      throw zpp::ParserError("Multiple source files specified");
  }
  if (sourceFile.empty())
    throw zpp::ParserError("No source files specified");

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  zpp::Driver driver;
  driver.parse(sourceFile);
}