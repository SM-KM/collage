#ifndef REQUESTS_H
#define REQUESTS_H

#include "../util/videos.h"
#include <cstddef>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            std::string *output) {
  output->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string fetchFromTMDB(const std::string &apiKey, const std::string url);

std::vector<Series> getLatestPopularSeries();
std::vector<Series> getTopRatedSeries();
std::vector<Series> getAiringTodaySeries();

std::vector<Movie> getLatestPopularMovies();

#endif // !REQUESTS_H
