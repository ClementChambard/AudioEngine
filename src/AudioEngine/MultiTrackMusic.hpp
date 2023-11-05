// Copyright 2023 Clément Chambard
#ifndef AUDIODATAMULTITRACKMUSIC_INCLUDED_H
#define AUDIODATAMULTITRACKMUSIC_INCLUDED_H

#include "./IAudioData.hpp"
#include "./IAudioDataHolder.hpp"
#include <memory>
#include <utility>
#include <vector>

typedef size_t MusicTrack;
#define MusicTrack_Track0 ((MusicTrack)0)

class MultiTrackMusic;
class MultiTrackMusicHolder : public IAudioDataHolder {
public:
  MusicTrack addTrack(std::unique_ptr<IAudioData> new_track);
  void switchTrack(MusicTrack track);
  void transitionTrack(MusicTrack track, double duration);

private:
  explicit MultiTrackMusicHolder(MultiTrackMusic *amm) : m_amm(amm) {}
  IAudioData *getData() override {
    return reinterpret_cast<IAudioData *>(m_amm);
  }
  MultiTrackMusic *m_amm;
  friend class MultiTrackMusic;
};

inline void fill_step(MultiTrackMusic *) {}

template <typename... Args>
inline void fill_step(MultiTrackMusic *amm, std::unique_ptr<IAudioData> iad,
                      Args &&...args);

class MultiTrackMusic : public IAudioData {
public:
  template <typename... Args>
  static MultiTrackMusicHolder from_tracks(std::unique_ptr<IAudioData> track0,
                                           Args... args) {
    auto ret = new MultiTrackMusic(std::move(track0));
    fill_step(ret, std::forward<Args>(args)...);
    return MultiTrackMusicHolder(ret);
  }

  size_t generateSamples(float *stream, size_t streamLength, size_t pos,
                         SampleInfo const &info) noexcept override;
  size_t getAudioLength() noexcept override {
    return tracks[0]->getAudioLength();
  }
  double getSampleRate() noexcept override {
    return tracks[0]->getSampleRate();
  }
  size_t getSampleSize() noexcept override {
    return tracks[0]->getSampleSize();
  }

private:
  explicit MultiTrackMusic(std::unique_ptr<IAudioData> track_0);
  MusicTrack addTrack(std::unique_ptr<IAudioData> new_track) {
    tracks.push_back(std::move(new_track));
    return tracks.size() - 1;
  }
  void switchTrack(MusicTrack track) { active_track = track; }
  void transitionTrack(MusicTrack track, double duration) {
    transition_duration = duration;
    transition_t = 0.0;
    transition_to = track;
    in_transition = true;
  }

  std::vector<std::unique_ptr<IAudioData>> tracks;
  double transition_duration = 0.f;
  double transition_t = 0.f;
  MusicTrack active_track = MusicTrack_Track0;
  MusicTrack transition_to = MusicTrack_Track0;
  bool in_transition = false;
  MultiTrackMusic(MultiTrackMusic const &) = delete;
  void operator=(MultiTrackMusic const &) = delete;
  MultiTrackMusic(MultiTrackMusic &&) = delete;
  void operator=(MultiTrackMusic &&) = delete;
  friend class MultiTrackMusicHolder;
  template <typename... Args>
  friend void fill_step(MultiTrackMusic *amm, std::unique_ptr<IAudioData> iad,
                        Args &&...args);
};

template <typename... Args>
void fill_step(MultiTrackMusic *amm, std::unique_ptr<IAudioData> iad,
               Args &&...args) {
  amm->addTrack(std::move(iad));
  fill_step(amm, std::forward<Args>(args)...);
}

#endif // !AUDIODATAMULTITRACKMUSIC_INCLUDED_H
