#include "formatting.h"
#include "videos.h"
#include <string>
#include <vector>

std::vector<Movie> formatMovies(nlohmann::json &results) {
  std::vector<Movie> movies;
  for (const auto &item : results) {
    Movie movie;
    movie.m_id = item.value("id", 0);
    movie.m_adult = item.value("adult", false);
    movie.m_name = item.value("title", "");
    movie.m_overview = item.value("overview", "");
    movie.m_posterPath = item.value("poster_path", "");
    movie.m_backdropPath = item.value("backdrop_path", "");
    movie.m_lang = item.value("original_language", "");
    movie.m_voteCount = item.value("vote_count", 0);
    movie.m_voteAverage = item.value("vote_average", 0.0);

    if (!item.value("genre_ids", nlohmann::json::array()).empty()) {
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
    serie.m_id = item.value("id", 0);
    serie.m_name = item.value("title", "");
    serie.m_overview = item.value("overview", "");
    serie.m_backdrop_path = item.value("backdrop_path", "");
    serie.m_firstAirDate = item.value("first_air_date", "");

    if (item.contains("last_air_date") && !item["last_air_date"].is_null()) {
      serie.m_lastAirDate = item.value("last_air_date", "");
    }

    if (item.contains("seasons") && item["seasons"].is_array() &&
        !item["seasons"].empty()) {
      std::vector<Season> seasons;
      for (const auto &s : item["seasons"]) {
        Season season;
        season.m_airDate = s.value("air_date", "");
        season.m_id = s.value("id", 0);
        season.m_sname = s.value("name", "");
        season.m_voteAverage = s.value("vote_average", 0.0);
        season.m_season = s.value("season_number", 0);
        season.m_posterPath = s.value("poster_path", "");
        season.m_overview = s.value("overview", "");
        season.m_episodesAmount = s.value("episode_count", 0);
        seasons.push_back(season);
      }

      serie.m_seasons = seasons;
    }

    serie.show();
    series.push_back(serie);
  }

  return series;
}
