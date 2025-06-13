#include "requests/requests.h"
#include "util/argHandlers.h"
#include "util/dotenv.h"
#include "util/play.h"
#include "util/types.h"
#include "util/utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <curl/curl.h>
#include <curl/easy.h>
#include <exception>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>

#define PROMPT ">> "

void runCommand(const std::vector<std::string> &args,
                const std::map<std::string, std::vector<Genre>> &genres) {
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
      int genreId = -1;
      bool play = false;
      std::string genreName;

      ++i;
      while (i < args.size()) {
        const std::string &next = args[i];

        if (next == "-id" && i + 1 < args.size()) {
          id = args[++i];
        } else if (next == "--play") {
          play = true;
        } else if (next == "--genre") {
          if (i + 1 < args.size()) {
            genreName = args[++i];
            flags.push_back(next);
          } else {
            std::cerr << "Error: --genre flag requires a genre name.\n";
            return;
          }
        } else if (next[0] == '-') {
          flags.push_back(next);
        } else {
          --i;
          break;
        }
        ++i;
      }
      --i;

      if (!genreName.empty()) {
        std::vector<Genre> relevantGenres = genres.at("series");

        try {
          auto it =
              std::find_if(relevantGenres.begin(), relevantGenres.end(),
                           [&](const Genre &g) {
                             return to_lower(g.name) == to_lower(genreName);
                           });

          if (it != relevantGenres.end()) {
            genreId = it->id;
          } else {
            std::cerr << "Unknown genre for this type: " << genreName << "\n";
            return;
          }

        } catch (const std::out_of_range &e) {
          std::cerr << "Genre list not found for this type\n";
          return;
        }
      }

      for (const auto &flagstr : flags) {
        std::cout << flagstr << "\n";
        try {
          Flags flag = to_flag(flagstr);
          if (!id.empty()) {
            LoadReq(VideoType::SERIE, flag, true, std::stoi(id));
          } else if (genreId != -1) {
            LoadReq(VideoType::SERIE, flag, false, 0, genreId);
          } else {
            LoadReq(VideoType::SERIE, flag);
          }
        } catch (const std::exception &e) {
          std::cerr << "Invalid series flag or id: " << e.what() << "\n";
        }
      }

      if (play) {
        if (!id.empty()) {
          try {
            PlayContentById(std::stoi(id));
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
      int genreId = -1;
      bool play = false;
      std::string genreName;

      ++i;
      while (i < args.size()) {
        const std::string &next = args[i];

        if (next == "-id" && i + 1 < args.size()) {
          id = args[++i];
        } else if (next == "--play") {
          play = true;
        } else if (next == "--genre") {
          if (i + 1 < args.size()) {
            genreName = args[++i];
            flags.push_back(next);
          } else {
            std::cerr << "Error: --genre flag requires a genre name.\n";
            return;
          }
        } else if (next[0] == '-') {
          flags.push_back(next);
        } else {
          --i;
          break;
        }
        ++i;
      }
      --i;

      if (!genreName.empty()) {
        std::vector<Genre> relevantGenres = genres.at("movies");

        try {
          auto it =
              std::find_if(relevantGenres.begin(), relevantGenres.end(),
                           [&](const Genre &g) { return g.name == genreName; });

          if (it != relevantGenres.end()) {
            std::cout << genreId << "\n";
            genreId = it->id;
          } else {
            std::cerr << "Unknown genre for this type: " << genreName << "\n";
            return;
          }

        } catch (const std::out_of_range &e) {
          std::cerr << "Genre list not found for this type\n";
          return;
        }
      }

      for (const auto &flagstr : flags) {
        try {
          Flags flag = to_flag(flagstr);
          if (!id.empty()) {
            LoadReq(VideoType::MOVIE, flag, true, std::stoi(id));
          } else if (genreId != -1) {
            LoadReq(VideoType::MOVIE, flag, false, 0, genreId);
          } else {
            LoadReq(VideoType::MOVIE, flag);
          }

        } catch (const std::exception &e) {
          std::cerr << "Invalid movie flag or id: " << e.what() << "\n";
        }
      }

      if (play) {
        if (!id.empty()) {
          try {
            int movieId = std::stoi(id);
            PlayContentById(movieId);
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
            RateContent(id, rating, VideoType::MOVIE);
          } else if (typeFlag == "--ss") {
            RateContent(id, rating, VideoType::SERIE);
          }

        } catch (const std::exception &e) {
          std::cerr << "Invalid ID or rating format: " << e.what() << "\n";
        }
      } else {
        std::cerr << "Usage: -rate <id> <rating from 1 to 10> --m|--ss\n";
      }

    } else if (arg == "-genres") {
      if (genres.empty()) {
        std::cerr << "No se pudieron obtener los géneros.\n";
        return;
      }

      std::cout << "\nMovie Genres:\n";
      for (const auto &genre : genres.at("movies")) {
        std::cout << "  • [" << genre.id << "] " << genre.name << "\n";
      }

      std::cout << "\nSeries Genres:\n";
      for (const auto &genre : genres.at("series")) {
        std::cout << "  • [" << genre.id << "] " << genre.name << "\n";
      }

      std::cout << std::endl;

    } else if (arg == "exit" || arg == "quit") {
      std::exit(0);
    }
  }
}

int main(int argc, char *argv[]) {
  loadDotEnv();
  std::string line;
  std::map<std::string, std::vector<Genre>> genres = getAllGenres();

  while (true) {
    std::cout << PROMPT;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> args;
    while (iss >> token) {
      args.push_back(token);
    }

    runCommand(args, genres);
  }

  return 0;
}
