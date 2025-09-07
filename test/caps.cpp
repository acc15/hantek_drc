#include <doctest/doctest.h>
#include "../src/caps.hpp"

using namespace hantek_drc;

TEST_CASE("hantek_6254bd") {
    caps c = caps::hantek_6254bd();
    REQUIRE_EQ(c.max_channels, 4);
    REQUIRE_EQ(c.x_div, 10);
    REQUIRE_EQ(c.y_div, 8);
}
