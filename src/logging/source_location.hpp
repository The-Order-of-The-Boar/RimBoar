#pragma once

// builtin
#include <cstdint>
#include <string_view>



struct SourceLocation {

    uint64_t line_number;
    std::string_view filename;
};

SourceLocation get_current_source_location(uint64_t _line_number = __builtin_LINE(), std::string_view _filename = __builtin_FILE());
