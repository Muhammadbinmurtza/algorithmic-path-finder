#include "AStarPathfinder.h"
#include <algorithm>
#include <chrono>
#include <set>

double AStarPathfinder::heuristic(std::shared_ptr<Cell> from,
                                  std::shared_ptr<Cell> to) const {
  // Manhattan distance
  return std::abs(from->getX() - to->getX()) +
         std::abs(from->getY() - to->getY());
}

std::vector<Cell>
AStarPathfinder::findPath(const Maze &maze, std::shared_ptr<Cell> start,
                          std::shared_ptr<Cell> goal,
                          std::function<void(std::shared_ptr<Cell>)> onVisit) {
  auto startTime = std::chrono::high_resolution_clock::now();

  // Initialize
  path.clear();
  stats = {};

  if (!start || !goal) {
    return path;
  }

  // Priority queue for A* (min-heap based on f-score)
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

  // Map to track g-scores (cost from start)
  std::map<std::shared_ptr<Cell>, double> gScore;

  // Map to track parent cells for path reconstruction
  std::map<std::shared_ptr<Cell>, std::shared_ptr<Cell>> parent;

  // Set to track visited cells
  std::set<std::shared_ptr<Cell>> closedSet;

  // Initialize start
  gScore[start] = 0.0;
  double fScore = heuristic(start, goal);
  parent[start] = nullptr;
  openSet.push({start, fScore});
  stats.nodesExplored = 0;

  bool found = false;

  // A* main loop
  while (!openSet.empty() && !found) {
    Node current = openSet.top();
    openSet.pop();

    // Skip if already visited
    if (closedSet.find(current.cell) != closedSet.end()) {
      continue;
    }

    closedSet.insert(current.cell);
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
      if (closedSet.find(neighbor) == closedSet.end()) {
        double tentativeGScore =
            gScore[current.cell] + 1.0; // Cost of 1 per move

        // If we found a better path to this neighbor
        if (gScore.find(neighbor) == gScore.end() ||
            tentativeGScore < gScore[neighbor]) {
          gScore[neighbor] = tentativeGScore;
          double fScore = tentativeGScore + heuristic(neighbor, goal);
          parent[neighbor] = current.cell;
          openSet.push({neighbor, fScore});
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
