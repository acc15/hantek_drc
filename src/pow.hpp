#pragma once

#include <utility>
#include <array>

namespace hantek_drc {

template <typename T>
constexpr T ipow(T base, unsigned int pow) {
    if (base == 1 || pow == 0) return 1;
    if (base == 0) return 0;
    
    T result = 1;
    while (pow > 0) {
        if (pow % 2 == 0) { 
            pow /= 2;
            base *= base; 
        } else {
            --pow;
            result *= base;
        }
    }
    return result;
}

template <typename T>
constexpr std::pair<unsigned int, T> imaxpow(T base) {
    unsigned int pow = 0;
    T v = 1;
    for (T nv = v*base; nv / base == v; nv *= base) {
        ++pow;
        v = nv;
    }
    return std::make_pair(pow, v);
}

template <unsigned int base, typename T = std::uintmax_t>
using ipow_lookup_array = std::array<T, imaxpow<T>(base).first + 1>;

template <unsigned int base, typename T = std::uintmax_t>
constexpr ipow_lookup_array<base, T> ipow_lookup_init() {
    ipow_lookup_array<base, T> arr;
    T v = 1;
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = v;
        v *= base;
    }
    return arr;
}

template <unsigned int base, typename T = std::uintmax_t>
constexpr T ipow_lookup(unsigned int pow) {
    static constexpr ipow_lookup_array<base, T> table = ipow_lookup_init<base, T>();
    return table[pow];
}

}