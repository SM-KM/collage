// TODO: Add loading data from CSV
// TODO: Add support to load clips or entire movies using mov-cli and in the
// case of anime just directly get it
// TODO: Add the ability to rate any video
// TODO: Save ratings for the videos persistant
// WARN: Maybe add some kind of registering to know from who are the ratings
// depending from where we get the data

#include <iostream>
#include <numeric>
#include <string>
#include <vector>
class Video {
protected:
  int id;
  int duration; // In seconds
  std::string name, genre;
  std::vector<int> ratings;

public:
  Video(int id, std::string name, int duration, std::string genre)
      : id(id), name(name), duration(duration), genre(genre) {}

  void addRating(int rating) {
    if (rating >= 1 && rating <= 5)
      ratings.push_back(rating);
  }

  double getAverageRatings() const {
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

class Episode {};
class Series : public Video {};

int main(int argc, char *argv[]) { return 0; }
