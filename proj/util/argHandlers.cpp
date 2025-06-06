#include "argHandlers.h"
#include "../requests/requests.h"
#include <cstdio>
#include <iostream>

void LoadPopularMovies() {
  std::cout << "Ratings of popular movies: \n\n";
  getLatestPopularMovies();
}

void LoadPopularSeries() {
  std::cout << "Ratings of popular movies: \n\n";
  getLatestPopularSeries();
}

void LoadVideoSearch(std::string searchStr, std::string type) {
  printf("Searching for: %s [%s]... \n\n", searchStr.c_str(), type.c_str());
  // TODO: Implement this functionality
}

// TODO: Implement the rest of the searching, and for the results pop in new
// buffer and inside fzf for easy searching and data visualization
