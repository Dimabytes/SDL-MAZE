cmake_minimum_required(VERSION 3.7)
project(maze)
set(CMAKE_CXX_STANDARD 11)

add_executable(maze main.cpp menu.cpp maze.cpp)

target_link_libraries(maze SDL)
target_link_libraries(maze SDL_ttf)
target_link_libraries(maze SDL_draw)


