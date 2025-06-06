#ifndef REQUESTS_H
#define REQUESTS_H

#include <cstddef>
#include <string>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            std::string *output) {
  output->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string fetchFromTMDB(const std::string &apiKey, const std::string url);
#endif // !REQUESTS_H
