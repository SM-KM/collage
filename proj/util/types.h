#ifndef TYPES_H
#define TYPES_H

#include <string>

enum Gender { FEM, MASC };
enum class VideoType { MOVIE, SERIE };
enum class SeriesFlags { TOP_RATED, POPULAR, AIRING_TODAY };
enum class MoviesFlags { TOP_RATED, POPULAR };

enum class Flags { TOP_RATED, POPULAR };
std::string to_string(Flags flag);
Flags to_flag(std::string flagstr);

struct CreatedBy {
  int id;
  std::string name;
  std::string profilePath;
  Gender gender;
};

struct Genre {
  int id;
  std::string name;
};

enum SearchType { MOVIE, SERIES };

const char *searchTypeToString(SearchType s);
const SearchType searchTypeToEnum(std::string s);
const SeriesFlags parseSeriesFlag(std::string flag);
const MoviesFlags parseMoviesFlag(std::string flag);

#endif // !TYPES_H
