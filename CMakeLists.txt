cmake_minimum_required(VERSION 3.10)
project(3DVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
    "external/glad/src/glad.c"
)


# Add executable
add_executable(3DVisualizer ${SOURCES})

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8  # Use the latest stable release
)

FetchContent_MakeAvailable(glfw)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${glfw_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(3DVisualizer
    glfw
    dl  # For dynamic loading on Linux
    pthread  # For threading on Linux
)

# On Windows, you may need to link against opengl32
if (WIN32)
    target_link_libraries(3DVisualizer opengl32)
elseif(APPLE)
    # On macOS, link against OpenGL framework
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(3DVisualizer ${OpenGL_LIBRARY})
else()
    # On Linux, link against GL
    target_link_libraries(3DVisualizer GL)
endif()

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(3DVisualizer copy_assets)
