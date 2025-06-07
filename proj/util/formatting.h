#ifndef FORMATTING_H
#define FORMATTING_H

#include "videos.h"
#include <nlohmann/json_fwd.hpp>

std::vector<Movie> formatMovies(nlohmann::json &results);
std::vector<Series> formatSeries(nlohmann::json &results);
Movie formatMovie(nlohmann::json &m);
Series formatSerie(nlohmann::json &m);

#endif // !FORMATTING_H
