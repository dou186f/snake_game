cmake_minimum_required(VERSION 3.16)
project(snake_game)

set(CMAKE_CXX_STANDARD 17)


include(FetchContent)

FetchContent_Declare(
	raylib
	SOURCE_DIR /home/dou/Desktop/Projects/raylib-5.0
)


FetchContent_MakeAvailable(raylib)

add_executable(snake_game 
	src/main.cpp
	src/Snake.cpp
	src/Food.cpp)

target_link_libraries(snake_game PRIVATE raylib)
