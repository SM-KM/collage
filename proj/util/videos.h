#ifndef VIDEOS_H
#define VIDEOS_H

#include "types.h"
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class Video {
protected:
  int id;
  int duration;
  std::string name, genre;
  std::vector<int> ratings;

public:
  Video(int id, std::string name, int duration, std::string genre)
      : id(id), name(name), duration(duration), genre(genre) {}

  void addRating(int rating) {
    if (rating >= 1 && rating <= 5)
      ratings.push_back(rating);
  }

  virtual double getAverageRatings() const {
    if (ratings.empty())
      return 0.0;

    return std::accumulate(ratings.begin(), ratings.end(), 0.0) /
           ratings.size();
  }

  virtual void show() const = 0;
  virtual ~Video();
};

class Movie : public Video {
  bool adult;
  std::vector<int> genreIds;
  std::string backdropPath, posterPath;
  std::string lang, overview;
  int voteCount, voteAverage;

public:
  Movie(int id, std::string name, int duration, std::string genre, bool adult,
        std::vector<int> genreIds, std::string backdropPath,
        std::string posterPath, std::string lang, std::string overview,
        int voteCount, int voteAverage)
      : Video(id, name, duration, genre) {

    this->adult = adult;
    this->genreIds = genreIds;
    this->backdropPath = backdropPath;
    this->posterPath = posterPath;
    this->lang = lang;
    this->overview = overview;
    this->voteCount = voteCount;
    this->voteAverage = voteAverage;
  }

  void show() const override {
    std::cout << "Movie: " << name << " | Genre: " << genre
              << " | Average rating: " << getAverageRatings() << "\n";
  }
};

class Episode {
  int episodeNum;
  int length;
  std::string title;

public:
};

class Season {
  int m_season, m_rating, m_voteAverage;
  std::vector<Episode> m_episodes;

public:
  Season(int season, int rating, std::vector<Episode> episodes)
      : m_episodes(episodes), m_season(season), m_rating(rating) {}

  int getRating() const { return m_rating; }
  void show() const {}
};

class Series : public Video {
  bool adult, inProduction;
  std::string overview;
  std::string backdropPath;
  std::string m_firstAirDate, m_lastAirDate;
  std::vector<Season> seasons;
  std::vector<SeriesGenre> genres;
  std::vector<std::string> langs;

  int m_episodesAmount, m_seasonsAmount;

public:
  Series(int id, std::string name, int duration, std::string genre)
      : Video(id, name, duration, genre) {}

  void addEpisode(std::string title, int season, int rating) {
    seasons.emplace_back(title, season, rating);
  }

  double getAverageRatings() const override {
    if (seasons.empty())
      return 0.0;

    double sum = 0;
    for (const auto &e : seasons) {
      sum += e.getRating();
    }

    return sum / seasons.size();
  }

  void show() const override {
    std::cout << "Serie: " << name << " | Genre: " << genre
              << " | Average rating: " << getAverageRatings() << "\n";
    for (const auto &e : seasons)
      e.show();
  }
};

#endif // !VIDEOS_H
