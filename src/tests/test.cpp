
// all project files must be included using the external file include syntax, with angle brackets
// #include <foobar.hpp>

#include <logging/assert.hpp>

static bool __set_assert_exception = []
{
    assert_exception = true;
    return true;
}();

#include <catch2/catch_test_macros.hpp>

TEST_CASE("testing_system")
{
    REQUIRE(false);
}
