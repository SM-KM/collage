#ifndef VIDEOS_H
#define VIDEOS_H

#include <iostream>
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

  void addRating(int rating);
  virtual double getAverageRatings() const;
  virtual void show() const = 0;
  virtual ~Video();
};

class Movie : public Video {
public:
  Movie(int id, std::string name, int duration, std::string genre)
      : Video(id, name, duration, genre) {}

  void show() const override;
};

#endif // !VIDEOS_H
