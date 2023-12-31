cmake_minimum_required(VERSION 3.8)

project(thread)

set(CXX_STANDARD 17)

include_directories(include)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)