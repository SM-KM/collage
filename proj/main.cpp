// TODO: Add support to load clips or entire movies using mov-cli and in the
// case of anime just directly get it
// TODO: Run this program through a Go program, that handles UI with Bubble Tea
// like terminal.shop

#include "util/argHandlers.h"
#include "util/dotenv.h"
#include "util/play.h"
#include "util/types.h"
#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <exception>
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
        terms.push_back(args[i++]);
      }
      --i;

      if (!terms.empty()) {
        std::string query;
        for (const auto &word : terms)
          query += word + " ";
        query.pop_back();
        LoadVideoSearch(query);
      } else {
        std::cerr << "Missing search terms\n";
      }

    } else if (arg == "-ss") {
      std::vector<std::string> flags;
      std::string id;
      bool play = false;

      ++i;
      while (i < args.size()) {
        const std::string &next = args[i];

        if (next == "-id" && i + 1 < args.size()) {
          id = args[++i];
        } else if (next == "--play") {
          play = true;
        } else if (next[0] == '-') {
          flags.push_back(next);
        } else {
          --i;
          break;
        }
        ++i;
      }
      --i;

      for (const auto &flagstr : flags) {
        try {
          SeriesFlags flag = parseSeriesFlag(flagstr);
          if (!id.empty()) {
            int serieId = std::stoi(id);
            LoadSerieById(flag, serieId);
          } else {
            LoadSeriesReq(flag);
          }
        } catch (const std::exception &e) {
          std::cerr << "Invalid series flag or id: " << e.what() << "\n";
        }
      }

      if (play) {
        if (!id.empty()) {
          try {
            int serieId = std::stoi(id);
            PlaySeriesById(serieId);
          } catch (const std::exception &e) {
            std::cerr << "Invalid series id: " << id << " (" << e.what()
                      << ")\n";
          }
        } else {
          std::cerr << "--play was used but no -id provided\n";
        }
      }

    } else if (arg == "-m") {
      std::vector<std::string> flags;
      std::string id;
      bool play = false;

      ++i;
      while (i < args.size()) {
        const std::string &next = args[i];

        if (next == "-id" && i + 1 < args.size()) {
          id = args[++i];
        } else if (next == "--play") {
          play = true;
        } else if (next[0] == '-') {
          flags.push_back(next);
        } else {
          --i;
          break;
        }
        ++i;
      }
      --i;

      for (const auto &flagstr : flags) {
        try {
          MoviesFlags flag = parseMoviesFlag(flagstr);
          if (!id.empty()) {
            int movieId = std::stoi(id);
            LoadMovieById(flag, movieId);
          } else {
            LoadMoviesReq(flag);
          }
        } catch (const std::exception &e) {
          std::cerr << "Invalid movie flag or id: " << e.what() << "\n";
        }
      }

      if (play) {
        if (!id.empty()) {
          try {
            int movieId = std::stoi(id);
            PlayMovieById(movieId);
          } catch (const std::exception &e) {
            std::cerr << "Invalid movie id: " << id << " (" << e.what()
                      << ")\n";
          }
        } else {
          std::cerr << "--play was used but no -id provided\n";
        }
      }

    } else if (arg == "-r" || arg == "-rate") {
      if (i + 2 < args.size()) {
        std::string idStr = args[++i];
        std::string ratingStr = args[++i];
        std::string typeFlag = args[++i];

        try {
          int id = std::stoi(idStr);
          int rating = std::stoi(ratingStr);

          if (rating < 1 || rating > 10) {
            std::cerr << "Rating must be between 1 and 10\n";
          } else if (typeFlag == "--m") {
            RateMovieById(id, rating);
          } else if (typeFlag == "--ss") {
            RateSeriesById(id, rating);
          }

        } catch (const std::exception &e) {
          std::cerr << "Invalid ID or rating format: " << e.what() << "\n";
        }
      } else {
        std::cerr << "Usage: -rate <id> <rating from 1 to 10> --m|--ss\n";
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
