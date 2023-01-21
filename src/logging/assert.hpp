#pragma once

// builtin
#include <exception>
#include <string>

// local
#include "./source_location.hpp"



inline bool assert_exception = false;

class AssertException final: std::exception
{
    private:

        std::string message;

    public:

        AssertException(std::string _message);
        const char* what() const noexcept override;
};


void rb_assert(bool result, std::string const& message = "",
               SourceLocation location = get_current_source_location());
void rb_runtime_assert(bool result, std::string const& message = "",
                       SourceLocation location = get_current_source_location());
