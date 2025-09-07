#include "info.hpp"

#include "file.hpp"
#include "binary.hpp"

namespace hantek_drc {

info::info(const hantek_drc::caps& caps): caps(caps) {}

std::istream& operator>>(std::istream& stream, [[maybe_unused]] info& info)  {
    if (!(stream >> file_signature())) {
        return stream;
    }
/*
    if (!file_signature::read(stream)) {
        return stream;
    }*/
    
    return stream;
}

}
