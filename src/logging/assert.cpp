// header
#include "./assert.hpp"

// local
#include "./log.hpp"

// extern
#include <fmt/format.h>



void rb_assert(bool result, std::string const& message, SourceLocation location)
{
#ifndef NDEBUG

    rb_runtime_assert(result, message, location);

#endif
}

void rb_runtime_assert(bool result, std::string const& message, SourceLocation location)
{
    if (result == true)
        return;

    std::string panic_message;

    if (message.empty())
        panic_message = "assertion failed";
    else
        panic_message = fmt::format("assertion failed: {}", message);

    panic(panic_message, location);
}
