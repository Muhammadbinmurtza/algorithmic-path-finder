#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AStarPathfinder.h"
#include "BFSPathfinder.h"
#include "DFSPathfinder.h"
#include "DijkstraPathfinder.h"
#include "Maze.h"
#include "MazeVisualizer.h"
#include "PathFinder.h"

/**
 * @brief Main application entry point for the Maze Pathfinding project.
 *
 * This program demonstrates various pathfinding algorithms on a maze.
 */
int main() {
  std::cout << "=== Maze Pathfinding Algorithm Comparison ===" << std::endl;
  std::cout << std::endl;

  // Create a sample maze
  Maze maze;

  // Generate a random perfect maze (new maze each run!)
  std::cout << "Generating random perfect maze..." << std::endl;
  maze.generatePerfectMaze(20, 15,
                           MazeGenerationAlgorithm::RECURSIVE_BACKTRACKING);
  std::cout << "Generated maze: " << maze.getWidth() << "x" << maze.getHeight()
            << std::endl;

  std::cout << std::endl;

  // Get start and goal positions
  auto start = maze.getStart();
  auto goal = maze.getGoal();

  if (!start || !goal) {
    std::cerr << "Error: Start or goal position not set!" << std::endl;
    return 1;
  }

  std::cout << "Start: (" << start->getX() << ", " << start->getY() << ")"
            << std::endl;
  std::cout << "Goal: (" << goal->getX() << ", " << goal->getY() << ")"
            << std::endl;
  std::cout << std::endl;

  // Display the initial maze
  std::cout << "Initial Maze:" << std::endl;
  MazeVisualizer::displayMaze(maze);

  // Create algorithm instances
  std::vector<std::pair<std::string, std::unique_ptr<PathFinder>>> algorithms;
  algorithms.push_back({"BFS", std::make_unique<BFSPathfinder>()});
  algorithms.push_back({"DFS", std::make_unique<DFSPathfinder>()});
  algorithms.push_back({"Dijkstra", std::make_unique<DijkstraPathfinder>()});
  algorithms.push_back({"A*", std::make_unique<AStarPathfinder>()});

  // Store results for comparison
  std::vector<std::pair<std::string, PathStats>> results;

  // Run each algorithm
  std::cout << "Running pathfinding algorithms..." << std::endl;
  std::cout << std::endl;

  for (auto &[name, algorithm] : algorithms) {
    // Reset maze state before each algorithm
    maze.reset();

    // Run the algorithm
    algorithm->initialize();
    auto path = algorithm->findPath(maze, start, goal);
    auto stats = algorithm->getStats();

    // Store results
    results.push_back({name, stats});

    // Print individual result
    std::cout << name << ": ";
    if (stats.pathLength > 0) {
      std::cout << "Path found!" << std::endl;
    } else {
      std::cout << "No path found." << std::endl;
    }
  }

  std::cout << std::endl;
  std::cout << "=== Performance Comparison ===" << std::endl;
  std::cout << std::endl;

  // Print comparison table header
  std::cout << std::left << std::setw(15) << "Algorithm" << std::right
            << std::setw(15) << "Path Length" << std::setw(18)
            << "Nodes Explored" << std::setw(20) << "Time (ms)" << std::endl;
  std::cout << std::string(68, '-') << std::endl;

  // Print results for each algorithm
  for (const auto &[name, stats] : results) {
    std::cout << std::left << std::setw(15) << name << std::right
              << std::setw(15) << stats.pathLength << std::setw(18)
              << stats.nodesExplored << std::setw(20) << std::fixed
              << std::setprecision(4) << stats.executionTime.count()
              << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Comparison complete!" << std::endl;

  // Display path visualization for the first algorithm (BFS)
  if (!results.empty() && results[0].second.pathLength > 0) {
    std::cout << std::endl;
    std::cout << "=== Path Visualization (BFS) ===" << std::endl;

    // Re-run BFS to get the path for visualization
    maze.reset();
    BFSPathfinder bfs;
    auto path = bfs.findPath(maze, start, goal);
    MazeVisualizer::displayMazeWithPath(maze, path);

    std::cout << "Legend: S=Start, G=Goal, *=Path, #=Wall" << std::endl;
  }

  return 0;
}
