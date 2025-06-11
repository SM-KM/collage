#include "formatting.h"
#include "csv.h"
#include "types.h"
#include "videos.h"
#include <exception>
#include <iostream>
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
    try {
      std::vector<Entry> entries = readCSV(getMoviesCSVPath());
      Entry *entry = FindEntryById(entries, movie.m_id);

      if (entry) {
        int tmdb_count = item.value("vote_count", 0);
        double tmdb_rating = item.value("vote_average", 0.0);
        int total_count = tmdb_count + entry->count;
        double total_rating =
            (tmdb_rating * tmdb_count) + (entry->rating * entry->count);

        std::cout << entry->rating << " " << entry->count;

        movie.m_voteCount = total_count;
        movie.m_voteAverage =
            total_count > 0 ? total_rating / total_count : 0.0;

        std::cout << movie.m_voteAverage << " " << movie.m_voteCount << "\n";

      } else {
        movie.m_voteCount = item.value("vote_count", 0);
        movie.m_voteAverage = item.value("vote_average", 0.0);
      }

    } catch (const std::exception &e) {
      std::cerr
          << "Something went wrong retreiving or finding the entry data \n";
    }

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

Movie formatMovie(nlohmann::json &m) {
  Movie movie;
  movie.m_id = m.value("id", 0);
  movie.m_adult = m.value("adult", false);
  movie.m_name = m.value("title", "");
  movie.m_overview = m.value("overview", "");
  movie.m_posterPath = m.value("poster_path", "");
  movie.m_backdropPath = m.value("backdrop_path", "");
  movie.m_lang = m.value("original_language", "");

  try {
    std::vector<Entry> entries = readCSV(getMoviesCSVPath());
    Entry *entry = FindEntryById(entries, movie.m_id);

    if (entry) {
      int tmdb_count = m.value("vote_count", 0);
      double tmdb_rating = m.value("vote_average", 0.0);
      int total_count = tmdb_count + entry->count;
      double total_rating =
          (tmdb_rating * tmdb_count) + (entry->rating * entry->count);

      std::cout << entry->rating << " " << entry->count;

      movie.m_voteCount = total_count;
      movie.m_voteAverage = total_count > 0 ? total_rating / total_count : 0.0;

      std::cout << movie.m_voteAverage << " " << movie.m_voteCount << "\n";

    } else {
      movie.m_voteCount = m.value("vote_count", 0);
      movie.m_voteAverage = m.value("vote_average", 0.0);
    }

  } catch (const std::exception &e) {
    std::cerr << "Something went wrong retreiving or finding the entry data \n";
  }
  if (!m.value("genre_ids", nlohmann::json::array()).empty()) {
    for (const auto &id : m["genre_ids"]) {
      movie.m_genreIds.push_back(id);
    }
  }

  if (!m.value("genres", nlohmann::json::array()).empty()) {
    std::vector<MovieGenre> genres;
    for (const auto &i : m["genres"]) {
      MovieGenre movieGenre;
      movieGenre.id = i["id"];
      movieGenre.name = i["name"];

      genres.push_back(movieGenre);
    }

    movie.m_genres = genres;
  }

  movie.show();
  return movie;
}

Series formatSerie(nlohmann::json &s) {
  Series serie;

  serie.m_id = s.value("id", 0);
  serie.m_name = s.contains("name") && s["name"].is_string()
                     ? s["name"].get<std::string>()
                     : "";
  serie.m_overview = s.contains("overview") && s["overview"].is_string()
                         ? s["overview"].get<std::string>()
                         : "";
  serie.m_backdrop_path =
      s.contains("backdrop_path") && s["backdrop_path"].is_string()
          ? s["backdrop_path"].get<std::string>()
          : "";
  serie.m_firstAirDate =
      s.contains("first_air_date") && s["first_air_date"].is_string()
          ? s["first_air_date"].get<std::string>()
          : "";

  if (s.contains("last_air_date") && s["last_air_date"].is_string()) {
    serie.m_lastAirDate = s["last_air_date"];
  }

  if (s.contains("genres") && s["genres"].is_array() && !s["genres"].empty()) {
    std::vector<SeriesGenre> genres;
    for (const auto &i : s["genres"]) {
      SeriesGenre serieGenre;
      serieGenre.id = i.value("id", 0);
      serieGenre.name = i.contains("name") && i["name"].is_string()
                            ? i["name"].get<std::string>()
                            : "";
      genres.push_back(serieGenre);
    }
    serie.m_genres = genres;
  }

  std::vector<Entry> entries = readCSV(getSeriesCSVPath());
  Entry *entry = FindEntryById(entries, serie.m_id);

  if (entry) {
    serie.m_voteAverage = entry->rating;
    serie.m_voteCount = entry->count;
  } else {
    serie.m_voteAverage = 0.0;
    serie.m_voteCount = 0;
  }

  if (s.contains("seasons") && s["seasons"].is_array() &&
      !s["seasons"].empty()) {
    std::vector<Season> seasons;
    for (const auto &ss : s["seasons"]) {
      Season season;
      season.m_airDate = ss.contains("air_date") && ss["air_date"].is_string()
                             ? ss["air_date"].get<std::string>()
                             : "";
      season.m_id = ss.value("id", 0);
      season.m_sname = ss.contains("name") && ss["name"].is_string()
                           ? ss["name"].get<std::string>()
                           : "";
      season.m_voteAverage = ss.value("vote_average", 0.0);
      season.m_season = ss.value("season_number", 0);
      season.m_posterPath =
          ss.contains("poster_path") && ss["poster_path"].is_string()
              ? ss["poster_path"].get<std::string>()
              : "";
      season.m_overview = ss.contains("overview") && ss["overview"].is_string()
                              ? ss["overview"].get<std::string>()
                              : "";
      season.m_episodesAmount = ss.value("episode_count", 0);
      seasons.push_back(season);
    }

    serie.m_seasons = seasons;
  }

  serie.show();
  return serie;
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

    std::vector<Entry> entries = readCSV(getSeriesCSVPath());
    Entry *entry = FindEntryById(entries, serie.m_id);

    if (entry) {
      serie.m_voteAverage = entry->rating;
      serie.m_voteCount = entry->count;
    } else {
      serie.m_voteAverage = 0.0;
      serie.m_voteCount = 0;
    }

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
