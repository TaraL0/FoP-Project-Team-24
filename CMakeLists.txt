cmake_minimum_required(VERSION 4.1)
project(P1)

set(CMAKE_CXX_STANDARD 20)

add_executable(P1 main.cpp
        motion_block.h
        looks_block.h
        sound_block.h
        sprite.h)
