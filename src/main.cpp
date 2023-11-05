// Copyright 2023 Clément Chambard
#include "./AudioDataSin.hpp"
#include "./AudioEngine.hpp"
#include "AudioEngine/MultiTrackMusic.hpp"
#include <cstddef>
#include <iostream>

#include <SDL2/SDL.h>

#define FILE_PATH "/home/clement/Documents/touhouExtract/sfx/se_nep00.wav"
#define FILE_PATH0 "/home/clement/Documents/touhouExtract/sfx/se_alert.wav"
#define FILE_PATH1 "/home/clement/Documents/touhouExtract/sfx/se_cat00.wav"
#define FILE_PATH_MUSIC_BATTLE "../dw_aqua_battle.wav"
#define FILE_PATH_MUSIC_CALM "../dw_aqua_calm.wav"

int main(int argc [[maybe_unused]], char *argv [[maybe_unused]][]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  {
    AudioEngine engine(AudioEngine::Backend::AUDIO_SDL);

    auto music_handle =
        MultiTrackMusic::from_tracks(engine.soundData(FILE_PATH_MUSIC_CALM),
                                     engine.soundData(FILE_PATH_MUSIC_BATTLE));

    AudioEngine::AudioID music = engine.makeAudio(
        &music_handle,
        {.volume = 0.8, .loop_start = (12 * 44100), .loop_end = 180 * 44100});

    AudioEngine::AudioID sin_wave =
        engine.makeAudio(AudioDataSin::make(), {0.8});

    AudioEngine::AudioID master_spark = engine.loadSFX(FILE_PATH, {0.8});

    SDL_Window *window = SDL_CreateWindow("test audio", 0, 0, 200, 200, 0);
    if (!window) {
      return 1;
    }

    SDL_Event e;
    bool running = true;
    bool track0 = true;
    while (running) {
      while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          switch (e.key.keysym.sym) {
          case SDLK_q:
            running = false;
            break;
          case SDLK_a:
            engine.playSoundLooped(music, -1);
            break;
          case SDLK_s:
            engine.pauseSound(music);
            break;
          case SDLK_d:
            engine.stopSound(music);
            break;
          case SDLK_f:
            if (track0) {
              music_handle.transitionTrack(1, 0.8);
              track0 = false;
            } else {
              music_handle.transitionTrack(0, 0.8);
              track0 = true;
            }
            break;
          case SDLK_t:
            engine.playSound(master_spark);
            break;
          case SDLK_o:
            if (engine.getSFX(master_spark).info().globalVolume - 1)
              engine.getSFX(master_spark).info().globalVolume += 0.05;
            break;
          case SDLK_l:
            if (engine.getSFX(master_spark).info().globalVolume)
              engine.getSFX(master_spark).info().globalVolume -= 0.05;
            break;
          case SDLK_r:
            engine.playSound(sin_wave);
            break;
          }
          break;
        case SDL_KEYUP:
          switch (e.key.keysym.sym) {
          case SDLK_r:
            engine.stopSound(sin_wave);
            break;
          }
          break;
        }
      }
    }
    SDL_DestroyWindow(window);
  }
  SDL_Quit();
  return 0;
}
