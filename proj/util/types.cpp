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

const MoviesFlags parseMoviesFlag(std::string flag) {
  if (flag == "--toprated") {
    return MoviesFlags::TOP_RATED;
  } else if (flag == "--popular") {
    return MoviesFlags::POPULAR;
  } else {
    return MoviesFlags::POPULAR;
  }
}

std::string to_string(Flags flag) {
  switch (flag) {
  case Flags::TOP_RATED:
    return "top_rated";
  case Flags::POPULAR:
    return "popular";
  default:
    return "UNKNOWN";
  }
}

Flags to_flag(std::string flagstr) {
  if (flagstr == "--popular") {
    return Flags::POPULAR;
  } else if (flagstr == "--toprated") {
    return Flags::TOP_RATED;
  }

  return Flags::POPULAR;
}
