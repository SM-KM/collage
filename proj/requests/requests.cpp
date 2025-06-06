#include "requests.h"
#include <nlohmann/json.hpp>
#include "../util/videos.h"
#include "../util/formatting.h"
#include <curl/curl.h>
#include <iostream>
#include <vector>

// Helpers
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

std::vector<Series> getAiringTodaySeries() {
  std::string url = BASE_REQ + "tv/airing_today?language=en-US&page=1";

  std::string apiKey = GetApiKey();
  std::string response = fetchFromTMDB(apiKey, url);
  try {
    nlohmann::json data = nlohmann::json::parse(response);
    if (data.empty() || !data["results"].is_array()) {
      std::cerr << "Missing data or bad formatting of results for series \n";
      return {};
    }

    return formatSeries(data["results"]);
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "json parsing error: " << e.what() << "\n";
    return {};
  }
}

std::vector<Series> getLatestPopularSeries() {
  std::string url = BASE_REQ + "tv/popular?language=en-US&page=1";

  std::string apiKey = GetApiKey();
  std::string response = fetchFromTMDB(apiKey, url);
  try {
    nlohmann::json data = nlohmann::json::parse(response);
    if (data.empty() || !data["results"].is_array()) {
      std::cerr << "Missing data or bad formatting of results for series \n";
      return {};
    }

    return formatSeries(data["results"]);
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "json parsing error: " << e.what() << "\n";
    return {};
  }
}

std::vector<Series> getTopRatedSeries() {
  std::string url = BASE_REQ + "tv/top_rated?language=en-US&page=1";

  std::string apiKey = GetApiKey();
  std::string response = fetchFromTMDB(apiKey, url);
  try {
    nlohmann::json data = nlohmann::json::parse(response);
    if (data.empty() || !data["results"].is_array()) {
      std::cerr << "Missing data or bad formatting of results for series \n";
      return {};
    }

    return formatSeries(data["results"]);
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "json parsing error: " << e.what() << "\n";
    return {};
  }
}

std::vector<Movie> getLatestPopularMovies() {
  std::string url = BASE_REQ + "movie/popular?language=en-US&page=1";

  std::string apiKey = GetApiKey();
  std::string response = fetchFromTMDB(apiKey, url);
  try {
    nlohmann::json data = nlohmann::json::parse(response);
    if (data.empty() || !data["results"].is_array()) {
      std::cerr << "Missing data or bad formatting of results for movies \n";
      return {};
    }
    return formatMovies(data["results"]);
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << "\n";
    return {};
  }
}
