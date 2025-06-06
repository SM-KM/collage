// TODO: Add loading data from CSV
// TODO: Add support to load clips or entire movies using mov-cli and in the
// case of anime just directly get it
// TODO: Add the ability to rate any video
// TODO: Save ratings for the videos persistant
// WARN: Maybe add some kind of registering to know from who are the ratings
// depending from where we get the data

// TODO: Implement search on the movie or series specific id data, and organize
// on the classes
// TODO: Create the bash scripts for custom fzf for results and using mov-cli
// for bringing videos and anime

#include "util/argHandlers.h"
#include "util/dotenv.h"
#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>

#define PROMPT ">> "

void runCommand(const std::vector<std::string> &args) {
  if (args.empty())
    return;

  if (args[0] == "-s" || args[0] == "-search") {
    if (args.size() > 2) {
      LoadVideoSearch(args[1], args[2]);
    } else {
      std::cerr << "Missing argument for search\n";
    }
  } else if (args[0] == "-series") {
    LoadPopularSeries();
  } else if (args[0] == "-p") {
    LoadPopularMovies();
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
