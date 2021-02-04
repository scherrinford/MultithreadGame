cmake_minimum_required(VERSION 3.17)
project(untitled3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-lncurses")



add_executable(untitled3 main.cpp Legend.cpp PlayerHandler.cpp Maze.cpp Beast.cpp)