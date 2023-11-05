// Copyright 2023 Clément Chambard
#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

class Logger {
public:
  Logger();
  ~Logger();

  void info(std::string);
  void warn(std::string);
  void error(std::string);
};

extern Logger logger;

#endif // LOGGER_H_
