# 指定CMake的版本
cmake_minimum_required(VERSION 3.17.0)
# 使用调试
SET(CMAKE_BUILD_TYPE "Debug")  
project(C_ZERO)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
)
aux_source_directory(./source SRC_LIST)
add_executable(C_ZERO ${SRC_LIST})