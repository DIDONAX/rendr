include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.6
    SOURCE_SUBDIR cmake
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)

FetchContent_MakeAvailable(glfw glad glm)

if(APPLE)
    glad_add_library(glad_lib STATIC REPRODUCIBLE LOADER API gl:core=4.1)
else()
    glad_add_library(glad_lib STATIC REPRODUCIBLE LOADER API gl:core=4.6)
endif()
