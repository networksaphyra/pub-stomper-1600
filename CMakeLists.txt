cmake_minimum_required(VERSION 3.10)

project(PubStomper VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(PubStomper
    src/bitboard.cpp
    src/board.cpp
    src/constants.cpp
    src/evaluate.cpp
    src/main.cpp
    src/move.cpp
    src/movegen.cpp
    src/movetable.cpp
)
