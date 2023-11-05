// Copyright 2023 Clément Chambard
#ifndef SDLAUDIODEVICE_H_
#define SDLAUDIODEVICE_H_

#include "../IAudioDevice.hpp"
#include <memory>

class SdlAudioDevice : public IAudioDevice {
public:
  void releaseAudio(std::unique_ptr<IAudioData> audioData) noexcept override;
  std::unique_ptr<IAudioData>
  createAudioFromFile(char const *filePath) noexcept override;
};

#endif // SDLAUDIODEVICE_H_
