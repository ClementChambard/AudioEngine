// Copyright 2023 Clément Chambard
#ifndef IAUDIODEVICE_H_
#define IAUDIODEVICE_H_

#include "./IAudioData.hpp"
#include <memory>
#include <string>

class IAudioDevice {
public:
  virtual ~IAudioDevice() noexcept {}
  virtual std::unique_ptr<IAudioData>
  createAudioFromFile(char const *filePath) noexcept = 0;
  virtual void releaseAudio(std::unique_ptr<IAudioData> audioData) noexcept = 0;
};

#endif // IAUDIODEVICE_H_
