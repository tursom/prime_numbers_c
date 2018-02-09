cmake_minimum_required(VERSION 3.9)
project(prime_numbers_c C)

set(CMAKE_C_STANDARD 99)

add_executable(prime_numbers_c main.c)
target_link_libraries(prime_numbers_c m)