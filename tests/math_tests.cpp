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
