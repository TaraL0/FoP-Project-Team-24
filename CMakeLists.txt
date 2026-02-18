cmake_minimum_required(VERSION 4.1)
project(P1)

set(CMAKE_CXX_STANDARD 20)

add_executable(P1 main.cpp
        motion_block.h
        looks_block.h
        sound_block.h
        sprite.h
        setup_screen.h
        add_extension_pen.h
        file_menu.h
        costume_menu.h
        control_block.h
        sensing_block.h
        operator_block.h
        variable_block.h
        myblock_block.h
        event_block.h
        debugger.h)

target_link_libraries (${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)