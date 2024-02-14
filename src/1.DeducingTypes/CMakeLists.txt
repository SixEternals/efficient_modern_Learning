cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD_REQUIRED 14)
project(ch1)

# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)

add_executable(app i3_decltype.cpp)
