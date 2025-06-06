#ifndef VIDEOS_H
#define VIDEOS_H

#include "types.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numeric>
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
  std::string m_backdropPath, m_posterPath;
  std::string m_lang, m_overview;
  int m_voteCount, m_voteAverage;

public:
  Movie() : Video() {};
  Movie(int id, std::string name, int duration, std::string genre, bool adult,
        std::vector<int> genreIds, std::string backdropPath,
        std::string posterPath, std::string lang, std::string overview,
        int voteCount, int voteAverage)
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
    std::cout << "Movie: " << m_name << " | Genre: " << m_genre
              << " | Average rating: " << getAverageRatings() << "\n";
  }
};

class Episode {
  int m_episode_num;
  int m_length;
  std::string m_title;

public:
};

class Season {
  int m_id;
  int m_season, m_rating, m_voteAverage, m_voteCount;
  std::string m_posterPath, m_airDate, m_overview, m_status, m_tagline;
  std::vector<Episode> m_episodes;

public:
  Season(int season, int rating, std::vector<Episode> episodes)
      : m_episodes(episodes), m_season(season), m_rating(rating) {}

  int getRating() const { return m_rating; }
  void show() const {}
};

class Series : public Video {
public:
  bool m_adult, m_in_production;
  std::string m_overview;
  std::string m_backdrop_path;
  std::string m_firstAirDate, m_lastAirDate;
  std::vector<Season> m_seasons;
  std::vector<SeriesGenre> m_genres;
  std::vector<std::string> m_langs;

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
    std::cout << "Serie: " << m_name << " | Genre: " << m_genre
              << " | Average rating: " << getAverageRatings() << "\n";
    for (const auto &e : m_seasons)
      e.show();
  }
};

#endif // !VIDEOS_H
