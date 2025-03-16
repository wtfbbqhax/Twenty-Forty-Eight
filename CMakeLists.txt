cmake_minimum_required(VERSION 3.20)
project(TwentyFortyEight VERSION 1.0 DESCRIPTION "A simple console 2048 game")

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(2048
    main.cc
    game.cc
    game.h
    cmds.cc
    cmds.h
    display.cc
    display.h
    debug.h
    q_trie.c q_trie.h
    cvar.c cvar.h
    glob.c glob.h
    prompt.c prompt.h
)

# Set compiler flags
target_compile_options(2048 PRIVATE -Wall -Wextra -pedantic)

# Install the binary into the bin directory
install(TARGETS 2048 DESTINATION bin)

