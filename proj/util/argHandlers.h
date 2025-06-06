#ifndef ARGHANDLERS_H
#define ARGHANDLERS_H

#include "types.h"

void LoadSeriesReq(SeriesFlags flag);
void LoadMoviesReq();
void LoadVideoSearch(std::string searchStr, std::string type);

#endif // !ARGHANDLERS_H
