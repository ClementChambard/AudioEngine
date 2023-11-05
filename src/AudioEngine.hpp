// Copyright 2023 Clément Chambard
#ifndef AUDIOENGINE_H_
#define AUDIOENGINE_H_

#include "./Logger.hpp"
#include "AudioEngine/AudioObject.hpp"
#include "AudioEngine/IAudioDataHolder.hpp"
#include "AudioEngine/SampleInfo.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class IAudioDevice;
class IAudioContext;
class IAudioData;
/**
 * Audio engine
 */
class AudioEngine {
public:
  /** Backend type for audio engine */
  enum class Backend {
    /** Use SDL2 audio */ AUDIO_SDL,
  };

  /** Sound index */
  typedef size_t AudioID;

  explicit AudioEngine(Backend be);
  ~AudioEngine();

  std::unique_ptr<IAudioData> soundData(char const *filename);

  AudioID loadSFX(char const *filename, SampleInfo const &info);
  AudioID makeAudio(std::unique_ptr<IAudioData> data, SampleInfo const &info);
  AudioID makeAudio(IAudioDataHolder *data_holder, SampleInfo const &info);
  AudioObject &getSFX(AudioID id);

  void playSound(AudioID id);
  void playSoundLooped(AudioID id, int loop);
  void pauseSound(AudioID id);
  void stopSound(AudioID id);

private:
  std::unique_ptr<IAudioContext> m_context;
  std::unique_ptr<IAudioDevice> m_device;

  std::vector<AudioObject> m_sfx;
  std::vector<std::unique_ptr<IAudioData>> m_createdAudio;
};

#endif // AUDIOENGINE_H_
