// Copyright 2023 Clément Chambard
#include "../AudioEngine.hpp"
#include "./AudioException.hpp"
#include "./IAudioDataHolder.hpp"
#include "sdl/SdlBackend.hpp"

AudioEngine::AudioEngine(Backend be) {
  switch (be) {
  case Backend::AUDIO_SDL:
    m_device = std::make_unique<SdlAudioDevice>();
    m_context = std::make_unique<SdlAudioContext>();
    break;
  }
}

AudioEngine::~AudioEngine() {}

std::unique_ptr<IAudioData> AudioEngine::soundData(char const *filename) {
  return m_device->createAudioFromFile(filename);
}

AudioEngine::AudioID AudioEngine::loadSFX(char const *filename,
                                          SampleInfo const &info) {
  std::unique_ptr<IAudioData> data = m_device->createAudioFromFile(filename);
  if (!data)
    return (AudioEngine::AudioID)-1;
  m_createdAudio.push_back(std::move(data));
  m_sfx.push_back({info, m_createdAudio.back().get()});
  return m_sfx.size() - 1;
}

AudioEngine::AudioID AudioEngine::makeAudio(std::unique_ptr<IAudioData> data,
                                            SampleInfo const &info) {
  m_createdAudio.push_back(std::move(data));
  m_sfx.push_back({info, m_createdAudio.back().get()});
  return m_sfx.size() - 1;
}

AudioEngine::AudioID AudioEngine::makeAudio(IAudioDataHolder *data_holder,
                                            SampleInfo const &info) {
  m_createdAudio.push_back(std::unique_ptr<IAudioData>(data_holder->getData()));
  m_sfx.push_back({info, m_createdAudio.back().get()});
  return m_sfx.size() - 1;
}

AudioObject &AudioEngine::getSFX(AudioEngine::AudioID id) {
  if (id >= m_sfx.size()) {
    if (m_sfx.size() == 0)
      throw AudioException(
          "AudioEngine::getSFX : No sfx loaded -> can't get sfx");
    logger.warn("AudioEngine::getSFX : sfx " + std::to_string(id) +
                " doesn't exist -> returning sfx 0");
    return m_sfx[0];
  }
  return m_sfx[id];
}

void AudioEngine::playSound(AudioEngine::AudioID id) {
  if (id < m_sfx.size()) {
    m_sfx[id].info().loop_count = 0;
    m_context->playAudio(m_sfx[id]);
  }
}

void AudioEngine::playSoundLooped(AudioEngine::AudioID id, int loop) {
  if (id < m_sfx.size()) {
    m_sfx[id].info().loop_count = loop;
    m_context->playAudio(m_sfx[id]);
  }
}

void AudioEngine::pauseSound(AudioEngine::AudioID id) {
  if (id < m_sfx.size())
    m_context->pauseAudio(m_sfx[id]);
}

void AudioEngine::stopSound(AudioEngine::AudioID id) {
  if (id < m_sfx.size())
    m_context->stopAudio(m_sfx[id]);
}
