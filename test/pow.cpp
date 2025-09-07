#include <doctest/doctest.h>
#include "../src/pow.hpp"

using namespace hantek_drc;

TEST_CASE("imaxpow") {
    REQUIRE_EQ( imaxpow<std::uint8_t>(10), std::make_pair(2, 100) );
    REQUIRE_EQ( imaxpow<std::uint16_t>(10), std::make_pair(4, 10000) );
    REQUIRE_EQ( imaxpow<std::uint32_t>(10), std::make_pair(9, 1000000000) );
}

TEST_CASE("ipow_table") {
    auto table = ipow_lookup_init<10, uint16_t>();
    REQUIRE_EQ( table.size(), 5 );
    REQUIRE_EQ( table[0], 1 );
    REQUIRE_EQ( table[1], 10 );
    REQUIRE_EQ( table[2], 100 );
    REQUIRE_EQ( table[3], 1000 );
    REQUIRE_EQ( table[4], 10000 );
}
