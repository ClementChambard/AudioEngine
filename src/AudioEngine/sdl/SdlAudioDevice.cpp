// Copyright 2023 Clément Chambard
#include "./SdlAudioDevice.hpp"
#include "../../Logger.hpp"
#include "../AudioException.hpp"
#include "./SdlWavAudioData.hpp"

std::unique_ptr<IAudioData>
SdlAudioDevice::createAudioFromFile(char const *filePath) noexcept {
  try {
    return std::make_unique<SdlWavAudioData>(filePath, false);
  } catch (AudioException const &e) {
    logger.error(e.what());
    return nullptr;
  }
}

void SdlAudioDevice::releaseAudio(
    std::unique_ptr<IAudioData> audioData) noexcept {
  delete audioData.release();
}
