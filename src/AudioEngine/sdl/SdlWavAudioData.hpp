// Copyright 2023 Clément Chambard
#ifndef SDLWAVAUDIODATA_H_
#define SDLWAVAUDIODATA_H_

#include "../IAudioData.hpp"
#include <SDL2/SDL.h>
#include <string>

class SdlWavAudioData : public IAudioData {
public:
  SdlWavAudioData(char const *fileName, bool streamFromFile);
  ~SdlWavAudioData() override;

  size_t generateSamples(float *stream, size_t streamLength, size_t pos,
                         SampleInfo const &info) noexcept override;
  size_t getAudioLength() noexcept override;
  double getSampleRate() noexcept override { return m_spec.freq; }
  size_t getSampleSize() noexcept override;

private:
  Uint8 *m_pos;
  Uint8 *m_start;
  Uint8 *m_end;
  SDL_AudioSpec m_spec;

  SdlWavAudioData(SdlWavAudioData &) {}
  SdlWavAudioData(SdlWavAudioData &&) {}
  void operator=(SdlWavAudioData &) {}
  void operator=(SdlWavAudioData &&) {}
};

#endif // SDLWAVAUDIODATA_H_
