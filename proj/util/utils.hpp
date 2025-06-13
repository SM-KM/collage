#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <boost/pfr.hpp>
#include <boost/pfr/core.hpp>
#include <iostream>

// Only works for plain data, no abstract
template <typename T> void showProperties(const T &obj) {
  boost::pfr::for_each_field(obj, [](const auto &field, std::size_t idx) {
    std::cout << "Field " << idx << ": " << field << "\n";
  });
}

std::string to_lower(const std::string &str) {
  std::string lower = str;
  std::transform(lower.begin(), lower.end(), lower.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return lower;
}

#endif // !UTILS_H
