#pragma once

#include <istream>

namespace hantek_drc {

template <typename T>
struct binary_data { 
    T data; 
};

template <typename T>
std::istream& operator>>(std::istream& stream, binary_data<T> value) {
    stream.read(reinterpret_cast<char*>(&value.data), sizeof(value.data));
    return stream;
}


}