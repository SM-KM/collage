#include "../util/videos.h"
#include "../util/formatting.h"
#include "requests.h"
#include <algorithm>
#include <map>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <iostream>
#include <optional>
#include <string>
#include <sys/select.h>
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

std::map<std::string, std::vector<Genre>> getAllGenres() {
  const std::string moviesGenresURL =
      "https://api.themoviedb.org/3/genre/movie/list?language=en";
  const std::string seriesGenresURL =
      "https://api.themoviedb.org/3/genre/tv/list?language=en";

  std::string apiKey = GetApiKey();

  std::string movieGenres = fetchFromTMDB(apiKey, moviesGenresURL);
  std::string seriesGenres = fetchFromTMDB(apiKey, seriesGenresURL);

  std::map<std::string, std::vector<Genre>> genreMap;

  try {
    nlohmann::json m_Genres = nlohmann::json::parse(movieGenres);
    nlohmann::json m_Series = nlohmann::json::parse(seriesGenres);
    nlohmann::json movieArray = m_Genres["genres"];
    nlohmann::json seriesArray = m_Series["genres"];

    nlohmann::json combined = movieArray;
    combined.insert(combined.end(), seriesArray.begin(), seriesArray.end());

    std::vector<Genre> genres;
    for (const auto &item : combined) {
      genres.push_back(Genre{item["id"], item["name"]});
    }

    for (const auto &item : genres) {
      genreMap["all"].push_back(Genre{item.id, item.name});
    }

    for (const auto &item : m_Genres["genres"]) {
      genreMap["movies"].push_back(Genre{item["id"], item["name"]});
    }

    for (const auto &item : m_Series["genres"]) {
      genreMap["series"].push_back(Genre{item["id"], item["name"]});
    }

    return genreMap;

  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << "\n";
    return {};
  } catch (nlohmann::json::type_error &e) {
    std::cerr << "JSON type error: " << e.what() << "\n";
    return {};
  }
}

std::variant<std::vector<Movie>, std::vector<Series>, Movie, Series>
getContent(VideoType type, Flags flag, bool byId, std::optional<int> id,
           std::optional<int> genreId) {
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
    if (data.empty()) {
      return {};
      std::cerr << "Missing data or bad formatting of results for movies \n";
    }

    if (type == VideoType::MOVIE) {
      if (byId)
        return formatMovie(data);

      std::vector<Movie> fmovies = formatMovies(data["results"]);

      if (genreId.has_value()) {
        int target = genreId.value();
        std::cout << target;

        // Filter by genre
        fmovies.erase(std::remove_if(fmovies.begin(), fmovies.end(),
                                     [&](const Movie &m) {
                                       return std::find(m.m_genreIds.begin(),
                                                        m.m_genreIds.end(),
                                                        target) ==
                                              m.m_genreIds.end();
                                     }),
                      fmovies.end());

        // Fallback to discover if empty
        if (fmovies.empty()) {
          int page = 1;
          while (true) {
            std::string discoverURL =
                BASE_REQ +
                "discover/movie?language=en-US&page=" + std::to_string(page) +
                "&with_genres=" + std::to_string(target);
            std::string fallback = fetchFromTMDB(apiKey, discoverURL);
            nlohmann::json fallbackData = nlohmann::json::parse(fallback);

            if (fallbackData.empty() || !fallbackData.contains("results") ||
                fallbackData["results"].empty()) {
              break;
            }

            fmovies = formatMovies(data["results"]);
            if (!fmovies.empty()) {
              break;
            }

            if (!fallbackData.contains("total_pages") ||
                page >= fallbackData["total_pages"].get<int>()) {
              break;
            }

            ++page;
          }
        }
      }

      return fmovies;

    } else if (type == VideoType::SERIE) {
      if (byId)
        return formatSerie(data);

      std::vector<Series> fseries = formatSeries(data["results"]);

      if (genreId.has_value()) {
        int target = genreId.value();

        fseries.erase(std::remove_if(fseries.begin(), fseries.end(),
                                     [&](const Series &s) {
                                       return std::find(s.m_genreIds.begin(),
                                                        s.m_genreIds.end(),
                                                        target) ==
                                              s.m_genreIds.end();
                                     }),
                      fseries.end());

        // Fallback to discover if empty
        if (fseries.empty()) {
          int page = 1;
          while (true) {
            std::string discoverURL =
                BASE_REQ +
                "discover/tv?language=en-US&page=" + std::to_string(page) +
                "&with_genres=" + std::to_string(target);
            std::string fallback = fetchFromTMDB(apiKey, discoverURL);
            nlohmann::json fallbackData = nlohmann::json::parse(fallback);

            if (fallbackData.empty() || !fallbackData.contains("results") ||
                fallbackData["results"].empty())
              break;

            fseries = formatSeries(fallbackData["results"]);
            if (!fseries.empty())
              break;

            if (!fallbackData.contains("total_pages") ||
                page >= fallbackData["total_pages"].get<int>())
              break;

            ++page;
          }
        }
      }

      return fseries;
    }

  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << "\n";
    return {};
  }

  return {};
}
