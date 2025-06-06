#ifndef FORMATTING_H
#define FORMATTING_H

#include "videos.h"
#include <nlohmann/json_fwd.hpp>

std::vector<Movie> formatMovies(nlohmann::json &results);
std::vector<Series> formatSeries(nlohmann::json &results);

#endif // !FORMATTING_H
