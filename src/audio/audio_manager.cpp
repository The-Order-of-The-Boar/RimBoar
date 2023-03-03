// header
#include "./audio_manager.hpp"
#include <SDL_mixer.h>
#include <iostream>
#include <SDL.h>

// built in
#include <filesystem>

// local


AudioManager::AudioManager()
{
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);
    //this->load_sound();
    std::cout << "a\n";
}

AudioManager::~AudioManager()
{
    Mix_CloseAudio();
    
    for(auto& sound : this->loaded_sounds)
        Mix_FreeChunk(sound.second);

    for(auto& music : this->loaded_musics)
        Mix_FreeMusic(music.second);
}

void AudioManager::load_music()
{

}

void AudioManager::load_sound(const std::filesystem::path& path)
{
    bool exists = std::filesystem::exists(path);
    rb_runtime_assert(exists);
    Mix_Chunk* new_sound = Mix_LoadWAV(path.c_str());
    std::cout << Mix_GetError() <<std::endl;
    this->loaded_sounds["funny"] = new_sound;
    std::cout << "war\n";
    int channel = Mix_PlayChannel(-1, new_sound, 1);
    std::cout << "bar\n";
    std::cout << Mix_GetError() <<std::endl;
}

void AudioManager::load_all_sounds()
{

}