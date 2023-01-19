// header
#include "log.hpp"

// extern
#include <exception>
#include <fmt/format.h>
#include <fmt/color.h>
#include <sstream>



std::string format_log(std::string const& type, std::string const& message, SourceLocation location, fmt::color color) {

    auto styled = [&](std::string const& text) { return fmt::styled(text, fmt::fg(color)); };
    return fmt::format("[{}]({}:{}): {}", styled(type), location.filename, location.line_number, styled(message));
}

void log(std::string const& message) {

    fmt::print("{}\n", message);
}



void panic(std::string const& message, SourceLocation location) {

    log(format_log("ERROR", message, location, fmt::color::red));
    std::exit(1);
}

void warn(std::string const& message, SourceLocation location) {

    log(format_log("WARN", message, location, fmt::color::yellow));
}

void notice(std::string const& message, SourceLocation location) {

    log(format_log("NOTICE", message, location, fmt::color::white));
}
