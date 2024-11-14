cmake_minimum_required(VERSION 3.17)
project(CnC-Tools VERSION 0.0.1)

include_directories(Framework)
add_subdirectory(Framework)

add_subdirectory("CPU Tests")
