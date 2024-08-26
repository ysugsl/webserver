cmake_minimum_required(VERSION 3.10)

project(Webserver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找源文件和头文件
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 创建可执行文件
add_executable(Webserver ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(Webserver pthread)
