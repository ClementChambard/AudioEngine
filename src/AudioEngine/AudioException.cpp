// Copyright 2023 Clément Chambard
#include "./AudioException.hpp"

const char *AudioException::what() const noexcept { return m_what.c_str(); }
