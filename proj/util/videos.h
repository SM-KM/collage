#ifndef VIDEOS_H
#define VIDEOS_H

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
public:
  Movie(int id, std::string name, int duration, std::string genre)
      : Video(id, name, duration, genre) {}

  void show() const override {
    std::cout << "Movie: " << name << " | Genre: " << genre
              << " | Average rating: " << getAverageRatings() << "\n";
  }
};

class Episode {
  std::string title;
  int season;
  int rating;

public:
  Episode(std::string title, int season, int rating)
      : title(title), season(season), rating(rating) {}

  int getRating() const { return rating; }
  void show() const {
    std::cout << " Episode: " << title << " | Season: " << season
              << " | Rating: " << rating << "\n";
  }
};

class Series : public Video {
  std::vector<Episode> episodes;

public:
  Series(int id, std::string name, int duration, std::string genre)
      : Video(id, name, duration, genre) {}

  void addEpisode(std::string title, int season, int rating) {
    episodes.emplace_back(title, season, rating);
  }

  double getAverageRatings() const override {
    if (episodes.empty())
      return 0.0;

    double sum = 0;
    for (const auto &e : episodes) {
      sum += e.getRating();
    }

    return sum / episodes.size();
  }

  void show() const override {
    std::cout << "Serie: " << name << " | Genre: " << genre
              << " | Average rating: " << getAverageRatings() << "\n";
    for (const auto &e : episodes)
      e.show();
  }
};

#endif // !VIDEOS_H
