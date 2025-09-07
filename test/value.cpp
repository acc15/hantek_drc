#include <doctest/doctest.h>
#include "../src/value.hpp"

using namespace hantek_drc;

TEST_CASE("drc_value") {
    REQUIRE_EQ(drc_value(0), 2);
    REQUIRE_EQ(drc_value(1), 5);
    REQUIRE_EQ(drc_value(2), 10);
    REQUIRE_EQ(drc_value(3), 20);
    REQUIRE_EQ(drc_value(4), 50);
    REQUIRE_EQ(drc_value(5), 100);
    REQUIRE_EQ(drc_value(6), 200);
    REQUIRE_EQ(drc_value(7), 500);
    REQUIRE_EQ(drc_value(8), 1000);
    REQUIRE_EQ(drc_value(23), 100000000);
    REQUIRE_EQ(drc_value(32), 100000000000);
    REQUIRE_EQ(drc_value(32), 100000000000);
}

TEST_CASE("timediv_nanos") {
    REQUIRE_EQ(timediv_nanos(35), 1000000000000UL);
    REQUIRE_EQ(timediv_nanos(32), 100000000000UL);
    REQUIRE_EQ(timediv_nanos(29), 10000000000UL);
    REQUIRE_EQ(timediv_nanos(26), 1000000000UL);
}

TEST_CASE("timediv_seconds") {
    REQUIRE_EQ(timediv_seconds<float>(35), 1000.f);
    REQUIRE_EQ(timediv_seconds<float>(32), 100.f);
    REQUIRE_EQ(timediv_seconds<float>(29), 10.f);
    REQUIRE_EQ(timediv_seconds<float>(26), 1.f);
}

TEST_CASE("sampling_rate_milli") {
    std::size_t x_div = 10;
    std::uint64_t max_rate = 1000000000000ULL;
    REQUIRE_EQ( sampling_rate_milli(x_div, max_rate, timediv_nanos(35)), 250UL );
    REQUIRE_EQ( sampling_rate_milli(x_div, max_rate, timediv_nanos(34)), 500UL );
    REQUIRE_EQ( sampling_rate_milli(x_div, max_rate, timediv_nanos(33)), 1250UL );
    REQUIRE_EQ( sampling_rate_milli(x_div, max_rate, timediv_nanos(32)), 2500UL );
    REQUIRE_EQ( sampling_rate_milli(x_div, max_rate, timediv_nanos(7)), 500000000000UL );
    for (uint16_t i = 0; i < 7; ++i) {
        REQUIRE_EQ( sampling_rate_milli(x_div, max_rate, timediv_nanos(i)), 1000000000000UL );
    }
}
