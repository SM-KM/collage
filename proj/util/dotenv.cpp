#include "dotenv.h"
#include <fstream>
#include <string>

void loadDotEnv(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;

  while (std::getline(file, line)) {
    size_t eq = line.find("=");
    if (eq != std::string::npos) {
      std::string key = line.substr(0, eq);
      std::string value = line.substr(eq + 1);

      key.erase(0, key.find_first_not_of(" \t\r\n"));
      key.erase(key.find_last_not_of(" \t\r\n") + 1);
      value.erase(0, value.find_first_not_of(" \t\r\n"));
      value.erase(value.find_last_not_of(" \t\r\n") + 1);

#ifdef _WIN32
      _putenv((key + "=" + value).c_str());
#else
      setenv(key.c_str(), value.c_str(), 1);
#endif
    }
  }
}
