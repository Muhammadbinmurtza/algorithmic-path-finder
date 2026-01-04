#ifndef BFS_PATHFINDER_H
#define BFS_PATHFINDER_H

#include "Maze.h"
#include "PathFinder.h"
#include <map>
#include <queue>

/**
 * @brief Breadth-First Search pathfinding algorithm implementation.
 *
 * BFS explores the maze level by level, guaranteeing the shortest path
 * in terms of number of steps (unweighted graph).
 */
class BFSPathfinder : public PathFinder {
public:
  /**
   * @brief Find a path from start to goal using BFS.
   * @param maze The maze to search.
   * @param start The starting cell.
   * @param goal The goal cell.
   * @param onVisit Optional callback for animation (default: nullptr).
   * @return A vector of cells representing the path, or empty if no path
   * exists.
   */
  std::vector<Cell> findPath(
      const Maze &maze, std::shared_ptr<Cell> start, std::shared_ptr<Cell> goal,
      std::function<void(std::shared_ptr<Cell>)> onVisit = nullptr) override;
};

#endif // BFS_PATHFINDER_H
