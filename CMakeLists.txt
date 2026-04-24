cmake_minimum_required(VERSION 3.16)
project(rendr LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GLAD_REBUILD OFF CACHE BOOL "")
option(BUILD_DOCS "Build documentation" OFF)

file(GLOB rendr-source src/*.cpp)
include(FetchContent)


# GLFW
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG master
	GIT_PROGRESS TRUE
)

# GLAD
FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG v2.0.6
	GIT_PROGRESS TRUE
	SOURCE_SUBDIR cmake
)

# GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glfw glad glm)

glad_add_library(glad_lib STATIC REPRODUCIBLE LOADER API gl:core=4.6)

add_library(rendr STATIC ${rendr-source})

target_include_directories(rendr PUBLIC include)

find_package(OpenGL REQUIRED)

target_link_libraries(rendr PUBLIC glfw glad_lib OpenGL::GL glm::glm)

add_executable(demo demo/main.cpp)
target_link_libraries(demo PUBLIC rendr)
