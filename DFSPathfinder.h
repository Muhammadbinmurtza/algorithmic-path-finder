#ifndef DFS_PATHFINDER_H
#define DFS_PATHFINDER_H

#include "Maze.h"
#include "PathFinder.h"
#include <map>
#include <stack>

/**
 * @brief Depth-First Search pathfinding algorithm implementation.
 *
 * DFS explores as far as possible along each branch before backtracking.
 * Does not guarantee the shortest path.
 */
class DFSPathfinder : public PathFinder {
public:
  /**
   * @brief Find a path from start to goal using DFS.
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

#endif // DFS_PATHFINDER_H
