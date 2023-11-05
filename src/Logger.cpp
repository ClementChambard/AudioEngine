// Copyright 2023 Clément Chambard
#include "./Logger.hpp"
#include <iostream>

Logger logger;

Logger::Logger() {}
Logger::~Logger() {}

void Logger::info(std::string s) { std::cout << "INFO: " << s << "\n"; }

void Logger::warn(std::string s) { std::cout << "WARNING" << s << "\n"; }

void Logger::error(std::string s) { std::cout << "ERROR" << s << "\n"; }
