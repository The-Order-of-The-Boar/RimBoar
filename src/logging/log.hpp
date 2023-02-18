#pragma once

// builtin
#include <string>

// local
#include "./source_location.hpp"

// extern
#include <fmt/format.h>


[[noreturn]] void panic(std::string const& message,
                        SourceLocation location = get_current_source_location());
void warn(std::string const& message, SourceLocation location = get_current_source_location());
void notice(std::string const& message, SourceLocation location = get_current_source_location());
