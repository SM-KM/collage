#include "csv.h"
#include "../requests/requests.h"
#include "argHandlers.h"
#include "types.h"
#include <cstdio>
#include <cstdlib>
#include <optional>

// TODO: Implement the rest of the searching, and for the results pop in new
// buffer and inside fzf for easy searching and data visualization

void RateMovieById(int id, int rating) {
  std::cout << "Rating movie: " << id << " with: " << rating << "\n";
  updateOrAddRating(getMoviesCSVPath(), id, rating);
};

void RateSeriesById(int id, int rating) {
  std::cout << "Rating series: " << id << " with: " << rating << "\n";
  updateOrAddRating(getSeriesCSVPath(), id, rating);
}

void LoadReq(VideoType type, Flags flag, bool byId, std::optional<int> id) {
  getContent(type, flag, byId, id);
}

void LoadVideoSearch(std::string searchStr) {
  printf("Searching for: %s... \n\n", searchStr.c_str());
  system("pwd");
  std::string command = "./search.sh \"" + searchStr + "\"";
  int result = system(command.c_str());
  std::cout << "Running command: " << command << std::endl;

  if (result != 0) {
    std::cerr << "Error: Bash script failed with code " << result << std::endl;
  }
}
