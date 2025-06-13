#ifndef ARGHANDLERS_H
#define ARGHANDLERS_H

#include "types.h"
#include <optional>
#include <vector>

void LoadReq(VideoType type, Flags flag, bool byId = false,
             std::optional<int> id = std::nullopt,
             std::optional<int> genreId = std::nullopt);

void LoadVideoSearch(std::string query);
void LoadGenresReq();

void RateContent(int id, int rating, VideoType type);

#endif // !ARGHANDLERS_H
