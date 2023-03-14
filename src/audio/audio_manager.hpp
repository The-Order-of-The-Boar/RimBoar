#pragma once

// builtin
#include <string>
#include <map>
#include <filesystem>

// third party
#include "SDL2/SDL_mixer.h"

const int32_t FREQUENCY = 22050;
const int32_t CHANNELS = 2;
const int32_t CHUNK_SIZE = 2048;


class AudioManager
{
private:
    std::map<std::string, Mix_Chunk*> loaded_sounds;
    std::map<std::string, Mix_Music*> loaded_musics;

    float music_volume = 1.0;
    float sound_volume = 1.0;
    float general_volume = 1.0;

    bool muted = false;

private:
    void load_all_audio_files();

    void load_music(const std::filesystem::path& path);
    void load_sound(const std::filesystem::path& path);

    void update_volumes();

public:
    AudioManager();
    ~AudioManager();

    void play_music(const std::string& music_name);
    void pause_music();
    void resume_music();
    void halt_music();

    void play_sound(const std::string& sound_name);
    
    void set_general_volume(const float volume);
    void set_sound_volume(const float volume);
    void set_music_volume(const float volume);
    
    bool is_muted() const;
    void mute();
    void unmute();

};