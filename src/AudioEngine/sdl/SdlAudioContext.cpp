// Copyright 2023 Clément Chambard
#include "./SdlAudioContext.hpp"
#include "../AudioException.hpp"

static void SdlAudioContext_AudioCallback(void *userData, Uint8 *streamIn,
                                          int length) {
  SdlAudioContext *context = reinterpret_cast<SdlAudioContext *>(userData);
  context->generateSamples(streamIn, length);
}

SdlAudioContext::SdlAudioContext() {
  SDL_AudioSpec spec;

  // TODO(ClementChambard): don't hardcode
  SDL_zero(spec);
  spec.freq = 44100;
  spec.format = AUDIO_S16SYS;
  spec.channels = 2;
  spec.samples = 2048;
  spec.callback = SdlAudioContext_AudioCallback;
  spec.userdata = reinterpret_cast<void *>(this);

  // TODO(ClementChambard): handle differing specs
  m_device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);

  if (m_device == 0) {
    throw AudioException(SDL_GetError());
  }

  SDL_PauseAudioDevice(m_device, 0);
}

SdlAudioContext::~SdlAudioContext() noexcept {
  SDL_PauseAudioDevice(m_device, 1);
  SDL_CloseAudioDevice(m_device);
}

void SdlAudioContext::playAudio(AudioObject &ao) noexcept {
  SDL_LockAudioDevice(m_device);

  // prevent duplicate
  removeAudio(ao);
  m_playingAudio.push_back(&ao);

  SDL_UnlockAudioDevice(m_device);
}

void SdlAudioContext::pauseAudio(AudioObject &ao) noexcept {
  SDL_LockAudioDevice(m_device);

  removeAudio(ao);

  SDL_UnlockAudioDevice(m_device);
}

void SdlAudioContext::stopAudio(AudioObject &ao) noexcept {
  SDL_LockAudioDevice(m_device);

  if (removeAudio(ao))
    ao.setPos(0.0);

  SDL_UnlockAudioDevice(m_device);
}

void SdlAudioContext::generateSamples(Uint8 *streamIn,
                                      int streamInLen) noexcept {
  size_t streamLen = static_cast<size_t>(streamInLen / 2);

  m_stream.reserve(streamLen);
  float *floatStream = m_stream.data();

  for (size_t i = 0; i < streamLen; i++)
    floatStream[i] = 0.0f;

  auto it = m_playingAudio.begin();
  auto end = m_playingAudio.end();

  for (; it != end; ++it) {
    if (!(*it)->generateSamples(floatStream, streamLen)) {
      removeAudio(*(*it));
    }
  }

  Sint16 *stream = reinterpret_cast<Sint16 *>(streamIn);
  for (size_t i = 0; i < streamLen; i++) {
    float val = floatStream[i];

    if (val > 1.0f)
      val = 1.0f;
    else if (val < -1.0f)
      val = -1.0f;

    stream[i] = (Sint16)(val * 32767);
  }
}

bool SdlAudioContext::removeAudio(AudioObject &ao) noexcept {
  auto it = m_playingAudio.begin();
  auto end = m_playingAudio.end();

  for (; it != end; ++it) {
    if (*it == &ao) {
      m_playingAudio.erase(it);
      return true;
    }
  }

  return false;
}
