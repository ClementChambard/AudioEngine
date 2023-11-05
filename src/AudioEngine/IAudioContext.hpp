// Copyright 2023 Clément Chambard
#ifndef IAUDIOCONTEXT_H_
#define IAUDIOCONTEXT_H_

#include "./AudioObject.hpp"

class IAudioContext {
public:
  virtual ~IAudioContext() noexcept {}
  virtual void playAudio(AudioObject &ao) noexcept = 0;
  virtual void pauseAudio(AudioObject &ao) noexcept = 0;
  virtual void stopAudio(AudioObject &ao) noexcept = 0;
};

#endif // IAUDIOCONTEXT_H_
