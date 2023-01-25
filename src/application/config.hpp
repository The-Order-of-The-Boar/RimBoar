#pragma once

// builtin
#include <cstdint>
#include <memory>

// extern
#include <nlohmann/json.hpp>



struct Config
{
    float font_pixel_size = 13;
    float default_dpi = 90;
    std::string default_font = "cousine_regular.ttf";

    uint32_t window_width = 1280;
    uint32_t window_height = 720;

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, font_pixel_size, default_dpi, default_font,
                                       window_width, window_height);
};

inline std::unique_ptr<Config> config = nullptr;
