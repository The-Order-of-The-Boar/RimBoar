
// builtin
#include <iostream>
#include <vector>
#include <cstdint>

// local
#include <sort.hpp>
#include <vec_print.hpp>



std::vector<int64_t> read_input() {

    std::cout << "digite 3 números:" << std::endl;
    
    std::vector<int64_t> input;
    int64_t aux;
    
    std::cin >> aux;
    input.push_back(aux);

    std::cin >> aux;
    input.push_back(aux);

    std::cin >> aux;
    input.push_back(aux);

    return input;
}

int main() {

    auto input = read_input();
    std::cout << "before sort: " << input << std::endl;
    sort(input);
    std::cout << "after sort: " << input << std::endl;
}