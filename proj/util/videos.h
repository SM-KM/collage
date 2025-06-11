#ifndef VIDEOS_H
#define VIDEOS_H

#include "types.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

class Video {
public:
  int m_id;
  int m_duration;
  std::string m_name, m_genre;
  std::vector<int> m_ratings;

public:
  Video() {};
  Video(int id, std::string name, int duration, std::string genre)
      : m_id(id), m_name(name), m_duration(duration), m_genre(genre) {}

  void addRating(int rating) {
    if (rating >= 1 && rating <= 5)
      m_ratings.push_back(rating);
  }

  virtual double getAverageRatings() const {
    if (m_ratings.empty())
      return 0.0;

    return std::accumulate(m_ratings.begin(), m_ratings.end(), 0.0) /
           m_ratings.size();
  }

  virtual void show() const = 0;
  virtual ~Video() = default;
};

class Movie : public Video {
public:
  bool m_adult;
  std::vector<int> m_genreIds;
  std::vector<MovieGenre> m_genres;
  std::string m_backdropPath, m_posterPath;
  std::string m_lang, m_overview;
  int m_voteCount;
  double m_voteAverage;

  Movie() : Video() {};
  Movie(int id, std::string name, int duration, std::string genre, bool adult,
        std::vector<int> genreIds, std::string backdropPath,
        std::string posterPath, std::string lang, std::string overview,
        int voteCount, double voteAverage)
      : Video(id, name, duration, genre) {

    this->m_adult = adult;
    this->m_genreIds = genreIds;
    this->m_backdropPath = backdropPath;
    this->m_posterPath = posterPath;
    this->m_lang = lang;
    this->m_overview = overview;
    this->m_voteCount = voteCount;
    this->m_voteAverage = voteAverage;
  }

  void show() const override {
    std::cout << "Movie: " << m_name << " (ID: " << m_id << ")\n"
              << "Duration: " << m_duration << " mins\n"
              << "Adult: " << (m_adult ? "Yes" : "No") << "\n"
              << "Language: " << m_lang << "\n"
              << "Overview: " << m_overview << "\n"
              << "Vote Count: " << m_voteCount << "\n"
              << "Vote Average: " << m_voteAverage << "\n"
              << "Average User Rating: " << getAverageRatings() << "\n"
              << "Backdrop Path: " << m_backdropPath << "\n"
              << "Poster Path: " << m_posterPath << "\n"
              << "Genre IDs: ";
    for (auto id : m_genreIds)
      std::cout << id << " ";
    std::cout << "\n\n";
    std::cout << "Genres: ";
    for (auto g : m_genres) {
      std::cout << g.id << " ";
      std::cout << g.name << " ";
    }
    std::cout << "\n\n";
  }

  double getAverageRatings() const override { return 0.0; };
};

class Episode {
  int m_episode_num;
  int m_length;
  std::string m_title;

public:
  Episode(int num = 0, int length = 0, std::string title = "")
      : m_episode_num(num), m_length(length), m_title(title) {}

  void show() const {
    std::cout << "  Episode " << m_episode_num << ": " << m_title << " ("
              << m_length << " mins)\n";
  }
};

class Season {
public:
  // Local
  int m_rating, m_voteCount;

  // From API

  int m_id;
  int m_season, m_episodesAmount;
  double m_voteAverage;
  std::string m_posterPath, m_airDate, m_overview;
  std::string m_sname;
  std::vector<Episode> m_episodes;

  Season() {};
  Season(int season, int rating, std::vector<Episode> episodes)
      : m_episodes(episodes), m_season(season), m_rating(rating) {}

  int getRating() const { return m_rating; }
  void show() const {
    std::cout << "Season " << m_season
              << " | Episodes amount: " << m_episodesAmount
              << " | Vote average: " << m_voteAverage << "\n";
    for (const auto &ep : m_episodes) {
      ep.show();
    }
    std::cout << "\n";
  }
};

class Series : public Video {
public:
  bool m_adult;
  std::string m_overview;
  std::string m_backdrop_path;
  std::string m_firstAirDate, m_lastAirDate;
  std::vector<Season> m_seasons;
  std::vector<SeriesGenre> m_genres;
  std::vector<std::string> m_langs;
  double m_voteAverage;
  int m_voteCount;

  int m_episodesAmount, m_seasonsAmount;

  Series() : Video() {};
  Series(int id, std::string name, int duration, std::string genre)
      : Video(id, name, duration, genre) {}

  void addEpisode() {}
  double getAverageRatings() const override {
    if (m_seasons.empty())
      return 0.0;

    double sum = 0;
    for (const auto &e : m_seasons) {
      sum += e.getRating();
    }

    return sum / m_seasons.size();
  }

  void show() const override {
    std::cout << "Series: " << m_name << " (ID: " << m_id << ")\n"
              << "Genre: " << m_genre << "\n"
              << "Name: " << m_name << "\n"
              << "Adult: " << (m_adult ? "Yes" : "No") << "\n"
              << "Overview: " << m_overview << "\n"
              << "First Air Date: " << m_firstAirDate << "\n"
              << "Last Air Date: " << m_lastAirDate << "\n"
              << "Episodes: " << m_episodesAmount << "\n"
              << "Seasons: " << m_seasonsAmount << "\n"
              << "Vote count: " << m_voteCount << "\n"
              << "Average Rating: " << m_voteAverage << "\n"
              << "Languages: ";
    for (const auto &lang : m_langs)
      std::cout << lang << " ";
    std::cout << "\nGenres: ";
    for (const auto &genre : m_genres)
      std::cout << genre.name << " ";
    std::cout << "\n\n";

    for (const auto &season : m_seasons) {
      season.show();
    }
  }
};

#endif // !VIDEOS_H
