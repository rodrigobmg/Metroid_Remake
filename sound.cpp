#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

SDLSound& SDLSound::operator=(const SDLSound& rhs) {
  volume = rhs.volume;
  currentSound = rhs.currentSound;
  music = rhs.music;
  audioRate = rhs.audioRate;
  audioChannels = rhs.audioChannels;
  audioBuffers = rhs.audioBuffers;
  sounds = rhs.sounds;
  channels = rhs.channels;
  return *this;
}

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound() : 
  volume(SDL_MIX_MAXVOLUME), 
  currentSound(-1), 
  music(NULL),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
  channels() 
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  music = Mix_LoadMUS("sound/Norfair.mp3");
  // Need to install midi to play the following:
  // music = Mix_LoadMUS("sound/ballad2.mid");
  if (!music) throw string("Couldn't load 100Years.mp3")+Mix_GetError();

  startMusic();

  sounds.emplace_back( Mix_LoadWAV("sound/Basic Shot.wav") );       //0
  sounds.emplace_back( Mix_LoadWAV("sound/Basic Shot Impact.wav") );//1
  sounds.emplace_back( Mix_LoadWAV("sound/Samus Footstep.wav") );   //2
  sounds.emplace_back( Mix_LoadWAV("sound/Samus Morph.wav") );      //3
  sounds.emplace_back( Mix_LoadWAV("sound/Samus Jump.wav") );       //4
  sounds.emplace_back( Mix_LoadWAV("sound/Samus Ball.wav") );       //5
  sounds.emplace_back( Mix_LoadWAV("sound/Samus Death.wav") );      //6
  sounds.emplace_back( Mix_LoadWAV("sound/explosion.wav") );        //7
  
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
  std::cout << "Music and Sound is loaded" << std::endl;
}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}

void SDLSound::operator[](int index) {
  if (currentSound >= 7) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = 
     Mix_PlayChannel(-1, sounds[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, 1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

