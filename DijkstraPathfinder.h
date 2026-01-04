#ifndef DIJKSTRA_PATHFINDER_H
#define DIJKSTRA_PATHFINDER_H

#include "Maze.h"
#include "PathFinder.h"
#include <limits>
#include <map>
#include <queue>

/**
 * @brief Dijkstra's pathfinding algorithm implementation.
 *
 * Dijkstra's algorithm finds the shortest path in a weighted graph.
 * For uniform cost (all edges have weight 1), it behaves similarly to BFS.
 */
class DijkstraPathfinder : public PathFinder {
public:
  /**
   * @brief Find a path from start to goal using Dijkstra's algorithm.
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
   * @brief Node structure for priority queue.
   */
  struct Node {
    std::shared_ptr<Cell> cell;
    double cost;

    bool operator>(const Node &other) const { return cost > other.cost; }
  };
};

#endif // DIJKSTRA_PATHFINDER_H
