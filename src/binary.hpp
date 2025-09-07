#pragma once

#include <istream>
#include <ostream>

namespace hantek_drc {

template <typename T>
struct binary_data {
    T data; 
};
template <typename T>
binary_data<T&> binary_ref(T& ref) { 
    return binary_data<T&>(ref);
}
template <typename T>
binary_data<T> binary_value(T&& value) { 
    return binary_data<T>(std::move(value));
}
template <typename T>
binary_data<T> binary_value(const T& value) { 
    return binary_data<T>(value);
}

template <typename T>
std::istream& operator>>(std::istream& stream, binary_data<T> value) {
    stream.read(reinterpret_cast<char*>(&value.data), sizeof(T));
    return stream;
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const binary_data<T>& value) {
    stream.write(reinterpret_cast<const char*>(&value.data), sizeof(T));
    return stream;
}

}