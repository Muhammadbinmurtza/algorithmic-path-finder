#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AStarPathfinder.h"
#include "BFSPathfinder.h"
#include "DFSPathfinder.h"
#include "DijkstraPathfinder.h"
#include "Maze.h"
#include "Renderer.h"

/**
 * @brief Main project demonstration with interactive menu
 */

// Structure to store algorithm results
struct AlgorithmResult {
  std::string name;
  bool executed = false;
  bool pathFound = false;
  int pathLength = 0;
  int nodesExplored = 0;
  double executionTime = 0.0;
};

void displayMenu() {
  std::cout
      << "\n================================================================\n";
  std::cout << "           PATHFINDING ALGORITHM MENU                       \n";
  std::cout
      << "================================================================\n";
  std::cout << "\n";
  std::cout << "  1. BFS (Breadth-First Search)\n";
  std::cout << "  2. DFS (Depth-First Search)\n";
  std::cout << "  3. Dijkstra's Algorithm\n";
  std::cout << "  4. A* (A-Star) Algorithm\n";
  std::cout << "  5. Run All Algorithms\n";
  std::cout << "  6. Show Comparative Analysis\n";
  std::cout << "  0. Exit\n";
  std::cout << "\n";
  std::cout << "Enter your choice: ";
}

void displayComparativeAnalysis(const std::vector<AlgorithmResult> &results) {
  std::cout
      << "\n================================================================\n";
  std::cout << "              COMPARATIVE ANALYSIS & RESULTS                \n";
  std::cout
      << "================================================================\n";
  std::cout << "\n";

  // Check if any algorithms were executed
  bool anyExecuted = false;
  for (const auto &result : results) {
    if (result.executed) {
      anyExecuted = true;
      break;
    }
  }

  if (!anyExecuted) {
    std::cout << "No algorithms have been executed yet.\n";
    std::cout << "Please run at least one algorithm first.\n";
    return;
  }

  // Display table header
  std::cout << std::left << std::setw(20) << "Algorithm" << std::right
            << std::setw(15) << "Path Length" << std::setw(18)
            << "Nodes Explored" << std::setw(18) << "Time (sec)" << "\n";
  std::cout << std::string(71, '-') << "\n";

  // Display results for each executed algorithm
  for (const auto &result : results) {
    if (result.executed) {
      std::cout << std::left << std::setw(20) << result.name;
      if (result.pathFound) {
        std::cout << std::right << std::setw(15) << result.pathLength
                  << std::setw(18) << result.nodesExplored << std::setw(18)
                  << std::fixed << std::setprecision(2)
                  << (result.executionTime / 1000.0) << "\n";
      } else {
        std::cout << std::right << std::setw(15) << "No path" << std::setw(18)
                  << result.nodesExplored << std::setw(18) << std::fixed
                  << std::setprecision(2) << (result.executionTime / 1000.0)
                  << "\n";
      }
    }
  }

  std::cout << "\n";

  // Analysis summary
  std::cout << "Key Observations:\n";
  std::cout << "  * BFS: Guarantees shortest path, explores level-by-level\n";
  std::cout << "  * DFS: May find longer paths, explores depth-first\n";
  std::cout
      << "  * Dijkstra: Finds shortest path (similar to BFS for unweighted)\n";
  std::cout << "  * A*: Most efficient, uses heuristic to guide search\n";
  std::cout << "\n";
}

void runAlgorithm(int algorithmIndex, Maze &maze, std::shared_ptr<Cell> start,
                  std::shared_ptr<Cell> goal, Renderer &renderer,
                  std::vector<AlgorithmResult> &results) {

  // Reset maze state
  maze.reset();

  // Create appropriate algorithm
  std::unique_ptr<PathFinder> algorithm;
  std::string algorithmName;

  switch (algorithmIndex) {
  case 0: // BFS
    algorithm = std::make_unique<BFSPathfinder>();
    algorithmName = "BFS (Breadth-First Search)";
    break;
  case 1: // DFS
    algorithm = std::make_unique<DFSPathfinder>();
    algorithmName = "DFS (Depth-First Search)";
    break;
  case 2: // Dijkstra
    algorithm = std::make_unique<DijkstraPathfinder>();
    algorithmName = "Dijkstra's Algorithm";
    break;
  case 3: // A*
    algorithm = std::make_unique<AStarPathfinder>();
    algorithmName = "A* (A-Star) Algorithm";
    break;
  default:
    return;
  }

  std::cout
      << "\n===============================================================\n";
  std::cout << "  Running: " << algorithmName << "\n";
  std::cout
      << "===============================================================\n\n";

  // Create animation callback
  auto callback = [&](std::shared_ptr<Cell> cell) {
    renderer.animateStep(maze, cell);
  };

  // Run algorithm with animation
  auto path = algorithm->findPath(maze, start, goal, callback);

  // Display final result
  renderer.clearScreen();

  std::cout
      << "===============================================================\n";
  std::cout << "  " << algorithmName << " - COMPLETE\n";
  std::cout
      << "===============================================================\n\n";

  // Store results
  results[algorithmIndex].executed = true;
  results[algorithmIndex].pathFound = !path.empty();

  if (!path.empty()) {
    // Mark path
    for (auto &cell : path) {
      maze.getCell(cell.getX(), cell.getY())->setPath(true);
    }

    // Draw final maze with path
    renderer.drawMaze(maze);

    // Get and display statistics
    auto stats = algorithm->getStats();
    results[algorithmIndex].pathLength = stats.pathLength;
    results[algorithmIndex].nodesExplored = stats.nodesExplored;
    results[algorithmIndex].executionTime = stats.executionTime.count();

    std::cout << "\n";
    renderer.displayStats(stats.nodesExplored, stats.pathLength,
                          stats.executionTime.count());
    std::cout << "\n✓ Path found!\n";
  } else {
    auto stats = algorithm->getStats();
    results[algorithmIndex].nodesExplored = stats.nodesExplored;
    results[algorithmIndex].executionTime = stats.executionTime.count();
    std::cout << "\n✗ No path found!\n";
  }

  std::cout << "\nPress Enter to return to menu...";
  std::cin.ignore();
  std::cin.get();
}

int main() {
  std::cout
      << "================================================================\n";
  std::cout << "     MAZE PATHFINDING - INTERACTIVE DEMONSTRATION          \n";
  std::cout
      << "================================================================\n\n";

  // Create and generate maze
  Maze maze;
  std::cout << "Generating random perfect maze...\n";
  maze.generatePerfectMaze(25, 20,
                           MazeGenerationAlgorithm::RECURSIVE_BACKTRACKING);

  auto start = maze.getStart();
  auto goal = maze.getGoal();

  std::cout << "Maze generated: " << maze.getWidth() << "x" << maze.getHeight()
            << "\n";
  std::cout << "Start: (" << start->getX() << ", " << start->getY() << ")\n";
  std::cout << "Goal: (" << goal->getX() << ", " << goal->getY() << ")\n";

  // Create renderer
  Renderer renderer;
  renderer.setAnimationSpeed(30); // 30ms between frames
  renderer.setColorMode(true);
  renderer.setShowLegend(true);

  // Show initial maze
  std::cout << "\nInitial Maze:\n";
  renderer.drawMaze(maze);

  // Initialize results storage
  std::vector<AlgorithmResult> results(4);
  results[0].name = "BFS";
  results[1].name = "DFS";
  results[2].name = "Dijkstra";
  results[3].name = "A*";

  // Main menu loop
  bool running = true;
  while (running) {
    displayMenu();

    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "\nInvalid input! Please enter a number.\n";
      continue;
    }

    switch (choice) {
    case 1: // BFS
      runAlgorithm(0, maze, start, goal, renderer, results);
      break;

    case 2: // DFS
      runAlgorithm(1, maze, start, goal, renderer, results);
      break;

    case 3: // Dijkstra
      runAlgorithm(2, maze, start, goal, renderer, results);
      break;

    case 4: // A*
      runAlgorithm(3, maze, start, goal, renderer, results);
      break;

    case 5: // Run All
      std::cout << "\nRunning all algorithms...\n";
      for (int i = 0; i < 4; i++) {
        runAlgorithm(i, maze, start, goal, renderer, results);
      }
      break;

    case 6: // Show Analysis
      displayComparativeAnalysis(results);
      std::cout << "Press Enter to continue...";
      std::cin.ignore();
      std::cin.get();
      break;

    case 0: // Exit
      std::cout << "\n";
      displayComparativeAnalysis(results);
      std::cout << "\n========================================================="
                   "=======\n";
      std::cout
          << "                  Thank you for using!                      \n";
      std::cout << "==========================================================="
                   "=====\n";
      running = false;
      break;

    default:
      std::cout << "\nInvalid choice! Please select 0-6.\n";
      std::cin.ignore();
    }
  }

  return 0;
}
