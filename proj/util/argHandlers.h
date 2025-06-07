#ifndef ARGHANDLERS_H
#define ARGHANDLERS_H

#include "types.h"
#include <vector>

void LoadSeriesReq(SeriesFlags flag);
void LoadMoviesReq(MoviesFlags flag);
void LoadMovieById(MoviesFlags flag, int id);
void LoadSerieById(SeriesFlags flag, int id);
void LoadVideoSearch(std::string query);

#endif // !ARGHANDLERS_H
