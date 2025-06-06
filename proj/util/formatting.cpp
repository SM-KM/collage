#include "formatting.h"
#include "videos.h"
#include <vector>

std::vector<Movie> formatMovies(nlohmann::json &results) {
  std::vector<Movie> movies;
  for (const auto &item : results) {
    Movie movie;
    movie.m_id = item["id"];
    movie.m_name = item["title"];
    movie.m_overview = item["overview"];
    movie.m_posterPath = item["poster_path"];
    movies.push_back(movie);
  }

  return movies;
}

std::vector<Series> formatSeries(nlohmann::json &results) {
  std::vector<Series> series;
  for (const auto &item : results) {
    Series serie;
    serie.m_id = item["id"];
    serie.m_name = item["title"];
    serie.m_overview = item["overview"];
    series.push_back(serie);
  }

  return series;
}
