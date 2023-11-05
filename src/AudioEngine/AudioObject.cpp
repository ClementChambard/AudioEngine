// Copyright 2023 Clément Chambard
#include "./AudioObject.hpp"
#include <iostream>

AudioObject::AudioObject(SampleInfo const &info, IAudioData *data) noexcept
    : m_audioPos(0), m_audioLength(data->getAudioLength()), m_sampleInfo(info),
      m_audioData(data) {}

bool AudioObject::generateSamples(float *stream, size_t streamLength) noexcept {
  auto new_audioPos = m_audioData->generateSamples(stream, streamLength,
                                                   m_audioPos, m_sampleInfo);

  size_t sample_n = new_audioPos / m_audioData->getSampleSize();

  if (new_audioPos == static_cast<size_t>(-1)) {
    m_audioPos = 0;
    return false;
  }

  if (sample_n > m_sampleInfo.loop_end) {
    if (m_sampleInfo.loop_count > 0) {
      m_sampleInfo.loop_count--;
      new_audioPos += (m_sampleInfo.loop_start - m_sampleInfo.loop_end) *
                      m_audioData->getSampleSize();
    }
    if (m_sampleInfo.loop_count == -1) {
      new_audioPos += (m_sampleInfo.loop_start - m_sampleInfo.loop_end) *
                      m_audioData->getSampleSize();
    }
  }
  m_audioPos = new_audioPos;

  return true;
}

void AudioObject::setPos(double pos) noexcept {
  if (pos < 0.0)
    pos = 0.0;
  else if (pos > 1.0)
    pos = 1.0;

  m_audioPos = posToAbsolutePos(pos);
}

size_t AudioObject::posToAbsolutePos(double pos) const noexcept {
  return static_cast<size_t>(pos * m_audioLength);
}
