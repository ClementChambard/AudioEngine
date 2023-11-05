// Copyright 2023 Clément Chambard
#ifndef AUDIOOBJECT_H_
#define AUDIOOBJECT_H_

#include "./IAudioData.hpp"
#include "./SampleInfo.hpp"

class AudioObject {
public:
  AudioObject(SampleInfo const &info, IAudioData *data) noexcept;

  bool generateSamples(float *stream, size_t streamLength) noexcept;
  void setPos(double pos) noexcept;

  SampleInfo &info() noexcept { return m_sampleInfo; }

private:
  size_t m_audioPos;
  size_t m_audioLength;
  SampleInfo m_sampleInfo;
  IAudioData *m_audioData;

  size_t posToAbsolutePos(double pos) const noexcept;
};

#endif // AUDIOOBJECT_H_
