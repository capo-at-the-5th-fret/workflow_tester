#include <doctest/doctest.h>
#include "math/math.h"

TEST_CASE("add")
{
    using math::add;
    
    REQUIRE(add(-1,-2) == -3);
    REQUIRE(add(-1,2) == 1);
    REQUIRE(add(1,-2) == -1);
    REQUIRE(add(1,2) == 3);
}

TEST_CASE("subtract")
{
    using math::subtract;

    REQUIRE(subtract(-1,-2) == 0);
    REQUIRE(subtract(-1,2) == -3);
    REQUIRE(subtract(1,-2) == 3);
    REQUIRE(subtract(1,2) == -1);
}

TEST_CASE("multiply")
{
    using math::multiply;

    REQUIRE(multiply(-1,-2) == 2);
    REQUIRE(multiply(-1,2) == -2);
    REQUIRE(multiply(1,-2) == -2);
    REQUIRE(multiply(1,2) == 2);
}

TEST_CASE("divide")
{
    using math::divide;

    REQUIRE(divide(-2,-1) == 2);
    REQUIRE(divide(2,-1) == -2);
    REQUIRE(divide(-2,1) == -2);
    REQUIRE(divide(2,1) == 2);
}