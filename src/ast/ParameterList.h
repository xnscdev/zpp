#ifndef PARAMETERLIST_H
#define PARAMETERLIST_H

#include "Type.h"

#include <unordered_set>

namespace zpp::ast {
class ParameterList final {
public:
  ParameterList() = default;
  ParameterList(const std::string &firstName, std::unique_ptr<Type> firstType);
  void addParameter(const std::string &name, std::unique_ptr<Type> type);

  [[nodiscard]] const std::vector<std::pair<std::string, std::unique_ptr<Type>>> &list() const {
    return m_list;
  }

private:
  std::vector<std::pair<std::string, std::unique_ptr<Type>>> m_list;
  std::unordered_set<std::string> paramNames;
};
}

#endif
