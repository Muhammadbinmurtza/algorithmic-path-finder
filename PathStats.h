#ifndef PATH_STATS_H
#define PATH_STATS_H

#include <chrono>

/**
 * @brief Enum for representing the four cardinal directions.
 * Used to determine neighbor cells in the maze.
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @brief A simple struct to hold X, Y coordinates.
 * Includes comparison operators for easy use in data structures.
 */
struct Coordinate {
    int x = 0;
    int y = 0;

    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Coordinate& other) const {
        return !(*this == other);
    }
};


/**
 * @brief Struct to hold performance statistics from a pathfinding algorithm run.
 * This allows for easy comparison between different algorithms.
 */
struct PathStats {
    int pathLength = 0;
    int nodesExplored = 0;
    std::chrono::duration<double, std::milli> executionTime{0};
};

#endif // PATH_STATS_H
