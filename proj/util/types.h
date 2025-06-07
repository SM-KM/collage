#ifndef TYPES_H
#define TYPES_H

#include <string>

enum Gender { FEM, MASC };
enum class SeriesFlags { TOP_RATED, POPULAR, AIRING_TODAY };
enum class MoviesFlags { TOP_RATED, POPULAR };

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

struct MovieGenre {
  int id;
  std::string name;
};

enum SearchType { MOVIE, SERIES };

const char *searchTypeToString(SearchType s);
const SearchType searchTypeToEnum(std::string s);
const SeriesFlags parseSeriesFlag(std::string flag);
const MoviesFlags parseMoviesFlag(std::string flag);

#endif // !TYPES_H
