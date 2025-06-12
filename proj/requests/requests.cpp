#include "requests.h"
#include <nlohmann/json.hpp>
#include "../util/videos.h"
#include "../util/formatting.h"
#include <curl/curl.h>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

const std::string BASE_REQ = "https://api.themoviedb.org/3/";

std::string GetApiKey() {
  std::string apiKey = std::getenv("TMDB_API_KEY");

  if (apiKey.empty()) {
    std::cerr << "Api key not found. Exiting. \n";
  }

  return apiKey;
}

std::string fetchFromTMDB(const std::string &apiKey, const std::string url) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  std::string authHeaderValue = "Authorization: Bearer " + apiKey;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "accept: application/json");
    headers = curl_slist_append(headers, authHeaderValue.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }

  return readBuffer;
}

std::variant<std::vector<Movie>, std::vector<Series>, Movie, Series>
getContent(VideoType type, Flags flag, bool byId, std::optional<int> id) {
  std::string testId = id.has_value() ? std::to_string(id.value()) : "";
  std::string complement = byId == true ? testId : to_string(flag);
  std::string paging = byId == true ? "" : "&page=1";
  std::string url =
      type == VideoType::MOVIE
          ? BASE_REQ + "movie/" + complement + "?language=en-US" + paging
          : BASE_REQ + "tv/" + complement + "?language=en-US" + paging;

  std::string apiKey = GetApiKey();
  std::string response = fetchFromTMDB(apiKey, url);

  try {
    nlohmann::json data = nlohmann::json::parse(response);
    std::cout << data << "\n";
    if (data.empty()) {
      std::cerr << "Missing data or bad formatting of results for movies \n";
      return {};
    }

    if (type == VideoType::MOVIE) {
      if (byId)
        return formatMovie(data);

      return formatMovies(data["results"]);
    } else if (type == VideoType::SERIE) {
      if (byId)
        return formatSerie(data);

      return formatSeries(data["results"]);
    }

  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << "\n";
    return {};
  }

  return {};
}
