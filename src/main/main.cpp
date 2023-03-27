
// all project files must be included using the external file include syntax, with angle brackets
// #include <foobar.hpp>


// builtin
#include <iostream>

// local
#include <application/game_application.hpp>
#include <slotmap/slotmap.hpp>
#include <utils/logging/log.hpp>
#include <utils/system.hpp>

// extern
#include <fmt/format.h>



bool const assert_exception = false;



int main()
{
    notice(fmt::format("{}", get_binary_path().c_str()));
    GameApplication rimboar{};
    rimboar.setup();
    rimboar.run();
}
