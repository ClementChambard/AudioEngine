// Copyright 2023 Clément Chambard
#ifndef SAMPLEINFO_H_
#define SAMPLEINFO_H_

#include <cstddef>
struct SampleInfo {
  double volume = 1.0f;
  double globalVolume = 1.0f;
  double pitch = 1.0f;
  size_t loop_start = 0;
  size_t loop_end = 0;
  int loop_count = 0;
};

#endif // SAMPLEINFO_H_
