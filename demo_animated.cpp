#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "BFSPathfinder.h"
#include "Maze.h"
#include "Renderer.h"

/**
 * @brief Minimal demo of the new Renderer with BFS animation
 */
int main() {
  std::cout << "=== Animated Maze Pathfinding Demo ===" << std::endl;
  std::cout << std::endl;

  // Create a maze
  Maze maze;
  std::cout << "Generating random perfect maze..." << std::endl;
  maze.generatePerfectMaze(20, 15,
                           MazeGenerationAlgorithm::RECURSIVE_BACKTRACKING);

  auto start = maze.getStart();
  auto goal = maze.getGoal();

  std::cout << "Maze generated: " << maze.getWidth() << "x" << maze.getHeight()
            << std::endl;
  std::cout << "Start: (" << start->getX() << ", " << start->getY() << ")"
            << std::endl;
  std::cout << "Goal: (" << goal->getX() << ", " << goal->getY() << ")"
            << std::endl;
  std::cout << std::endl;
  std::cout << "Press Enter to start BFS animation...";
  std::cin.get();

  // Create renderer
  Renderer renderer;
  renderer.setAnimationSpeed(50); // 50ms between frames
  renderer.setColorMode(true);
  renderer.setShowLegend(true);

  // Create BFS pathfinder
  BFSPathfinder bfs;

  // Run BFS with animation callback
  auto callback = [&](std::shared_ptr<Cell> cell) {
    renderer.animateStep(maze, cell);
  };

  std::cout << "Running BFS with animation..." << std::endl;
  auto path = bfs.findPath(maze, start, goal, callback);

  // Display final result
  renderer.clearScreen();

  if (!path.empty()) {
    std::cout << "Path found!" << std::endl;

    // Mark path
    for (auto &cell : path) {
      maze.getCell(cell.getX(), cell.getY())->setPath(true);
    }

    // Draw final maze with path
    renderer.drawMaze(maze);

    // Show statistics
    auto stats = bfs.getStats();
    renderer.displayStats(stats.nodesExplored, stats.pathLength,
                          stats.executionTime.count());
  } else {
    std::cout << "No path found!" << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Press Enter to exit...";
  std::cin.get();

  return 0;
}
