#include "MazeVisualizer.h"
#include <iostream>
#include <set>

void MazeVisualizer::displayMaze(const Maze &maze) {
  std::cout << std::endl;
  for (int y = 0; y < maze.getHeight(); ++y) {
    for (int x = 0; x < maze.getWidth(); ++x) {
      auto cell = maze.getCell(x, y);
      bool isStart = (maze.getStart() && *cell == *maze.getStart());
      bool isGoal = (maze.getGoal() && *cell == *maze.getGoal());

      std::cout << getCellChar(cell, false, isStart, isGoal);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void MazeVisualizer::displayMazeWithPath(const Maze &maze,
                                         const std::vector<Cell> &path) {
  // Create a set of path coordinates for quick lookup
  std::set<std::pair<int, int>> pathSet;
  for (const auto &cell : path) {
    pathSet.insert({cell.getX(), cell.getY()});
  }

  std::cout << std::endl;
  for (int y = 0; y < maze.getHeight(); ++y) {
    for (int x = 0; x < maze.getWidth(); ++x) {
      auto cell = maze.getCell(x, y);
      bool isInPath = pathSet.find({x, y}) != pathSet.end();
      bool isStart = (maze.getStart() && *cell == *maze.getStart());
      bool isGoal = (maze.getGoal() && *cell == *maze.getGoal());

      std::cout << getCellChar(cell, isInPath, isStart, isGoal);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void MazeVisualizer::displayMazeWithVisited(const Maze &maze) {
  std::cout << std::endl;
  for (int y = 0; y < maze.getHeight(); ++y) {
    for (int x = 0; x < maze.getWidth(); ++x) {
      auto cell = maze.getCell(x, y);
      bool isStart = (maze.getStart() && *cell == *maze.getStart());
      bool isGoal = (maze.getGoal() && *cell == *maze.getGoal());

      if (cell->isWall()) {
        std::cout << '#';
      } else if (isStart) {
        std::cout << 'S';
      } else if (isGoal) {
        std::cout << 'G';
      } else if (cell->isVisited()) {
        std::cout << '.';
      } else {
        std::cout << ' ';
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

char MazeVisualizer::getCellChar(std::shared_ptr<Cell> cell, bool isInPath,
                                 bool isStart, bool isGoal) {
  if (cell->isWall()) {
    return '#';
  } else if (isStart) {
    return 'S';
  } else if (isGoal) {
    return 'G';
  } else if (isInPath) {
    return '*';
  } else {
    return ' ';
  }
}
