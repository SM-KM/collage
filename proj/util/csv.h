#ifndef CSV_H
#define CSV_H

#include "types.h"
#include "videos.h"
#include <filesystem>
#include <string>
#include <vector>

struct Entry {
  int id;
  double rating;
  int count;

  Entry() : id(0), rating(0.0), count(0) {}
  Entry(const int &id, double rating, int count)
      : id(id), rating(rating), count(count) {}
};

std::vector<Entry> readCSV(const std::filesystem::path &filepath);
void writeCSV(const std::filesystem::path &filepath,
              const std::vector<Entry> &entries);

void appendEntry(const std::filesystem::path &filePath, const Entry &entry);
bool updateRating(std::vector<Entry> entries, const std::string &targetId,
                  const std::string &newRating);

void updateOrAddRating(const std::filesystem::path &path, const int id,
                       const int rating);

Entry *FindEntryById(std::vector<Entry> &entries, int targetId);
std::filesystem::path getCSVPath(VideoType type);

#endif // !CSV_H
