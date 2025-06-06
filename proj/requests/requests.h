#ifndef REQUESTS_H
#define REQUESTS_H

#include <cstddef>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            std::string *output) {
  output->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string fetchFromTMDB(const std::string &apiKey, const std::string url);
nlohmann::json getLatestPopularMovies();
nlohmann::json getLatestPopularSeries();

#endif // !REQUESTS_H
