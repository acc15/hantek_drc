#include <doctest/doctest.h>
#include "../src/binary.hpp"
#include "../src/info.hpp"
#include "../src/file.hpp"

using namespace hantek_drc;

template<typename Format>
class csv_handler {

};

template <typename Handler>
class reader {
    struct info info;
    Handler handler;
    reader(Handler&& handler): handler(handler) {}

    std::istream& read_frame(std::istream& stream, const channel& channel) {
        file_data_header data_header;
        if (!(stream >> binary_ref(data_header))) {
            return stream;
        }
        std::array<std::int16_t, BUFFER_LENGTH> frame_data;
        if (!(stream >> binary_ref(frame_data))) {
            return stream;
        }
        if (!handler.on_frame(channel, frame_data)) {
            stream.setstate(std::ios_base::failbit);
        }
        return stream;
    }
};

TEST_CASE("read_data") {



}