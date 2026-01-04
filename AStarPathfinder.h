#ifndef ASTAR_PATHFINDER_H
#define ASTAR_PATHFINDER_H

#include "Maze.h"
#include "PathFinder.h"
#include <cmath>
#include <map>
#include <queue>

/**
 * @brief A* pathfinding algorithm implementation.
 *
 * A* uses a heuristic function to guide the search toward the goal,
 * combining the benefits of Dijkstra's algorithm with informed search.
 * Guarantees the shortest path if the heuristic is admissible.
 */
class AStarPathfinder : public PathFinder {
public:
  /**
   * @brief Find a path from start to goal using A* algorithm.
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

private:
  /**
   * @brief Node structure for priority queue with f-score.
   */
  struct Node {
    std::shared_ptr<Cell> cell;
    double fScore; // f = g + h

    bool operator>(const Node &other) const { return fScore > other.fScore; }
  };

  /**
   * @brief Calculate Manhattan distance heuristic.
   * @param from The starting cell.
   * @param to The goal cell.
   * @return The Manhattan distance.
   */
  double heuristic(std::shared_ptr<Cell> from, std::shared_ptr<Cell> to) const;
};

#endif // ASTAR_PATHFINDER_H
