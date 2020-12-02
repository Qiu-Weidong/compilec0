# 指定CMake的版本
cmake_minimum_required(VERSION 3.17.0)
# 使用调试
SET(CMAKE_BUILD_TYPE "Debug")  
# 指定项目名称，这条命名会引入两个变量<projectname>_BINARY_DIR和<projectname>_SOURCE_DIR
# 同时，cmake 自动定义了两个等价的变量 PROJECT_BINARY_DIR 和 PROJECT_SOURCE_DIR，用于
# 指向编译后的二进制文件目录和源文件目录
project(C_ZERO)

# add_executable(demo demo.cpp) # 生成可执行文件
# add_library(common STATIC util.cpp) # 生成静态库，这是默认的选项
# add_library(common SHARED util.cpp) # 生成动态库或共享库

# # 将demo.cpp、test.cpp、util.cpp一起编译为名为demo.lib的静态库
# add_library(demo demo.cpp test.cpp util.cpp)

# aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件
# add_library(demo ${SRC_LIST}) # 将变量中的cpp文件全部编译为静态库

aux_source_directory(./source SRC_LIST)
add_executable(C_ZERO ${SRC_LIST})
# set(sfml_include_path C:/Users/56838/SFML-2.5.1/include)
# set(sfml_lib_path C:/Users/56838/SFML-2.5.1/lib)
include_directories(
    # ${CMAKE_CURRENT_SOURCE_DIR} # 这是CMakeLists.txt所在的目录，这里不需要
    # ${CMAKE_CURRENT_BINARY_DIR} # 这是build目录，这里不需要
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    # C:/Users/56838/SFML-2.5.1/include
    # ${sfml_include_path}
)

# link_directories(
#     # ${CMAKE_CURRENT_SOURCE_DIR}/lib
#     # C:/Users/56838/Documents/SFML_workplace/SFML-2.5.1/lib
#     ${sfml_lib_path}
# )

# target_link_libraries(MAGA_CMake 
#     sfml-graphics.lib
#     sfml-window.lib
#     sfml-system.lib
#     sfml-audio.lib
#     sfml-network.lib
# ) 