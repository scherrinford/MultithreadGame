cmake_minimum_required(VERSION 3.17)
project(MultithreadGame)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-lncurses")


add_executable(MultithreadGame main.cpp Legend.cpp Player.cpp Server.cpp Maze.cpp Beast.cpp Client.cpp)