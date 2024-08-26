cmake_minimum_required(VERSION 3.22)
project(WebBench)

set(CMAKE_CXX_STANDARD 11)

add_executable(WebBench main.cpp
        webbench.cpp
        webbench.h)
