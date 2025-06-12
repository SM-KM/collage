#ifndef ARGHANDLERS_H
#define ARGHANDLERS_H

#include "types.h"
#include <optional>
#include <vector>

void LoadReq(VideoType type, Flags flag, bool byId = false,
             std::optional<int> id = std::nullopt);
void LoadSeriesReq(SeriesFlags flag);
void LoadMovieById(MoviesFlags flag, int id);
void LoadSerieById(SeriesFlags flag, int id);
void LoadVideoSearch(std::string query);

void RateMovieById(int id, int rating);
void RateSeriesById(int id, int rating);

#endif // !ARGHANDLERS_H
