
// all project files must be included using the external file include syntax, with angle brackets
// #include <foobar.hpp>


// builtin
#include <iostream>

// local
#include <scenes/game_application.hpp>
#include <slotmap/slotmap.hpp>

// extern
#include <fmt/format.h>



int main()
{
    GameApplication rimboar{};
    rimboar.run();
}
