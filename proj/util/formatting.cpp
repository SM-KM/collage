#include "formatting.h"
#include "videos.h"
#include <string>
#include <vector>

std::vector<Movie> formatMovies(nlohmann::json &results) {
  std::vector<Movie> movies;
  for (const auto &item : results) {
    Movie movie;
    movie.m_id = item["id"];
    movie.m_adult = item["adult"];
    movie.m_name = item["title"];
    movie.m_overview = item["overview"];
    movie.m_posterPath = item["poster_path"];
    movie.m_backdropPath = item["backdrop_path"];
    movie.m_lang = item["original_language"];
    movie.m_voteCount = item["vote_count"];
    movie.m_voteAverage = item["vote_average"];

    if (!item["genre_ids"].empty()) {
      for (const auto &id : item["genre_ids"]) {
        movie.m_genreIds.push_back(id);
      }
    }

    movie.show();
    movies.push_back(movie);
  }

  return movies;
}

std::vector<Series> formatSeries(nlohmann::json &results) {
  std::vector<Series> series;
  for (const auto &item : results) {
    Series serie;
    serie.m_adult = item["adult"];
    serie.m_id = item["id"];
    serie.m_name = item["title"];
    serie.m_overview = item["overview"];
    serie.m_backdrop_path = item["backdrop_path"];
    serie.m_firstAirDate = item["first_air_date"];

    if (!item["last_air_date"].empty()) {
      serie.m_lastAirDate = item["last_air_date"];
    }

    if (!item["seasons"].empty()) {
      std::vector<Season> seasons;
      for (const auto &s : item["seasons"]) {
        Season season;
        season.m_airDate = s["air_data"];
        season.m_id = s["id"];
        season.m_sname = s["name"];
        season.m_voteAverage = s["vote_average"];
        season.m_season = s["season_number"];
        season.m_posterPath = s["poster_path"];
        season.m_overview = s["overview"];

        seasons.push_back(season);
      }

      serie.m_seasons = seasons;
    }

    serie.show();
    series.push_back(serie);
  }

  return series;
}
