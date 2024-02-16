#ifndef PARSERERROR_H
#define PARSERERROR_H

#include <stdexcept>

namespace zpp {
class ParserError final : public std::runtime_error {
public:
  explicit ParserError(const std::string &string) : runtime_error(string) {}
  explicit ParserError(const char *string) : runtime_error(string) {}
  explicit ParserError(const runtime_error &err) : runtime_error(err) {}
};
}

#endif
