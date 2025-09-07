#include <doctest/doctest.h>

#include <sstream>

#include "../src/file.hpp"
#include "../src/info.hpp"
#include "../src/binary.hpp"

using namespace hantek_drc;

TEST_CASE("read_signature") {

    info i(caps::hantek_6254bd());

    std::stringstream stream_good(
        std::string(file_signature::VALID_SIGNATURE.begin(), file_signature::VALID_SIGNATURE.end())
            .append({ -1, -1, -1, -1 })
    );
    i.read_signature(stream_good);
    CHECK( stream_good.good() );

    std::stringstream stream_nobytes(std::string({ 0x00, 0x00, 0x02, 0x00, 0x14 }));
    i.read_signature(stream_nobytes);
    CHECK( stream_nobytes.fail() );

    std::stringstream stream_wrong_signature(std::string(file_signature::VALID_SIGNATURE.size() + 4, 0));
    i.read_signature(stream_wrong_signature);
    CHECK( stream_wrong_signature.fail() );
    
}

TEST_CASE("read_channels") {

    info i(caps::hantek_6254bd());

    std::stringstream stream;
    stream << binary_value<std::uint32_t>(0) 
           << binary_value<std::uint32_t>(1)
           << binary_value(file_channel {
                .voltage = 1,
                .coupling = 1,
                .multiplier = 1,
                .unknown_0 = 0,
                .unknown_1 = 0,
                .unknown_2 = 0,
                .lever_1 = 10,
                .lever_2 = 10,
                .unknown_3 = 0,
                .unknown_4 = 0
           })
           << binary_value<std::uint32_t>(0)
           << binary_value<std::uint32_t>(0);

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
