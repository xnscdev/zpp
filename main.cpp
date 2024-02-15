#include "ASTBuilder.h"
#include <iostream>

int main() {
  zpp::ASTBuilder builder;
  builder.parse(std::cin);
}