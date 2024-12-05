cmake_minimum_required(VERSION 3.10)
project(3DVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)


# Add executable
add_executable(3DVisualizer ${SOURCES})

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.30.9
)

FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/nigels-com/glew.git
    GIT_TAG        glew-2.2.0
)

FetchContent_MakeAvailable(glew)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# Find SDL2
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    message(STATUS "SDL2 found: ${SDL2_LIBRARIES}")
    include_directories(${SDL2_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "SDL2 not found")
endif()

# Link libraries
target_link_libraries(3DVisualizer
    dl
    pthread
    SDL2
    GLEW
)


if (WIN32)
    target_link_libraries(3DVisualizer opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(3DVisualizer ${OpenGL_LIBRARY})
else()
    target_link_libraries(3DVisualizer GL)
endif()

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(3DVisualizer copy_assets)
