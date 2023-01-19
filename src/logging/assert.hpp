#pragma once

// builtin
#include <string>

// local
#include "./source_location.hpp"



void rb_assert(bool result, std::string const& message = "", SourceLocation location = get_current_source_location());
void rb_runtime_assert(bool result, std::string const& message = "", SourceLocation location = get_current_source_location());
