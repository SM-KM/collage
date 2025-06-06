#ifndef TYPES_H
#define TYPES_H

#include <string>

enum Gender { FEM, MASC };
struct CreatedBy;
struct SeriesGenre;

enum SearchType { MOVIE, SERIES };
const char *searchTypeToString(SearchType s);
const SearchType searchTypeToEnum(std::string s);

#endif // !TYPES_H
