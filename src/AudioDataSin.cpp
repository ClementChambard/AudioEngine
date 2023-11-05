// Copyright 2023 Clément Chambard
#include "./AudioDataSin.hpp"
#include <cmath>

size_t AudioDataSin::generateSamples(float *stream, size_t streamLength,
                                     size_t pos,
                                     SampleInfo const &info) noexcept {
  float freq = 1000.f / 44100.f * 3.141592f * info.pitch;
  for (size_t i = 0; i < streamLength; i++) {
    float t = freq * (i + pos);
    stream[i] += sin(t) * info.volume;
    // float s = 0.f;
    // for (int j = 1; j < 50; j++)
    //{
    //   s += pow(-1, j)/(float)j * sin(j*t);
    // }
    // stream[i] = info.volume * s * -2 / 3.141592f;
    // float s = 0.f;
    // for (int j = 1; j < 20; j++)
    //{
    //     s += pow(-1,j)/((2*j-1)*(2*j-1))*sin((2*j-1)*t);
    // }
    // stream[i] = info.volume * s * -8 / (3.141592*3.141592);
  }
  return pos + streamLength;
}
