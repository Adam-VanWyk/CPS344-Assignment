cmake_minimum_required(VERSION 3.5)
project(checksum)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2")

add_executable(checksum src/checksum.c)
