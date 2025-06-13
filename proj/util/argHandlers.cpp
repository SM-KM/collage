#include "csv.h"
#include "../requests/requests.h"
#include "argHandlers.h"
#include "types.h"
#include <cstdio>
#include <cstdlib>
#include <optional>

void RateContent(int id, int rating, VideoType type) {
  std::cout << "Rating movie: " << id << " with: " << rating << "\n";
  updateOrAddRating(getCSVPath(type), id, rating);
};

void LoadReq(VideoType type, Flags flag, bool byId, std::optional<int> id,
             std::optional<int> genreId) {
  getContent(type, flag, byId, id, genreId);
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
