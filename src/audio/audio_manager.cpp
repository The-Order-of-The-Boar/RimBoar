// header
#include "./audio_manager.hpp"

// built in
#include <filesystem>
#include <iostream>

// third party
#include <SDL_mixer.h>
#include <SDL.h>

// local
#include "../utils/logging/assert.hpp"
#include "../utils/time_utils.hpp"


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
    TimeMeasurer load_file_time{"Audio files loaded"};
    const std::filesystem::path sound_folder{"assets/audio/sound_effects"};
    const std::filesystem::path music_folder{"assets/audio/music"};

    for(const auto& sound_file : std::filesystem::directory_iterator{sound_folder})
    {
        this->load_sound(sound_file.path());
    }
    
    for(const auto& music_file : std::filesystem::directory_iterator{music_folder})
    {
        this->load_music(music_file.path());
    }
    load_file_time.print_time();

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
    if(this->muted) return;
    int32_t channel = Mix_PlayMusic(this->loaded_musics.at(music_name), -1);
}
void AudioManager::pause_music()
{
    Mix_PauseMusic();
}

void AudioManager::resume_music()
{
    if(this->muted) return;
    Mix_ResumeMusic();
}

void AudioManager::halt_music()
{
    Mix_HaltMusic();
}

void AudioManager::play_sound(const std::string &sound_name)
{
    if(this->muted) return;
    int32_t channel = Mix_PlayChannel(-1, this->loaded_sounds.at(sound_name), 0);
}

void AudioManager::update_volumes()
{
    Mix_VolumeMusic(MIX_MAX_VOLUME * this->music_volume * this->general_volume);
    Mix_Volume(-1,MIX_MAX_VOLUME * this->sound_volume * this->general_volume);
}

float AudioManager::get_general_volume() const
{
    return this->general_volume;
}

float AudioManager::get_sound_volume() const
{
    return this->sound_volume;
}

float AudioManager::get_music_volume() const
{
    return this->music_volume;
}

void AudioManager::set_general_volume(const float volume)
{
    this->general_volume = volume;
    this->update_volumes();
}

void AudioManager::set_sound_volume(const float volume)
{
    this->sound_volume = volume;
    this->update_volumes();
}

void AudioManager::set_music_volume(const float volume)
{
    this->music_volume = volume;
    this->update_volumes();
}

bool AudioManager::is_muted() const
{
    return this->muted;
}

void AudioManager::mute()
{
    Mix_VolumeMusic(0);
    Mix_Volume(-1,0);
    this->muted = true;
    this->pause_music();
}

void AudioManager::unmute()
{
    this->muted = false;
    this->update_volumes();
    this->resume_music();
}
