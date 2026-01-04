#include "BFSPathfinder.h"
#include <algorithm>
#include <chrono>

std::vector<Cell>
BFSPathfinder::findPath(const Maze &maze, std::shared_ptr<Cell> start,
                        std::shared_ptr<Cell> goal,
                        std::function<void(std::shared_ptr<Cell>)> onVisit) {
  auto startTime = std::chrono::high_resolution_clock::now();

  // Initialize
  path.clear();
  stats = {};

  if (!start || !goal) {
    return path;
  }

  // Queue for BFS
  std::queue<std::shared_ptr<Cell>> queue;

  // Map to track parent cells for path reconstruction
  std::map<std::shared_ptr<Cell>, std::shared_ptr<Cell>> parent;

  // Mark start as visited and enqueue
  start->setVisited(true);
  queue.push(start);
  parent[start] = nullptr;
  stats.nodesExplored = 0;

  bool found = false;

  // BFS main loop
  while (!queue.empty() && !found) {
    auto current = queue.front();
    queue.pop();
    stats.nodesExplored++;

    // Call animation callback if provided
    if (onVisit) {
      onVisit(current);
    }

    // Check if we reached the goal
    if (*current == *goal) {
      found = true;
      break;
    }

    // Explore neighbors
    auto neighbors = maze.getNeighbors(current);
    for (auto &neighbor : neighbors) {
      if (!neighbor->isVisited()) {
        neighbor->setVisited(true);
        parent[neighbor] = current;
        queue.push(neighbor);
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
