// Copyright 2023 Clément Chambard
#include "./SdlWavAudioData.hpp"
#include "../AudioException.hpp"
#include <iostream>

SdlWavAudioData::SdlWavAudioData(char const *fileName,
                                 bool streamFromFile [[maybe_unused]]) {
  // TODO(ClementChambard): handle streamFromFile

  Uint32 wavLength;

  if (SDL_LoadWAV(fileName, &m_spec, &m_start, &wavLength) == NULL) {
    throw AudioException(std::string(fileName) +
                         " could not be loaded as an audio file");
  }

  m_pos = m_start;
  m_end = m_start + wavLength;
}

SdlWavAudioData::~SdlWavAudioData() { SDL_FreeWAV(m_start); }

size_t SdlWavAudioData::generateSamples(float *stream, size_t streamLength,
                                        size_t pos,
                                        const SampleInfo &info) noexcept {
  float pitch = static_cast<float>(info.pitch);
  m_pos = pos + m_start;

  if (m_pos >= m_end || m_pos < m_start)
    return static_cast<size_t>(-1);

  float length = streamLength * pitch;
  float lengthLeft = ((m_end - m_pos) / pitch);
  length = (length > lengthLeft ? lengthLeft : length);

  float sampleIndex = 0.0f;
  int siz = SDL_AUDIO_BITSIZE(m_spec.format);
  bool sign = SDL_AUDIO_ISSIGNED(m_spec.format);
  pitch *= (static_cast<float>(m_spec.freq) / 44100.f);
  float factor = 1.0f / pow(2, siz - 1);
  if (SDL_AUDIO_ISFLOAT(m_spec.format))
    factor = 1.f;
  float add = sign ? 0.0f : -1.0f;
  float volume = info.volume * info.globalVolume;
  // TODO(ClementChambard): big/little endian & float

#define GetAt(i)                                                               \
  (static_cast<float>(samples[static_cast<size_t>(i)] * factor + add))

#define Interp(samples, id)                                                    \
  ((id + 1 > lengthLeft)                                                       \
       ? GetAt(id)                                                             \
       : GetAt(id) + (GetAt(id + 1) - GetAt(id)) *                             \
                         (id - static_cast<float>(static_cast<size_t>(id))))

#define DO                                                                     \
  for (Uint32 i = 0; i < (Uint32)streamLength; i++) {                          \
    stream[i] = stream[i] + volume * Interp(samples, sampleIndex);             \
    sampleIndex += pitch;                                                      \
  }                                                                            \
                                                                               \
  m_pos =                                                                      \
      reinterpret_cast<Uint8 *>(samples + static_cast<size_t>(sampleIndex));   \
  if (length == lengthLeft)                                                    \
    m_pos = m_end;

  if (SDL_AUDIO_ISFLOAT(m_spec.format)) {
    float *samples = reinterpret_cast<float *>(m_pos);
    DO
  } else {
    switch (siz) {
    case 8:
      if (!sign) {
        Uint8 *samples = reinterpret_cast<Uint8 *>(m_pos);
        DO
      } else {
        Sint8 *samples = reinterpret_cast<Sint8 *>(m_pos);
        DO
      }
      break;
    case 32:
      if (!sign) {
        Uint32 *samples = reinterpret_cast<Uint32 *>(m_pos);
        DO
      } else {
        Sint32 *samples = reinterpret_cast<Sint32 *>(m_pos);
        DO
      }
      break;
    default:
    case 16:
      if (!sign) {
        Uint16 *samples = reinterpret_cast<Uint16 *>(m_pos);
        DO
      } else {
        Sint16 *samples = reinterpret_cast<Sint16 *>(m_pos);
        DO
      }
    }
  }

#undef DO
#undef GetAt
#undef Interp

  return static_cast<size_t>(m_pos - m_start);
}

size_t SdlWavAudioData::getAudioLength() noexcept {
  return static_cast<size_t>(m_end - m_start);
}

size_t SdlWavAudioData::getSampleSize() noexcept {
  return SDL_AUDIO_BITSIZE(m_spec.format) / 4;
}
