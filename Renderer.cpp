#include "Renderer.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

// ANSI color codes for Unix/Linux
#ifndef _WIN32
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_BLACK "\033[30m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN "\033[36m"
#define ANSI_COLOR_WHITE "\033[37m"
#define ANSI_COLOR_GRAY "\033[90m"
#define ANSI_COLOR_BRIGHT_RED "\033[91m"
#define ANSI_COLOR_BRIGHT_GREEN "\033[92m"
#define ANSI_COLOR_BRIGHT_YELLOW "\033[93m"
#define ANSI_COLOR_BRIGHT_BLUE "\033[94m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\033[95m"
#define ANSI_COLOR_BRIGHT_CYAN "\033[96m"
#define ANSI_COLOR_BRIGHT_WHITE "\033[97m"
#endif

// ============================================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================================

Renderer::Renderer()
    : animationDelay(50), colorEnabled(true), paused(false), showStats(true),
      showLegend(true), charSet(CharSet::ASCII), lastCurrent(nullptr) {

#ifdef _WIN32
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  defaultAttributes = csbi.wAttributes;
#endif
}

Renderer::~Renderer() { resetColor(); }

// ============================================================================
// CROSS-PLATFORM COLOR SUPPORT
// ============================================================================

void Renderer::setColor(Color color) {
  if (!colorEnabled)
    return;

#ifdef _WIN32
  // Windows console colors
  WORD colorCode = defaultAttributes;
  switch (color) {
  case Color::BLACK:
    colorCode = 0;
    break;
  case Color::RED:
    colorCode = FOREGROUND_RED;
    break;
  case Color::GREEN:
    colorCode = FOREGROUND_GREEN;
    break;
  case Color::YELLOW:
    colorCode = FOREGROUND_RED | FOREGROUND_GREEN;
    break;
  case Color::BLUE:
    colorCode = FOREGROUND_BLUE;
    break;
  case Color::MAGENTA:
    colorCode = FOREGROUND_RED | FOREGROUND_BLUE;
    break;
  case Color::CYAN:
    colorCode = FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
  case Color::WHITE:
    colorCode = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
  case Color::GRAY:
    colorCode = FOREGROUND_INTENSITY;
    break;
  case Color::BRIGHT_RED:
    colorCode = FOREGROUND_RED | FOREGROUND_INTENSITY;
    break;
  case Color::BRIGHT_GREEN:
    colorCode = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    break;
  case Color::BRIGHT_YELLOW:
    colorCode = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    break;
  case Color::BRIGHT_BLUE:
    colorCode = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    break;
  case Color::BRIGHT_MAGENTA:
    colorCode = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    break;
  case Color::BRIGHT_CYAN:
    colorCode = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    break;
  case Color::BRIGHT_WHITE:
    colorCode = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
                FOREGROUND_INTENSITY;
    break;
  case Color::RESET:
    colorCode = defaultAttributes;
    break;
  }
  SetConsoleTextAttribute(hConsole, colorCode);
#else
  // Unix/Linux ANSI codes
  switch (color) {
  case Color::BLACK:
    std::cout << ANSI_COLOR_BLACK;
    break;
  case Color::RED:
    std::cout << ANSI_COLOR_RED;
    break;
  case Color::GREEN:
    std::cout << ANSI_COLOR_GREEN;
    break;
  case Color::YELLOW:
    std::cout << ANSI_COLOR_YELLOW;
    break;
  case Color::BLUE:
    std::cout << ANSI_COLOR_BLUE;
    break;
  case Color::MAGENTA:
    std::cout << ANSI_COLOR_MAGENTA;
    break;
  case Color::CYAN:
    std::cout << ANSI_COLOR_CYAN;
    break;
  case Color::WHITE:
    std::cout << ANSI_COLOR_WHITE;
    break;
  case Color::GRAY:
    std::cout << ANSI_COLOR_GRAY;
    break;
  case Color::BRIGHT_RED:
    std::cout << ANSI_COLOR_BRIGHT_RED;
    break;
  case Color::BRIGHT_GREEN:
    std::cout << ANSI_COLOR_BRIGHT_GREEN;
    break;
  case Color::BRIGHT_YELLOW:
    std::cout << ANSI_COLOR_BRIGHT_YELLOW;
    break;
  case Color::BRIGHT_BLUE:
    std::cout << ANSI_COLOR_BRIGHT_BLUE;
    break;
  case Color::BRIGHT_MAGENTA:
    std::cout << ANSI_COLOR_BRIGHT_MAGENTA;
    break;
  case Color::BRIGHT_CYAN:
    std::cout << ANSI_COLOR_BRIGHT_CYAN;
    break;
  case Color::BRIGHT_WHITE:
    std::cout << ANSI_COLOR_BRIGHT_WHITE;
    break;
  case Color::RESET:
    std::cout << ANSI_COLOR_RESET;
    break;
  }
#endif
}

void Renderer::resetColor() { setColor(Color::RESET); }

// ============================================================================
// SCREEN MANAGEMENT
// ============================================================================

void Renderer::clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  std::cout << "\033[2J\033[H";
  std::cout.flush();
#endif
}

void Renderer::moveCursor(int x, int y) {
#ifdef _WIN32
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(hConsole, coord);
#else
  std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
#endif
}

// ============================================================================
// CHARACTER AND COLOR HELPERS
// ============================================================================

char Renderer::getCellChar(std::shared_ptr<Cell> cell, const Maze &maze) const {
  // Check if it's start or goal
  if (maze.getStart() && *cell == *maze.getStart())
    return 'S';
  if (maze.getGoal() && *cell == *maze.getGoal())
    return 'G';

  // Check cell state
  if (cell->isWall()) {
    switch (charSet) {
    case CharSet::UNICODE:
      return static_cast<char>(219); // █
    case CharSet::BOX_DRAWING:
      return static_cast<char>(219); // █
    default:
      return '#';
    }
  }

  if (cell->isPath())
    return '*';
  if (cell->isVisited())
    return '.';

  return ' '; // Empty cell
}

Color Renderer::getCellColor(std::shared_ptr<Cell> cell,
                             const Maze &maze) const {
  // Check if it's start or goal
  if (maze.getStart() && *cell == *maze.getStart())
    return Color::BRIGHT_CYAN;
  if (maze.getGoal() && *cell == *maze.getGoal())
    return Color::BRIGHT_MAGENTA;

  // Check cell state
  if (cell->isWall())
    return Color::WHITE;
  if (cell->isPath())
    return Color::BRIGHT_GREEN;
  if (cell->isVisited())
    return Color::BLUE;

  return Color::RESET;
}

// ============================================================================
// CORE RENDERING
// ============================================================================

void Renderer::drawMaze(const Maze &maze) {
  clearScreen();

  for (int y = 0; y < maze.getHeight(); ++y) {
    for (int x = 0; x < maze.getWidth(); ++x) {
      auto cell = maze.getCell(x, y);

      setColor(getCellColor(cell, maze));
      std::cout << getCellChar(cell, maze);
    }
    std::cout << std::endl;
  }

  resetColor();

  if (showLegend) {
    displayLegend();
  }
}

void Renderer::drawPath(const std::vector<std::shared_ptr<Cell>> &path) {
  for (auto &cell : path) {
    cell->setPath(true);
  }
}

// ============================================================================
// ANIMATION
// ============================================================================

void Renderer::animateStep(const Maze &maze, std::shared_ptr<Cell> current) {
  if (paused || animationDelay == 0)
    return;

  // Redraw the maze with current cell highlighted
  clearScreen();

  for (int y = 0; y < maze.getHeight(); ++y) {
    for (int x = 0; x < maze.getWidth(); ++x) {
      auto cell = maze.getCell(x, y);

      // Highlight current cell
      if (current && *cell == *current) {
        setColor(Color::BRIGHT_YELLOW);
        std::cout << '@';
      } else {
        setColor(getCellColor(cell, maze));
        std::cout << getCellChar(cell, maze);
      }
    }
    std::cout << std::endl;
  }

  resetColor();

  if (showLegend) {
    displayLegend();
  }

  // Delay
  std::this_thread::sleep_for(std::chrono::milliseconds(animationDelay));

  lastCurrent = current;
}

void Renderer::setAnimationSpeed(int ms) { animationDelay = ms; }

void Renderer::pauseAnimation() { paused = true; }

void Renderer::resumeAnimation() { paused = false; }

void Renderer::skipAnimation() { animationDelay = 0; }

// ============================================================================
// CONFIGURATION
// ============================================================================

void Renderer::setColorMode(bool enabled) { colorEnabled = enabled; }

void Renderer::setCharacterSet(CharSet set) { charSet = set; }

void Renderer::setShowStats(bool show) { showStats = show; }

void Renderer::setShowLegend(bool show) { showLegend = show; }

// ============================================================================
// STATISTICS AND LEGEND
// ============================================================================

void Renderer::displayStats(int nodesExplored, int pathLength,
                            double timeElapsed) {
  if (!showStats)
    return;

  std::cout << std::endl;
  setColor(Color::BRIGHT_WHITE);
  std::cout << "Statistics:" << std::endl;
  resetColor();

  std::cout << "  Nodes Explored: " << nodesExplored << std::endl;
  std::cout << "  Path Length: " << pathLength << std::endl;
  std::cout << "  Time: " << std::fixed << std::setprecision(2)
            << (timeElapsed / 1000.0) << " sec" << std::endl;
}

void Renderer::displayLegend() {
  std::cout << std::endl;
  setColor(Color::BRIGHT_WHITE);
  std::cout << "Legend: ";
  resetColor();

  setColor(Color::BRIGHT_CYAN);
  std::cout << "S";
  resetColor();
  std::cout << "=Start ";

  setColor(Color::BRIGHT_MAGENTA);
  std::cout << "G";
  resetColor();
  std::cout << "=Goal ";

  setColor(Color::BRIGHT_GREEN);
  std::cout << "*";
  resetColor();
  std::cout << "=Path ";

  setColor(Color::BLUE);
  std::cout << ".";
  resetColor();
  std::cout << "=Visited ";

  setColor(Color::WHITE);
  std::cout << "#";
  resetColor();
  std::cout << "=Wall ";

  setColor(Color::BRIGHT_YELLOW);
  std::cout << "@";
  resetColor();
  std::cout << "=Current";

  std::cout << std::endl;
}

// ============================================================================
// FRAME BUFFERING (For future optimization)
// ============================================================================

void Renderer::bufferFrame(const Maze &maze) {
  std::ostringstream buffer;

  for (int y = 0; y < maze.getHeight(); ++y) {
    for (int x = 0; x < maze.getWidth(); ++x) {
      auto cell = maze.getCell(x, y);
      buffer << getCellChar(cell, maze);
    }
    buffer << '\n';
  }

  frameBuffer = buffer.str();
}

void Renderer::flushBuffer() {
  std::cout << frameBuffer;
  std::cout.flush();
}
