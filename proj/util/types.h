#ifndef TYPES_H
#define TYPES_H

#include <string>

enum SearchType { MOVIE, SERIES };
const char *searchTypeToString(SearchType s);
const SearchType searchTypeToEnum(std::string s);

#endif // !TYPES_H
