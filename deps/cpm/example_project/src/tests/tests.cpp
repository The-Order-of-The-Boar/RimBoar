
// local
#include <sort.hpp>

// builtin
#include <cassert>
#include <iostream>



int main() {

    std::vector<int64_t> vec{4, 3, 2, 1};
    std::vector<int64_t> vec_ref{1, 2, 3, 4};
    
    sort(vec);
    assert(vec == vec_ref);

    std::cout << "all tests passed" << std::endl;
}
