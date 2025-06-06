#include "types.h"
#include <string>

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

const char *searchTypeToString(SearchType s) {
  switch (s) {
  case MOVIE:
    return "MOVIE";
  case SERIES:
    return "SERIES";
  }

  return "MOVIE";
}

const SearchType searchTypeToEnum(std::string s) {
  if (s == "MOVIE") {
    return MOVIE;
  } else if (s == "SERIES") {
    return SERIES;
  }

  return MOVIE;
}
