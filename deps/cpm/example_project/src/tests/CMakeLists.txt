
# target
add_executable(tests tests.cpp)

# libraries
target_link_libraries(tests PRIVATE utils core)
