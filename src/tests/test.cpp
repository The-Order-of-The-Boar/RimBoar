
// all project files must be included using the external file include syntax, with angle brackets
// #include <foobar.hpp>

#include <utils/logging/assert.hpp>

bool const assert_exception = true;

#include <catch2/catch_test_macros.hpp>

TEST_CASE("testing_system")
{
    REQUIRE(true);
}
