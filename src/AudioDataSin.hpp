// Copyright 2023 Clément Chambard
#ifndef AUDIODATASIN_H_
#define AUDIODATASIN_H_

#include "AudioEngine/IAudioData.hpp"
#include <memory>

class AudioDataSin : public IAudioData {
public:
  static std::unique_ptr<AudioDataSin> make() {
    return std::make_unique<AudioDataSin>();
  }
  ~AudioDataSin() override {}
  size_t generateSamples(float *stream, size_t streamLength, size_t pos,
                         SampleInfo const &info) noexcept override;
  size_t getAudioLength() noexcept override { return 100000000; }
  double getSampleRate() noexcept override { return 44100.0; }
  size_t getSampleSize() noexcept override { return 1; }

private:
  AudioDataSin() {}
  friend std::unique_ptr<AudioDataSin> std::make_unique<AudioDataSin>();
};

#endif // AUDIODATASIN_H_
