#ifndef RENDERER_H
#define RENDERER_H

#include "Cell.h"
#include "Maze.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>


// Cross-platform includes
#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @brief Color codes for console output
 */
enum class Color {
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  GRAY,
  BRIGHT_RED,
  BRIGHT_GREEN,
  BRIGHT_YELLOW,
  BRIGHT_BLUE,
  BRIGHT_MAGENTA,
  BRIGHT_CYAN,
  BRIGHT_WHITE,
  RESET
};

/**
 * @brief Character sets for maze rendering
 */
enum class CharSet {
  ASCII,      // # . * S G
  UNICODE,    // █ ░ ● ◉ ◎
  BOX_DRAWING // ─ │ ┌ ┐ └ ┘
};

/**
 * @brief Advanced ASCII visualization engine with color support and animation
 */
class Renderer {
public:
  /**
   * @brief Construct a new Renderer object
   */
  Renderer();

  /**
   * @brief Destroy the Renderer object
   */
  ~Renderer();

  // ========================================================================
  // CORE RENDERING METHODS
  // ========================================================================

  /**
   * @brief Draw the entire maze
   * @param maze The maze to render
   */
  void drawMaze(const Maze &maze);

  /**
   * @brief Draw the final path
   * @param path Vector of cells representing the path
   */
  void drawPath(const std::vector<std::shared_ptr<Cell>> &path);

  /**
   * @brief Clear the console screen
   */
  void clearScreen();

  // ========================================================================
  // ANIMATION METHODS
  // ========================================================================

  /**
   * @brief Animate a single step of the algorithm
   * @param maze The current maze state
   * @param current The cell being explored
   */
  void animateStep(const Maze &maze, std::shared_ptr<Cell> current);

  /**
   * @brief Set animation speed
   * @param ms Delay in milliseconds between frames
   */
  void setAnimationSpeed(int ms);

  /**
   * @brief Pause the animation
   */
  void pauseAnimation();

  /**
   * @brief Resume the animation
   */
  void resumeAnimation();

  /**
   * @brief Skip animation (instant rendering)
   */
  void skipAnimation();

  // ========================================================================
  // CONFIGURATION METHODS
  // ========================================================================

  /**
   * @brief Enable or disable color output
   * @param enabled True to enable colors, false for monochrome
   */
  void setColorMode(bool enabled);

  /**
   * @brief Set the character set for rendering
   * @param set The character set to use
   */
  void setCharacterSet(CharSet set);

  /**
   * @brief Show or hide statistics
   * @param show True to show stats, false to hide
   */
  void setShowStats(bool show);

  /**
   * @brief Show or hide legend
   * @param show True to show legend, false to hide
   */
  void setShowLegend(bool show);

  // ========================================================================
  // STATISTICS DISPLAY
  // ========================================================================

  /**
   * @brief Update and display statistics
   * @param nodesExplored Number of nodes explored
   * @param pathLength Length of the path
   * @param timeElapsed Time elapsed in milliseconds
   */
  void displayStats(int nodesExplored, int pathLength, double timeElapsed);

private:
  // ========================================================================
  // CROSS-PLATFORM COLOR SUPPORT
  // ========================================================================

  /**
   * @brief Set console text color
   * @param color The color to set
   */
  void setColor(Color color);

  /**
   * @brief Reset console color to default
   */
  void resetColor();

  // ========================================================================
  // RENDERING HELPERS
  // ========================================================================

  /**
   * @brief Get the character for a cell
   * @param cell The cell to get character for
   * @param maze The maze (for context)
   * @return char The character to display
   */
  char getCellChar(std::shared_ptr<Cell> cell, const Maze &maze) const;

  /**
   * @brief Get the color for a cell
   * @param cell The cell to get color for
   * @param maze The maze (for context)
   * @return Color The color to use
   */
  Color getCellColor(std::shared_ptr<Cell> cell, const Maze &maze) const;

  /**
   * @brief Move cursor to specific position
   * @param x X coordinate
   * @param y Y coordinate
   */
  void moveCursor(int x, int y);

  /**
   * @brief Display the legend
   */
  void displayLegend();

  // ========================================================================
  // FRAME BUFFERING
  // ========================================================================

  /**
   * @brief Buffer the entire frame
   * @param maze The maze to buffer
   */
  void bufferFrame(const Maze &maze);

  /**
   * @brief Flush the buffer to console
   */
  void flushBuffer();

  // ========================================================================
  // MEMBER VARIABLES
  // ========================================================================

  int animationDelay;      // Delay between frames (ms)
  bool colorEnabled;       // Color mode enabled
  bool paused;             // Animation paused
  bool showStats;          // Show statistics
  bool showLegend;         // Show legend
  CharSet charSet;         // Current character set
  std::string frameBuffer; // Frame buffer for optimized rendering
  std::shared_ptr<Cell>
      lastCurrent; // Last current cell (for differential updates)

#ifdef _WIN32
  HANDLE hConsole;        // Windows console handle
  WORD defaultAttributes; // Default console attributes
#endif
};

#endif // RENDERER_H
