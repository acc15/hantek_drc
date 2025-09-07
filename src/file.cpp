#include "file.hpp"

namespace hantek_drc {

bool file_signature::is_valid() const {
    return signature == VALID_SIGNATURE;
}

}