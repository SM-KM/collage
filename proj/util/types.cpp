#include "types.h"
#include <string>

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

const SeriesFlags parseSeriesFlag(std::string flag) {
  if (flag == "--toprated") {
    return SeriesFlags::TOP_RATED;
  } else if (flag == "--popular") {
    return SeriesFlags::POPULAR;
  } else if (flag == "--airing") {
    return SeriesFlags::AIRING_TODAY;
  }

  return SeriesFlags::POPULAR;
}
