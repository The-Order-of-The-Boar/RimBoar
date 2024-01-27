// header
#include "vec_print.hpp"



std::ostream& operator<<(std::ostream& out, std::vector<int64_t> const& vec) {

    out << '[';

    for (size_t idx = 0; idx < vec.size() - 1; ++idx)
        out << vec[idx] << ", ";
    out << vec.back();

    out << ']';

    return out;
}
