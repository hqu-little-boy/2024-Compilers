cmake_minimum_required(VERSION 3.16)
project(lexer-handwritten)
set(CMAKE_CXX_STANDARD 23)
# 递归搜索源文件
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
# 添加可执行文件
add_executable(lexer-handwritten main.cpp ${HEADER_FILES} ${SOURCE_FILES})
