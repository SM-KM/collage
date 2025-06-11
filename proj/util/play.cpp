#include "../requests/requests.h"
#include "play.h"
#include "argHandlers.h"
#include "videos.h"

void PlaySeriesById(int id) {
  Series serie = getSerieById(id);
  LoadVideoSearch(serie.m_name);
}

void PlayMovieById(int id) {
  Movie movie = getMovieById(id);
  LoadVideoSearch(movie.m_name);
}
