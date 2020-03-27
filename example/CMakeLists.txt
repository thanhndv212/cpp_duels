cmake_minimum_required(VERSION 2.8.3)
project(example CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

add_executable(example main.cpp)
