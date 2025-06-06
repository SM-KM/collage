#ifndef UTILS_H
#define UTILS_H

#include <boost/pfr.hpp>
#include <boost/pfr/core.hpp>
#include <iostream>

// Only works for plain data, no abstract
template <typename T> void showProperties(const T &obj) {
  boost::pfr::for_each_field(obj, [](const auto &field, std::size_t idx) {
    std::cout << "Field " << idx << ": " << field << "\n";
  });
}
#endif // !UTILS_H
