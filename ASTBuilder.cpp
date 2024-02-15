#include "ASTBuilder.h"
#include <fstream>

void zpp::ASTBuilder::parse(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.good())
    throw std::runtime_error("Failed to open file");
  parse(file);
}

void zpp::ASTBuilder::parse(std::istream &is) {
  if (!is.good())
    return;
  scanner = std::make_unique<Scanner>(&is);
  std::unique_ptr<ast::Program> program;
  parser = std::make_unique<Parser>(*scanner, program);
  if (parser->parse())
    throw std::runtime_error("Parsing failed");
}