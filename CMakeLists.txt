cmake_minimum_required(VERSION 3.9)
project(prime_numbers_c C)

set(CMAKE_C_STANDARD 99)

add_executable(prime_numbers_c main.c)
add_executable(bit_set bit_set.c)
target_link_libraries(prime_numbers_c m)
target_link_libraries(bit_set m)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")