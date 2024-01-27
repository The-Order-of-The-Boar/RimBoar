
# target
add_library(core sort.hpp sort.cpp)
target_include_directories(core INTERFACE .)

# libraries
target_link_libraries(core PRIVATE utils)