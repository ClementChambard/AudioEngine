// Copyright 2023 Clément Chambard
#include "./MultiTrackMusic.hpp"
#include <cstring>

MultiTrackMusic::MultiTrackMusic(std::unique_ptr<IAudioData> track_0) {
  tracks.push_back(std::move(track_0));
}

size_t MultiTrackMusic::generateSamples(float *stream, size_t streamLength,
                                        size_t pos,
                                        const SampleInfo &info) noexcept {
  if (in_transition) {
    float *samples_from = new float[streamLength];
    float *samples_to = new float[streamLength];
    memset(samples_from, 0, streamLength * sizeof(float));
    memset(samples_to, 0, streamLength * sizeof(float));
    tracks[active_track]->generateSamples(samples_from, streamLength, pos,
                                          info);
    size_t ret = tracks[transition_to]->generateSamples(
        samples_to, streamLength, pos, info);
    double sample_time = 1.0 / getSampleRate();
    for (size_t i = 0; i < streamLength; i++) {
      float factor = transition_t / transition_duration;
      transition_t += sample_time;
      if (transition_duration <= transition_t) {
        stream[i] += samples_to[i];
        in_transition = false;
        active_track = transition_to;
        continue;
      }
      stream[i] += samples_from[i] * (1 - factor) + samples_to[i] * factor;
    }
    delete[] samples_from;
    delete[] samples_to;
    return ret;
  } else {
    return tracks[active_track]->generateSamples(stream, streamLength, pos,
                                                 info);
  }
}

MusicTrack
MultiTrackMusicHolder::addTrack(std::unique_ptr<IAudioData> new_track) {
  return m_amm->addTrack(std::move(new_track));
}

void MultiTrackMusicHolder::switchTrack(MusicTrack track) {
  m_amm->switchTrack(track);
}

void MultiTrackMusicHolder::transitionTrack(MusicTrack track, double duration) {
  m_amm->transitionTrack(track, duration);
}
