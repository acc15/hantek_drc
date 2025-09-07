#include <doctest/doctest.h>

#include <sstream>

#include "../src/file.hpp"

using namespace hantek_drc;

TEST_CASE("read signature") {

    std::stringstream stream_good(
        std::string(reinterpret_cast<const char*>(
            file_signature::VALID_SIGNATURE.data()), 
            file_signature::VALID_SIGNATURE.size()
        ).append({ -1, -1, -1, -1 })
    );

    stream_good >> file_signature();
    CHECK( stream_good.good() );

    std::stringstream stream_nobytes(std::string({ 0x00, 0x00, 0x02, 0x00, 0x14 }));
    stream_nobytes >> file_signature();
    CHECK( stream_nobytes.fail() );

    std::stringstream stream_wrong_signature(std::string(file_signature::VALID_SIGNATURE.size() + 4, 0));
    stream_wrong_signature >> file_signature();
    CHECK( stream_wrong_signature.fail() );
    
}