// Copyright 2023 Clément Chambard
#ifndef IAUDIODATAHOLDER_INCLUDED_H
#define IAUDIODATAHOLDER_INCLUDED_H

#include "./IAudioData.hpp"
#include <memory>
class IAudioDataHolder {
  virtual IAudioData *getData() = 0;
  friend class AudioEngine;
};

#endif // !IAUDIODATAHOLDER_INCLUDED_H
