#include "Driver.h"
#include "Options.h"

#include <llvm/Support/TargetSelect.h>

int main(const int argc, char **argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++)
    args.emplace_back(argv[i]);
  zpp::Options::instance().parseArgs(args);

  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  zpp::Driver driver;
  if (zpp::Options::instance().jitExecute())
    driver.jitExecute(zpp::Options::instance().sourceFile());
}