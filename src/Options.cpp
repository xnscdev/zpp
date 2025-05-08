#include "Options.h"
#include "ParserError.h"

zpp::Options zpp::Options::s_instance;

void zpp::Options::parseArgs(const std::vector<std::string> &args) {
  for (int i = 1; i < args.size(); i++) {
    const std::string &arg = args[i];
    if (arg.empty())
      continue;
    if (arg == "-j" || arg == "--jit")
      m_jitExecute = true;
    else if (arg == "-v" || arg == "--verbose")
      m_verbose = true;
    else if (m_sourceFile.empty())
      m_sourceFile = arg;
    else
      throw ParserError("Multiple source files specified");
  }
  if (m_sourceFile.empty())
    throw ParserError("No source files specified");
}