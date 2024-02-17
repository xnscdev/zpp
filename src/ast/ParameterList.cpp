#include "ParameterList.h"
#include "ParserError.h"

using namespace zpp::ast;

ParameterList::ParameterList(const std::string &firstName, std::unique_ptr<Type> firstType) {
  addParameter(firstName, std::move(firstType));
}

void ParameterList::addParameter(const std::string &name, std::unique_ptr<Type> type) {
  if (!name.empty()) {
    if (paramNames.contains(name))
      throw ParserError("Redefinition of parameter: " + name);
    paramNames.insert(name);
  }
  m_list.emplace_back(name, std::move(type));
}