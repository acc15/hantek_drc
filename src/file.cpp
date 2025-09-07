#include "file.hpp"

#include "binary.hpp"

namespace hantek_drc {

bool file_signature::is_valid() const {
    return signature == VALID_SIGNATURE;
}

std::istream& operator>>(std::istream& stream, file_signature&& signature) {
    if (stream >> binary_data<file_signature&>(signature) && !signature.is_valid()) {
        stream.setstate(std::ios_base::failbit);
    }
    return stream;
}

}