cmake_minimum_required (VERSION 3.8)

project(ExperimentalFuture)

add_compile_options(/std:c++17)

include_directories(include)

add_subdirectory(test)
