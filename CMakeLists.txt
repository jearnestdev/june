cmake_minimum_required(VERSION 3.20)

project(June VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(june 
    src/Main.cpp
    src/core/June.cpp)

target_include_directories(june 
    PRIVATE 
    $(PROJECT_SOURCE_DIR)/include)