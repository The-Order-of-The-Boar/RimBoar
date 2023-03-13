// header
#include "./audio_manager.hpp"
#include <SDL_mixer.h>
#include <iostream>
#include <SDL.h>

// built in
#include <filesystem>

// local
#include "../logging/assert.hpp"


AudioManager::AudioManager()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    this->load_all_audio_files();
}

AudioManager::~AudioManager()
{
    Mix_CloseAudio();
    
    for(auto& sound : this->loaded_sounds)
        Mix_FreeChunk(sound.second);

    for(auto& music : this->loaded_musics)
        Mix_FreeMusic(music.second);
}

void AudioManager::load_all_audio_files()
{
    const std::filesystem::path sound_folder{"assets/audio/sound_effects"};
    const std::filesystem::path music_folder{"assets/audio/music"};

    for(const auto& sound_file : std::filesystem::directory_iterator{sound_folder})
    {
        std::cout << sound_file.path().filename() << std::endl;
        this->load_sound(sound_file.path());
    }
    
    for(const auto& music_file : std::filesystem::directory_iterator{music_folder})
    {
        std::cout << music_file.path().filename() << std::endl;
        this->load_music(music_file.path());
    }

}

void AudioManager::load_music(const std::filesystem::path& path)
{
    bool exists = std::filesystem::exists(path);
    rb_runtime_assert(exists);

    Mix_Music* new_music = Mix_LoadMUS(path.c_str());
    this->loaded_musics[path.filename()] = new_music;
}

void AudioManager::load_sound(const std::filesystem::path& path)
{
    bool exists = std::filesystem::exists(path);
    rb_runtime_assert(exists);

    Mix_Chunk* new_sound = Mix_LoadWAV(path.c_str());
    this->loaded_sounds[path.filename()] = new_sound;
}


void AudioManager::play_music(const std::string& music_name)
{
    int32_t channel = Mix_PlayMusic(this->loaded_musics.at(music_name), -1);
}
void AudioManager::pause_music()
{
    Mix_PauseMusic();
}

void AudioManager::resume_music()
{
    Mix_ResumeMusic();
}

void AudioManager::halt_music()
{
    Mix_HaltMusic();
}

void AudioManager::play_sound(const std::string &sound_name)
{
    int32_t channel = Mix_PlayChannel(-1, this->loaded_sounds.at(sound_name), 0);
}