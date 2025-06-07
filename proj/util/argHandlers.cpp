#include "../requests/requests.h"
#include "argHandlers.h"
#include "types.h"
#include <cstdio>
#include <cstdlib>

// TODO: Implement the rest of the searching, and for the results pop in new
// buffer and inside fzf for easy searching and data visualization

void LoadMoviesReq(MoviesFlags flag) {
  if (flag == MoviesFlags::TOP_RATED) {
    getTopRatedMovies();
  } else if (flag == MoviesFlags::POPULAR) {
    getLatestPopularMovies();
  } else {
    // Fallback
    std::cerr << "Didnt parse flag correctly";
  }
};

void LoadMovieById(MoviesFlags flag, int id) {
  std::cout << "Loading movie with ID: " << id
            << " and flag: " << static_cast<int>(flag) << "\n";
  getMovieById(id);
};
void LoadSerieById(SeriesFlags flag, int id) {
  std::cout << "loading series with id: " << id
            << " and flag: " << static_cast<int>(flag) << "\n";
  getSerieById(id);
};

void LoadSeriesReq(SeriesFlags flag) {
  if (flag == SeriesFlags::POPULAR) {
    getLatestPopularSeries();
  } else if (flag == SeriesFlags::TOP_RATED) {
    getTopRatedSeries();
  } else if (flag == SeriesFlags::AIRING_TODAY) {
    getAiringTodaySeries();
  } else {
    // Fallback
    std::cerr << "Didnt parse flag correctly";
  }
}

void LoadVideoSearch(std::string searchStr) {
  printf("Searching for: %s... \n\n", searchStr.c_str());
  // TODO: Implement this functionality

  system("pwd");
  std::string command = "./search.sh \"" + searchStr + "\"";
  int result = system(command.c_str());
  std::cout << "Running command: " << command << std::endl;

  if (result != 0) {
    std::cerr << "Error: Bash script failed with code " << result << std::endl;
  }
}
