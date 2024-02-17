#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <vector>

namespace zpp {
class Options final {
public:
  static Options &instance() { return s_instance; }
  void parseArgs(const std::vector<std::string> &args);
  [[nodiscard]] const std::string &sourceFile() const { return m_sourceFile; }
  [[nodiscard]] bool verbose() const { return m_verbose; }
  [[nodiscard]] bool jitExecute() const { return m_jitExecute; }

private:
  Options() = default;

  static Options s_instance;
  std::string m_sourceFile;
  bool m_verbose = false;
  bool m_jitExecute = false;
};
}

#endif
