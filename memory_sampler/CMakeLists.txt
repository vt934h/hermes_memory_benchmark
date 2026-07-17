cmake_minimum_required(VERSION 3.10)
project(memory_sampler)

enable_language(C CXX)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)
