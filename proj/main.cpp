// TODO: Add loading data from CSV
// TODO: Add support to load clips or entire movies using mov-cli and in the
// case of anime just directly get it
// TODO: Add the ability to rate any video
// TODO: Save ratings for the videos persistant
// WARN: Maybe add some kind of registering to know from who are the ratings
// depending from where we get the data

// TODO: Implement search on the movie or series specific id data, and organize
// on the classes

#include "requests/requests.h"
#include "util/dotenv.h"
#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numeric>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>

#define PROMPT ">> "

// Get the data from the movies and series from TMCP
std::string url =
    "https://api.themoviedb.org/3/movie/popular?language=en-US&page=1";

class Video {
protected:
  int id;
  int duration; // in seconds
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

void getLatestPopularMovies() {
  std::string apiKey = std::getenv("TMDB_API_KEY");

  if (apiKey.empty()) {
    std::cerr << "Api key not found. Exiting. \n";
  }

  std::string response = fetchFromTMDB(apiKey, url);
  try {
    nlohmann::json data = nlohmann::json::parse(response);
    std::cout << data["results"];
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "JSON parsing error: " << e.what() << "\n";
  }
}

void runCommand(const std::vector<std::string> &args) {
  if (args.empty())
    return;

  if (args[0] == "-s" || args[0] == "-search") {
    if (args.size() > 1) {
      std::cout << "Searching for: " << args[1] << "\n";
      // TODO: Implement specific search
    } else {
      std::cerr << "Missing argument for search\n";
    }
  } else if (args[0] == "-r") {
    std::cout << "Ratings for popular movies: \n\n";
  } else if (args[0] == "-v") {
    std::cout << "Verbose mode enabled \n";
    // TODO: Implment the flag
  } else if (args[0] == "exit" || args[0] == "quit") {
    std::exit(0);
  } else {
    std::cerr << "Unknown command: " << args[0] << "\n";
  }
}

int main(int argc, char *argv[]) {
  loadDotEnv();
  std::string line;

  while (true) {
    std::cout << PROMPT;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> args;
    while (iss >> token) {
      args.push_back(token);
    }
    runCommand(args);
  }

  return 0;
}
