cmake_minimum_required(VERSION 3.16)
project(rendr LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB rendr_sources src/*.cpp src/gl/*.cpp)

include(cmake/dependencies.cmake)
include(cmake/platform.cmake)
include(cmake/options.cmake)

add_library(rendr STATIC ${rendr_sources})
target_include_directories(rendr PUBLIC include)

find_package(OpenGL REQUIRED)

target_link_libraries(rendr PUBLIC
	glfw
	glad_lib
	OpenGL::GL
	glm::glm
)

add_executable(demo demo/main.cpp)
target_link_libraries(demo PUBLIC rendr)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
