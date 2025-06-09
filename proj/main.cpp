// TODO: Add loading data from CSV
// TODO: Add support to load clips or entire movies using mov-cli and in the
// case of anime just directly get it
// TODO: Add the ability to rate any video TODO: Save ratings for the videos
// persistant
// WARN: Maybe add some kind of registering to know from who are the
// ratings depending from where we get the data

// TODO: Implement search on the movie or series specific id data, and organize
// on the classes
// TODO: Create the bash scripts for custom fzf for results and using mov-cli
// for bringing videos and anime

// TODO: Run this program through a Go program, that handles UI with Bubble Tea
// like terminal.shop

#include "util/argHandlers.h"
#include "util/dotenv.h"
#include "util/types.h"
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

  for (size_t i = 0; i < args.size(); ++i) {
    const std::string &arg = args[i];

    if (arg == "-s" || arg == "-search") {
      std::vector<std::string> terms;
      ++i;

      while (i < args.size() && args[i][0] != '-') {
        terms.push_back(args[i]);
        ++i;
      }
      --i;

      if (!terms.empty()) {
        std::string query;
        for (const auto &word : terms)
          query += word + " ";

        query.pop_back();
        LoadVideoSearch(query);
      } else {
        std::cerr << "Missing search terms \n";
      }
    } else if (arg == "-ss") {
      std::vector<std::string> flags;
      std::string id;

      ++i;
      while (i < args.size() && args[i][0] == '-') {
        if (args[i] == "-id") {
          if (i + 1 < args.size()) {
            id = args[++i];
          } else {
            std::cerr << "Missing value for -id \n";
          }
        } else {
          flags.push_back(args[i]);
        }
        ++i;
      }
      --i;

      if (!flags.empty()) {
        for (const auto &flagstr : flags) {
          SeriesFlags flag = parseSeriesFlag(flagstr);

          try {
            if (!id.empty()) {
              int movieId = std::stoi(id);
              LoadSerieById(flag, movieId);
            } else {
              LoadSeriesReq(flag);
            }
          } catch (const std::exception &e) {
            std::cerr << "Invalid series id: " << id << " (" << e.what()
                      << ")\n";
          }
        }
      }
    } else if (arg == "-m") {
      std::vector<std::string> flags;
      std::string id;
      ++i;

      while (i < args.size() && args[i][0] == '-') {
        if (args[i] == "-id") {
          if (i + 1 < args.size()) {
            id = args[++i];
          } else {
            std::cerr << "Missing value for -id \n";
          }
        } else {
          flags.push_back(args[i]);
        }
        ++i;
      }
      --i;

      if (!flags.empty()) {
        for (const auto &flagstr : flags) {
          MoviesFlags flag = parseMoviesFlag(flagstr);

          try {

            if (!id.empty()) {
              int movieId = std::stoi(id);
              LoadMovieById(flag, movieId);
            } else {
              LoadMoviesReq(flag);
            }
          } catch (const std::exception &e) {
            std::cerr << "Invalid movie id: " << id << " (" << e.what()
                      << ")\n";
          }
        }
      } else {
        std::cerr << "Missing movie flag \n";
      }
    } else if (arg == "-r" || arg == "-rate") {
      std::vector<std::string> flags;
      std::string id;

      ++i;
      while (i < args.size() && args[i][0] == '-') {
        if (args[i] == "-id") {
          if (i + 1 < args.size()) {
            id = args[++i];
          } else {
            std::cerr << "Missing value for -id \n";
          }
        } else {
          flags.push_back(args[i]);
        }
        ++i;
      }
      --i;

      if (!flags.empty()) {
        for (const auto &flagstr : flags) {
          SeriesFlags flag = parseSeriesFlag(flagstr);

          try {
            if (!id.empty()) {
              int movieId = std::stoi(id);
              LoadSerieById(flag, movieId);
            } else {
              LoadSeriesReq(flag);
            }
          } catch (const std::exception &e) {
            std::cerr << "Invalid series id: " << id << " (" << e.what()
                      << ")\n";
          }
        }
      }
    } else if (arg == "exit" || arg == "quit") {
      std::exit(0);
    } else if (arg[0] == '-') {
      std::cerr << "Unknown flag: " << arg << "\n";
    }
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
