#include "argHandlers.h"
#include "../requests/requests.h"
#include "types.h"
#include <cstdio>

// TODO: Implement the rest of the searching, and for the results pop in new
// buffer and inside fzf for easy searching and data visualization

void LoadMoviesReq() {};
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

void LoadVideoSearch(std::string searchStr, std::string type) {
  printf("Searching for: %s [%s]... \n\n", searchStr.c_str(), type.c_str());
  // TODO: Implement this functionality
}
