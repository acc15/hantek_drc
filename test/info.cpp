#include <doctest/doctest.h>

#include <sstream>

#include "../src/file.hpp"
#include "../src/info.hpp"
#include "../src/binary.hpp"

using namespace hantek_drc;

std::stringstream init_test_stream() {
    return std::stringstream(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
}

std::ostream& write_test_signature(std::ostream& out) {
    return out 
        << std::string(file_signature::VALID_SIGNATURE.begin(), file_signature::VALID_SIGNATURE.end()) 
        << std::string({ -1, -1 });
}

std::ostream& write_test_channels(std::ostream& out) {
    return out << binary_value<std::uint32_t>(0) 
        << binary_value<std::uint32_t>(1)
        << binary_value(file_channel {
            .voltage = 1,
            .coupling = 1,
            .multiplier = 1,
            .lever_1 = 10,
            .lever_2 = 10
        })
        << binary_value<std::uint32_t>(0)
        << binary_value<std::uint32_t>(0);
}

std::ostream& write_test_general(std::ostream& out) {
    return out << binary_value(file_general {
        .timediv = 10
    });
}


TEST_CASE("read_signature") {

    info i(caps::hantek_6254bd());

    std::stringstream stream_good = init_test_stream();
    write_test_signature(stream_good);

    i.read_signature(stream_good);
    CHECK( stream_good.good() );

    std::stringstream stream_nobytes = init_test_stream();
    stream_nobytes << std::string({ 0x00, 0x00, 0x02, 0x00, 0x14 });
    i.read_signature(stream_nobytes);
    CHECK( stream_nobytes.fail() );

    std::stringstream stream_wrong_signature = init_test_stream();
    stream_wrong_signature << std::string(file_signature::VALID_SIGNATURE.size() + 4, 0);
    i.read_signature(stream_wrong_signature);
    CHECK( stream_wrong_signature.fail() );
    
}

TEST_CASE("read_channels") {

    info i(caps::hantek_6254bd());

    std::stringstream stream = init_test_stream();
    write_test_channels(stream);

    i.read_channels(stream);
    CHECK( stream.good() );

    REQUIRE_EQ( i.channels.size(), 1 );
    REQUIRE_EQ( i.channels[0].index, 0 );
    REQUIRE_EQ( i.channels[0].number, 1 );
    REQUIRE_EQ( i.channels[0].voltage, 1 );
    REQUIRE_EQ( i.channels[0].coupling, coupling::AC );
    REQUIRE_EQ( i.channels[0].multiplier, 1 );
    REQUIRE_EQ( i.channels[0].lever, 10 );
    REQUIRE_EQ( &i.channels[0].info, &i );

}

TEST_CASE("read_general") {

    info i(caps::hantek_6254bd());

    std::stringstream stream = init_test_stream();
    write_test_general(stream);

    i.read_general(stream);
    REQUIRE( stream.good() );
    REQUIRE_EQ( i.timediv, 10 );

}

TEST_CASE("read_header") {
    std::stringstream stream = init_test_stream();
    write_test_signature(stream);
    write_test_channels(stream);
    write_test_general(stream);
    
    info i(caps::hantek_6254bd());
    stream >> i;
    REQUIRE( stream.good() );
    REQUIRE_EQ( i.channels.size(), 1 );
    REQUIRE_EQ( i.timediv, 10 );

}
