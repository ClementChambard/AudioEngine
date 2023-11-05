// Copyright 2023 Clément Chambard
#ifndef SDLAUDIOCONTEXT_H_
#define SDLAUDIOCONTEXT_H_

#include "../IAudioContext.hpp"

#include <vector>

#include <SDL2/SDL.h>

class SdlAudioContext : public IAudioContext {
public:
  SdlAudioContext();
  ~SdlAudioContext() noexcept override;

  void playAudio(AudioObject &ao) noexcept override;
  void pauseAudio(AudioObject &ao) noexcept override;
  void stopAudio(AudioObject &ao) noexcept override;

  void generateSamples(Uint8 *stream, int streamLen) noexcept;

private:
  SDL_AudioDeviceID m_device;
  std::vector<float> m_stream;
  std::vector<AudioObject *> m_playingAudio;

  bool removeAudio(AudioObject &ao) noexcept;

  SdlAudioContext(SdlAudioContext &) {}
  SdlAudioContext(SdlAudioContext &&) {}
  void operator=(SdlAudioContext &) {}
  void operator=(SdlAudioContext &&) {}
};

#endif // SDLAUDIOCONTEXT_H_
