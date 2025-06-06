#ifndef TYPES_H
#define TYPES_H

#include <string>

enum Gender { FEM, MASC };

struct CreatedBy {
  int id;
  std::string name;
  std::string profilePath;
  Gender gender;
};

struct SeriesGenre {
  int id;
  std::string name;
};

enum SearchType { MOVIE, SERIES };
const char *searchTypeToString(SearchType s);
const SearchType searchTypeToEnum(std::string s);

#endif // !TYPES_H
