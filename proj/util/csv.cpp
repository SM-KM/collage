#include "csv.h"
#include "types.h"
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

std::filesystem::path getCSVPath(VideoType type) {
  std::filesystem::path csvDir = "csv";
  std::filesystem::path filename =
      type == VideoType::MOVIE ? "movies.csv" : "series.csv";
  std::filesystem::path path = csvDir / filename;
  return path;
}

std::filesystem::path getMoviesCSVPath() {
  std::filesystem::path csvDir = "csv";
  std::filesystem::path filename = "movies.csv";
  std::filesystem::path path = csvDir / filename;
  return path;
}

std::filesystem::path getSeriesCSVPath() {
  std::filesystem::path csvDir = "csv";
  std::filesystem::path filename = "series.csv";
  std::filesystem::path path = csvDir / filename;
  return path;
}

std::vector<Entry> readCSV(const std::filesystem::path &filepath) {
  std::ifstream file(filepath);
  std::string line;
  std::getline(file, line);
  std::unordered_map<int, Entry> entriesMap;

  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    std::stringstream ss(line);
    std::string idStr, ratingStr, countStr;
    int id;
    double rating = 0;
    int count = 1;

    if (std::getline(ss, idStr, ',') && std::getline(ss, ratingStr, ',')) {
      try {
        id = std::stoi(idStr);
        rating = std::stoi(ratingStr);

        if (std::getline(ss, countStr, ',')) {
          count = std::stoi(countStr);
        }

        // Merge duplicated on read
        auto it = entriesMap.find(id);
        if (it == entriesMap.end()) {
          entriesMap[id] = Entry{id, rating, count};
        } else {
          double totalRating = it->second.rating * it->second.count;
          totalRating += rating * count;
          it->second.count += count;
          it->second.rating = totalRating / it->second.count;
        }
      } catch (const std::exception &e) {
        std::cerr << "dad" << e.what();
      }

    } else {
      std::cerr << "Warning: skipping malformed line: " << line << "\n";
    }
  }

  std::vector<Entry> entries;
  entries.reserve(entriesMap.size());

  for (auto &pair : entriesMap) {
    entries.push_back(pair.second);
  }

  return entries;
}

void writeCSV(const std::filesystem::path &filepath,
              const std::vector<Entry> &entries) {
  std::ofstream file(filepath);
  file << "id,rating,count\n";

  for (const auto &entry : entries) {
    file << entry.id << "," << entry.rating << "," << entry.count << "\n";
  }
}

bool updateRating(std::vector<Entry> &entries, const int &targetId,
                  const int &newRating) {
  for (auto &entry : entries) {
    if (entry.id == targetId) {
      double totalRating = entry.rating * entry.count;
      totalRating += newRating;
      entry.count += 1;
      entry.rating = totalRating / entry.count;
      return true;
    }
  }

  return false;
}

void appendEntry(const std::filesystem::path &filePath, const Entry &entry) {
  std::ofstream file(filePath, std::ios::app);
  file << entry.id << "," << entry.rating << "," << entry.count << "\n";
}

void updateOrAddRating(const std::filesystem::path &path, const int id,
                       const int rating) {
  std::vector<Entry> entries = readCSV(path);
  if (!updateRating(entries, id, rating)) {
    entries.emplace_back(id, rating, 1);
    std::cout << "Created a new rating for: " << id << "\n";
  } else {
    std::cout << "Updated the rating from: " << id << "\n";
  }

  writeCSV(path, entries);
};

Entry *FindEntryById(std::vector<Entry> &entries, int targetId) {
  for (auto &entry : entries) {
    if (entry.id == targetId) {
      return &entry;
    }
  }

  return nullptr;
}
