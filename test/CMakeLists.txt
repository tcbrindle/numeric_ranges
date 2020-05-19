
add_executable(test_numeric_ranges
    catch_main.cpp
    accumulate.cpp
    adjacent_difference.cpp
    inner_product.cpp
    iota.cpp
    partial_sum.cpp
)
target_link_libraries(test_numeric_ranges PUBLIC numeric_ranges)

