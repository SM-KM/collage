#include "requests.h"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

// Helpers

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

nlohmann::json getLatestPopularMovies() {
  std::string url =
      "https://api.themoviedb.org/3/movie/popular?language=en-US&page=1";

  std::string apiKey = GetApiKey();
  std::string response = fetchFromTMDB(apiKey, url);
  try {
    nlohmann::json data = nlohmann::json::parse(response);
    return data["results"];
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << "\n";
    return nlohmann::json::object();
  }
}
