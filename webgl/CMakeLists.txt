cmake_minimum_required(VERSION 3.21)
project(snake_game_web LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if(EMSCRIPTEN)
    set(PLATFORM Web CACHE STRING "" FORCE)
endif()

if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "Use emcmake cmake -S webgl -B webgl/build")
endif()

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
FetchContent_MakeAvailable(raylib)

add_executable(snake_game
    src/main.cpp
    src/Snake.cpp
    src/Food.cpp
)
target_link_libraries(snake_game PRIVATE raylib)


if(EMSCRIPTEN)
    set_target_properties(snake_game PROPERTIES
        OUTPUT_NAME index
        SUFFIX ".html"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/site"
    )
    set_property(TARGET snake_game APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shell.html")
    target_link_options(snake_game PRIVATE
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
        "-sUSE_GLFW=3"
        "-sASYNCIFY"
        "-sALLOW_MEMORY_GROWTH=1"
    )
endif()
