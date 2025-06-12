#ifndef REQUESTS_H
#define REQUESTS_H

#include "../util/videos.h"
#include <cstddef>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <variant>
#include <vector>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            std::string *output) {
  output->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string fetchFromTMDB(const std::string &apiKey, const std::string url);

std::variant<std::vector<Movie>, std::vector<Series>, Movie, Series>
getContent(VideoType type, Flags flag, bool byId = false,
           std::optional<int> id = std::nullopt);

#endif // !REQUESTS_H
