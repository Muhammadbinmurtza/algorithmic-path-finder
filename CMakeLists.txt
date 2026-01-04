cmake_minimum_required(VERSION 3.10)
project(MazePathfinding)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(maze_pathfinding
    main.cpp
    Cell.cpp
    Maze.cpp
    BFSPathfinder.cpp
    DFSPathfinder.cpp
    DijkstraPathfinder.cpp
    AStarPathfinder.cpp
    MazeVisualizer.cpp
)

# Include directories (current directory)
target_include_directories(maze_pathfinding PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable warnings
if(MSVC)
    target_compile_options(maze_pathfinding PRIVATE /W4)
else()
    target_compile_options(maze_pathfinding PRIVATE -Wall -Wextra -Wpedantic)
endif()
