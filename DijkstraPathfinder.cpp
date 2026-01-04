#include "DijkstraPathfinder.h"
#include <algorithm>
#include <chrono>
#include <set>

std::vector<Cell> DijkstraPathfinder::findPath(
    const Maze &maze, std::shared_ptr<Cell> start, std::shared_ptr<Cell> goal,
    std::function<void(std::shared_ptr<Cell>)> onVisit) {
  auto startTime = std::chrono::high_resolution_clock::now();

  // Initialize
  path.clear();
  stats = {};

  if (!start || !goal) {
    return path;
  }

  // Priority queue for Dijkstra's (min-heap)
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

  // Map to track costs to reach each cell
  std::map<std::shared_ptr<Cell>, double> cost;

  // Map to track parent cells for path reconstruction
  std::map<std::shared_ptr<Cell>, std::shared_ptr<Cell>> parent;

  // Set to track visited cells
  std::set<std::shared_ptr<Cell>> visited;

  // Initialize start
  cost[start] = 0.0;
  parent[start] = nullptr;
  pq.push({start, 0.0});
  stats.nodesExplored = 0;

  bool found = false;

  // Dijkstra's main loop
  while (!pq.empty() && !found) {
    Node current = pq.top();
    pq.pop();

    // Skip if already visited
    if (visited.find(current.cell) != visited.end()) {
      continue;
    }

    visited.insert(current.cell);
    current.cell->setVisited(true);
    stats.nodesExplored++;

    // Call animation callback if provided
    if (onVisit) {
      onVisit(current.cell);
    }

    // Check if we reached the goal
    if (*current.cell == *goal) {
      found = true;
      break;
    }

    // Explore neighbors
    auto neighbors = maze.getNeighbors(current.cell);
    for (auto &neighbor : neighbors) {
      if (visited.find(neighbor) == visited.end()) {
        double newCost = current.cost + 1.0; // Uniform cost of 1 per move

        // If we found a better path to this neighbor
        if (cost.find(neighbor) == cost.end() || newCost < cost[neighbor]) {
          cost[neighbor] = newCost;
          parent[neighbor] = current.cell;
          pq.push({neighbor, newCost});
        }
      }
    }
  }

  // Reconstruct path if found
  if (found) {
    auto current = goal;
    while (current != nullptr) {
      path.push_back(*current);
      current = parent[current];
    }
    std::reverse(path.begin(), path.end());
    stats.pathLength = path.size();
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  stats.executionTime =
      std::chrono::duration<double, std::milli>(endTime - startTime);

  return path;
}
