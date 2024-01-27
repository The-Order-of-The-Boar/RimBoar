// header
#include "./assert.hpp"

// local
#include "./log.hpp"

// extern
#include <fmt/format.h>



AssertException::AssertException(std::string _message): message(std::move(_message)) {}
char const* AssertException::what() const noexcept
{
    return this->message.c_str();
}


void rb_assert(bool result, std::string const& message, SourceLocation location)
{
#if !defined NDEBUG || defined TEST_BUILD

    rb_runtime_assert(result, message, location);

#endif
}

void rb_runtime_assert(bool result, std::string const& message, SourceLocation location)
{
    if (result == true) [[unlikely]]
        return;

    std::string panic_message;

    if (message.empty())
        panic_message = "assertion failed";
    else
        panic_message = fmt::format("assertion failed: {}", message);

    if (assert_exception == true)
        throw AssertException{fmt::format("{}: ({}:{})", panic_message, location.filename, location.line_number)};
    else
        panic(panic_message, location);
}
