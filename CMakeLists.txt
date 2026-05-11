cmake_minimum_required(VERSION 3.17)
project(PythonECFSR)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(ECF)
target_include_directories(ECF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ECF)
add_subdirectory(wrapper)
