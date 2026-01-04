#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Cell.h"
#include "PathStats.h"
#include <functional>
#include <memory>
#include <vector>

// Forward declaration for the Maze class.
// The PathFinder holds a reference to the maze it operates on, but doesn't own
// it.
class Maze;

/**
 * @brief Abstract base class for all pathfinding algorithms.
 *
 * This class defines the common interface for pathfinders. It follows the
 * Strategy design pattern, where each concrete algorithm (like BFS, DFS, A*)
 * is a separate 'strategy' for finding a path.
 *
 * The core application can work with any algorithm through this abstract
 * interface, enabling polymorphism. This design promotes loose coupling and
 * makes the system easily extensible: a new algorithm can be added by simply
 * creating a new class that inherits from PathFinder, with no changes to the
 * main application logic.
 *
 * Const correctness is used throughout to ensure that methods that should not
 * modify the object's state are marked as const.
 */
class PathFinder {
public:
  // Virtual destructor to ensure proper cleanup of derived classes.
  virtual ~PathFinder() = default;

  /**
   * @brief The primary pathfinding method that all concrete classes must
   * implement. This is a pure virtual function, making PathFinder an abstract
   * class.
   *
   * @param maze A reference to the maze grid to be traversed.
   * @param start A smart pointer to the starting cell. Using std::shared_ptr
   * for safety.
   * @param goal A smart pointer to the goal cell.
   * @param onVisit Optional callback function called when a cell is visited
   * (for animation). Defaults to nullptr for backward compatibility.
   * @return A std::vector<Cell> representing the sequence of cells in the found
   * path. Returns an empty vector if no path is found.
   */
  virtual std::vector<Cell>
  findPath(const Maze &maze, std::shared_ptr<Cell> start,
           std::shared_ptr<Cell> goal,
           std::function<void(std::shared_ptr<Cell>)> onVisit = nullptr) = 0;

  /**
   * @brief Initializes the algorithm's state before a run.
   * Resets statistics and any stored path data.
   */
  virtual void initialize() {
    stats = {}; // Zero-initialize the stats struct
    path.clear();
    // The visited array/state is managed by the Maze/Cell objects,
    // so we call the maze's reset method.
  }

  /**
   * @brief Resets the algorithm to its initial state.
   */
  virtual void reset() { initialize(); }

  /**
   * @brief Retrieves the performance statistics of the last algorithm run.
   * @return A const PathStats struct containing the collected metrics.
   */
  virtual PathStats getStats() const { return stats; }

protected:
  // A vector to store the sequence of cells forming the path.
  std::vector<Cell> path;

  // A 2D array to keep track of visited cells. This is managed by the concrete
  // algorithm.
  std::vector<std::vector<bool>> visited;

  // Struct to hold performance data.
  PathStats stats;
};

#endif // PATHFINDER_H
