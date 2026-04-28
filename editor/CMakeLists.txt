cmake_minimum_required(VERSION 3.16)
project(rendr LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB editor_sources src/*.cpp src/imgui/*.cpp)

add_executable(editor main.cpp ${editor_sources})
target_include_directories(editor PUBLIC include include/imgui)
target_link_libraries(editor PUBLIC rendr)
