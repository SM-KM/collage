#ifndef DOTENV_H
#define DOTENV_H

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <string>

void loadDotEnv(const std::string &filename = ".env");

#endif // !DOTENV_H
