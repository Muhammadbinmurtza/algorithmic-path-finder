#ifndef MAZE_VISUALIZER_H
#define MAZE_VISUALIZER_H

#include "Cell.h"
#include "Maze.h"
#include <vector>


/**
 * @brief Console-based maze visualization class.
 *
 * This class provides methods to display the maze and paths in the console
 * using ASCII characters.
 */
class MazeVisualizer {
public:
  /**
   * @brief Display the maze in the console.
   * @param maze The maze to display.
   */
  static void displayMaze(const Maze &maze);

  /**
   * @brief Display the maze with a highlighted path.
   * @param maze The maze to display.
   * @param path The path to highlight.
   */
  static void displayMazeWithPath(const Maze &maze,
                                  const std::vector<Cell> &path);

  /**
   * @brief Display the maze with visited cells highlighted.
   * @param maze The maze to display.
   */
  static void displayMazeWithVisited(const Maze &maze);

private:
  /**
   * @brief Get the character representation for a cell.
   * @param cell The cell to represent.
   * @param isInPath Whether the cell is part of the path.
   * @param isStart Whether the cell is the start position.
   * @param isGoal Whether the cell is the goal position.
   * @return The character to display.
   */
  static char getCellChar(std::shared_ptr<Cell> cell, bool isInPath,
                          bool isStart, bool isGoal);
};

#endif // MAZE_VISUALIZER_H
