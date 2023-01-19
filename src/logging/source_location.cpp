
// header
#include "source_location.hpp"

#include <string_view>

SourceLocation get_current_source_location(uint64_t _line_number, std::string_view _filename) {

    return SourceLocation{.line_number = _line_number, .filename = _filename};
}
