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

private:
public:
    void load_music();
    void load_sound(const std::filesystem::path& path);

    void load_all_sounds();

public:
    AudioManager();
    ~AudioManager();

    void play_music(std::string music_name);
    void pause_music();

    void play_sound(std::string sound_name);

};