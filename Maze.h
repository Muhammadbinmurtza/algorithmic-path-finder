#ifndef MAZE_H
#define MAZE_H

#include <memory>
#include <random>
#include <string>
#include <vector>


#include "Cell.h"

/**
 * @brief Enum for maze generation algorithms.
 */
enum class MazeGenerationAlgorithm {
  RECURSIVE_BACKTRACKING, // DFS-based, long corridors
  RANDOMIZED_PRIMS,       // Tree-like, branchy
  RECURSIVE_DIVISION      // Room-based, geometric
};

/**
 * @brief Represents a 2D maze grid composed of Cell objects.
 *
 * The Maze class manages the grid structure, provides methods to load/generate
 * mazes, and offers utilities to access cells and their neighbors.
 */
class Maze {
public:
  /**
   * @brief Construct a new Maze object with specified dimensions.
   * @param width The width of the maze (number of columns).
   * @param height The height of the maze (number of rows).
   */
  Maze(int width = 0, int height = 0);

  /**
   * @brief Load a maze from a text file.
   *
   * File format:
   * - First line: width height
   * - Following lines: grid representation where:
   *   - '.' or ' ' = open cell
   *   - '#' or '1' = wall
   *   - 'S' = start position
   *   - 'G' = goal position
   *
   * @param filename Path to the maze file.
   * @return true if the maze was loaded successfully, false otherwise.
   */
  bool loadFromFile(const std::string &filename);

  /**
   * @brief Generate a simple maze programmatically.
   * Creates a maze with borders and some internal walls.
   * @param width The width of the maze.
   * @param height The height of the maze.
   */
  void generate(int width, int height);

  /**
   * @brief Generate a perfect maze using specified algorithm.
   * Creates a random, solvable maze with exactly one path between any two
   * points.
   * @param width The width of the maze.
   * @param height The height of the maze.
   * @param algorithm The generation algorithm to use (default:
   * RECURSIVE_BACKTRACKING).
   */
  void generatePerfectMaze(int width, int height,
                           MazeGenerationAlgorithm algorithm =
                               MazeGenerationAlgorithm::RECURSIVE_BACKTRACKING);

  /**
   * @brief Get a reference to a cell at the specified coordinates.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @return A shared pointer to the Cell at (x, y).
   * @throws std::out_of_range if coordinates are invalid.
   */
  std::shared_ptr<Cell> getCell(int x, int y) const;

  /**
   * @brief Get all valid neighbors of a cell (not walls, within bounds).
   * @param cell The cell whose neighbors to find.
   * @return A vector of shared pointers to neighboring cells.
   */
  std::vector<std::shared_ptr<Cell>>
  getNeighbors(std::shared_ptr<Cell> cell) const;

  /**
   * @brief Check if coordinates are within the maze bounds.
   * @param x The x-coordinate.
   * @param y The y-coordinate.
   * @return true if the coordinates are valid, false otherwise.
   */
  bool isValid(int x, int y) const;

  /**
   * @brief Reset all cells in the maze to their initial state.
   * Clears visited and path flags but preserves wall status.
   */
  void reset();

  /**
   * @brief Get the width of the maze.
   * @return The width (number of columns).
   */
  int getWidth() const;

  /**
   * @brief Get the height of the maze.
   * @return The height (number of rows).
   */
  int getHeight() const;

  /**
   * @brief Get the start cell of the maze.
   * @return A shared pointer to the start cell.
   */
  std::shared_ptr<Cell> getStart() const;

  /**
   * @brief Get the goal cell of the maze.
   * @return A shared pointer to the goal cell.
   */
  std::shared_ptr<Cell> getGoal() const;

  /**
   * @brief Set the start cell of the maze.
   * @param x The x-coordinate of the start cell.
   * @param y The y-coordinate of the start cell.
   */
  void setStart(int x, int y);

  /**
   * @brief Set the goal cell of the maze.
   * @param x The x-coordinate of the goal cell.
   * @param y The y-coordinate of the goal cell.
   */
  void setGoal(int x, int y);

  /**
   * @brief Print the maze to console for debugging.
   * Shows walls (#), start (S), goal (G), and empty spaces.
   */
  void printMaze() const;

private:
  int width;
  int height;
  std::vector<std::vector<std::shared_ptr<Cell>>> grid;
  std::shared_ptr<Cell> start;
  std::shared_ptr<Cell> goal;

  /**
   * @brief Initialize the grid with Cell objects.
   */
  void initializeGrid();

  /**
   * @brief Validate maze dimensions.
   * @param width The width to validate.
   * @param height The height to validate.
   * @throws std::invalid_argument if dimensions are out of range.
   */
  void validateDimensions(int width, int height) const;

  // Maze generation algorithm implementations
  void generateRecursiveBacktracking();
  void generateRandomizedPrims();
  void generateRecursiveDivision();

  // Helper methods for maze generation
  std::vector<std::shared_ptr<Cell>>
  getUnvisitedNeighbors(std::shared_ptr<Cell> cell) const;
  void removeWallBetween(std::shared_ptr<Cell> cell1,
                         std::shared_ptr<Cell> cell2);
  void addWallBetween(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2);

  // Random number generator
  mutable std::mt19937 rng;
};

#endif // MAZE_H
