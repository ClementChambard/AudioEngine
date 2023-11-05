// Copyright 2023 Clément Chambard
#ifndef IAUDIODATA_H_
#define IAUDIODATA_H_

#include "./SampleInfo.hpp"
#include <cstddef>

class IAudioData {
public:
  virtual ~IAudioData() noexcept {}
  virtual size_t generateSamples(float *stream, size_t streamLength, size_t pos,
                                 SampleInfo const &info) noexcept = 0;
  virtual size_t getAudioLength() noexcept = 0;
  virtual double getSampleRate() noexcept = 0;
  virtual size_t getSampleSize() noexcept = 0;
};

#endif // IAUDIODATA_H_
