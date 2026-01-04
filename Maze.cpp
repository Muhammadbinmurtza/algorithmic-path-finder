#include "Maze.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>

Maze::Maze(int width, int height)
    : width(width), height(height), start(nullptr), goal(nullptr) {
  if (width > 0 && height > 0) {
    validateDimensions(width, height);
    initializeGrid();
  }
}

void Maze::initializeGrid() {
  grid.clear();
  grid.resize(height);
  for (int y = 0; y < height; ++y) {
    grid[y].resize(width);
    for (int x = 0; x < width; ++x) {
      grid[y][x] = std::make_shared<Cell>(x, y);
    }
  }
}

bool Maze::loadFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
  }

  // Read dimensions
  file >> width >> height;
  file.ignore(); // Ignore newline after dimensions

  if (width <= 0 || height <= 0) {
    std::cerr << "Error: Invalid maze dimensions" << std::endl;
    return false;
  }

  validateDimensions(width, height);

  initializeGrid();

  // Read maze grid
  for (int y = 0; y < height; ++y) {
    std::string line;
    if (!std::getline(file, line)) {
      std::cerr << "Error: Unexpected end of file" << std::endl;
      return false;
    }

    for (int x = 0; x < width && x < static_cast<int>(line.length()); ++x) {
      char c = line[x];

      if (c == '#' || c == '1') {
        grid[y][x]->setWall(true);
      } else if (c == 'S') {
        start = grid[y][x];
      } else if (c == 'G') {
        goal = grid[y][x];
      }
      // '.' or ' ' are open cells (default)
    }
  }

  file.close();

  if (!start || !goal) {
    std::cerr << "Warning: Start or goal position not found in maze file"
              << std::endl;
  }

  return true;
}

void Maze::generate(int w, int h) {
  validateDimensions(w, h);
  width = w;
  height = h;
  initializeGrid();

  // Create border walls
  for (int y = 0; y < height; ++y) {
    grid[y][0]->setWall(true);
    grid[y][width - 1]->setWall(true);
  }
  for (int x = 0; x < width; ++x) {
    grid[0][x]->setWall(true);
    grid[height - 1][x]->setWall(true);
  }

  // Add some internal walls for complexity
  for (int y = 2; y < height - 2; y += 2) {
    for (int x = 2; x < width - 2; x += 3) {
      grid[y][x]->setWall(true);
    }
  }

  // Set default start and goal
  if (width > 2 && height > 2) {
    start = grid[1][1];
    goal = grid[height - 2][width - 2];
  }
}

std::shared_ptr<Cell> Maze::getCell(int x, int y) const {
  if (!isValid(x, y)) {
    throw std::out_of_range("Cell coordinates out of bounds");
  }
  return grid[y][x];
}

std::vector<std::shared_ptr<Cell>>
Maze::getNeighbors(std::shared_ptr<Cell> cell) const {
  std::vector<std::shared_ptr<Cell>> neighbors;
  if (!cell)
    return neighbors;

  int x = cell->getX();
  int y = cell->getY();

  // Check all four cardinal directions
  const int dx[] = {0, 0, -1, 1}; // UP, DOWN, LEFT, RIGHT
  const int dy[] = {-1, 1, 0, 0};

  for (int i = 0; i < 4; ++i) {
    int nx = x + dx[i];
    int ny = y + dy[i];

    if (isValid(nx, ny)) {
      auto neighbor = grid[ny][nx];
      if (!neighbor->isWall()) {
        neighbors.push_back(neighbor);
      }
    }
  }

  return neighbors;
}

bool Maze::isValid(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

void Maze::reset() {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      grid[y][x]->reset();
    }
  }
}

int Maze::getWidth() const { return width; }

int Maze::getHeight() const { return height; }

std::shared_ptr<Cell> Maze::getStart() const { return start; }

std::shared_ptr<Cell> Maze::getGoal() const { return goal; }

void Maze::setStart(int x, int y) {
  if (isValid(x, y)) {
    start = grid[y][x];
  }
}

void Maze::setGoal(int x, int y) {
  if (isValid(x, y)) {
    goal = grid[y][x];
  }
}

void Maze::validateDimensions(int width, int height) const {
  if (width < 5 || height < 5) {
    throw std::invalid_argument("Maze dimensions must be at least 5x5");
  }
  if (width > 100 || height > 100) {
    throw std::invalid_argument("Maze dimensions must not exceed 100x100");
  }
}

void Maze::printMaze() const {
  std::cout << std::endl;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto cell = grid[y][x];

      if (start && *cell == *start) {
        std::cout << 'S';
      } else if (goal && *cell == *goal) {
        std::cout << 'G';
      } else if (cell->isWall()) {
        std::cout << '#';
      } else if (cell->isPath()) {
        std::cout << '*';
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

// ============================================================================
// MAZE GENERATION ALGORITHMS
// ============================================================================

void Maze::generatePerfectMaze(int w, int h,
                               MazeGenerationAlgorithm algorithm) {
  validateDimensions(w, h);
  width = w;
  height = h;

  // Initialize random number generator with time-based seed
  std::random_device rd;
  rng.seed(rd());

  initializeGrid();

  // Set all cells as walls initially (except for recursive division)
  if (algorithm != MazeGenerationAlgorithm::RECURSIVE_DIVISION) {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        grid[y][x]->setWall(true);
      }
    }
  }

  // Call appropriate algorithm
  switch (algorithm) {
  case MazeGenerationAlgorithm::RECURSIVE_BACKTRACKING:
    generateRecursiveBacktracking();
    break;
  case MazeGenerationAlgorithm::RANDOMIZED_PRIMS:
    generateRandomizedPrims();
    break;
  case MazeGenerationAlgorithm::RECURSIVE_DIVISION:
    generateRecursiveDivision();
    break;
  }

  // Set start and goal to valid passage cells (odd coordinates)
  // This ensures they're on cells that were carved during generation
  start = grid[1][1];
  goal = grid[height - 2][width - 2];

  // Make absolutely sure start and goal are not walls
  start->setWall(false);
  goal->setWall(false);
}

// Helper: Get unvisited neighbors for maze generation
std::vector<std::shared_ptr<Cell>>
Maze::getUnvisitedNeighbors(std::shared_ptr<Cell> cell) const {
  std::vector<std::shared_ptr<Cell>> neighbors;
  if (!cell)
    return neighbors;

  int x = cell->getX();
  int y = cell->getY();

  // Check all four cardinal directions
  const int dx[] = {0, 0, -1, 1};
  const int dy[] = {-1, 1, 0, 0};

  for (int i = 0; i < 4; ++i) {
    int nx = x + dx[i];
    int ny = y + dy[i];

    if (isValid(nx, ny)) {
      auto neighbor = grid[ny][nx];
      if (!neighbor->isVisited()) {
        neighbors.push_back(neighbor);
      }
    }
  }

  return neighbors;
}

// Helper: Remove wall between two adjacent cells
void Maze::removeWallBetween(std::shared_ptr<Cell> cell1,
                             std::shared_ptr<Cell> cell2) {
  if (!cell1 || !cell2)
    return;

  cell1->setWall(false);
  cell2->setWall(false);
}

// Helper: Add wall between two adjacent cells
void Maze::addWallBetween(std::shared_ptr<Cell> cell1,
                          std::shared_ptr<Cell> cell2) {
  if (!cell1 || !cell2)
    return;

  cell1->setWall(true);
  cell2->setWall(true);
}

// Algorithm 1: Recursive Backtracking (DFS-based)
void Maze::generateRecursiveBacktracking() {
  // Start from random odd position (to ensure proper maze structure)
  std::uniform_int_distribution<> distX(0, (width - 3) / 2);
  std::uniform_int_distribution<> distY(0, (height - 3) / 2);

  int startX = 1 + distX(rng) * 2;
  int startY = 1 + distY(rng) * 2;

  auto current = grid[startY][startX];
  current->setWall(false);
  current->setVisited(true);

  // Stack for backtracking
  std::vector<std::shared_ptr<Cell>> stack;
  stack.push_back(current);

  while (!stack.empty()) {
    current = stack.back();

    // Get unvisited neighbors (2 cells away in cardinal directions)
    std::vector<std::shared_ptr<Cell>> neighbors;
    int x = current->getX();
    int y = current->getY();

    // Check all four directions (2 cells away)
    const int dx[] = {0, 0, -2, 2};
    const int dy[] = {-2, 2, 0, 0};

    for (int i = 0; i < 4; ++i) {
      int nx = x + dx[i];
      int ny = y + dy[i];

      if (isValid(nx, ny) && !grid[ny][nx]->isVisited()) {
        neighbors.push_back(grid[ny][nx]);
      }
    }

    if (!neighbors.empty()) {
      // Choose random neighbor
      std::uniform_int_distribution<> dist(0, neighbors.size() - 1);
      auto chosen = neighbors[dist(rng)];

      // Remove wall between current and chosen
      chosen->setWall(false);
      chosen->setVisited(true);

      // Remove wall between them (the cell in between)
      int mx = (current->getX() + chosen->getX()) / 2;
      int my = (current->getY() + chosen->getY()) / 2;
      grid[my][mx]->setWall(false);

      stack.push_back(chosen);
    } else {
      stack.pop_back();
    }
  }

  // Reset visited flags
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      grid[y][x]->setVisited(false);
    }
  }
}

// Algorithm 2: Randomized Prim's Algorithm
void Maze::generateRandomizedPrims() {
  // Start from random cell
  std::uniform_int_distribution<> distX(1, width - 2);
  std::uniform_int_distribution<> distY(1, height - 2);

  auto start = grid[distY(rng)][distX(rng)];
  start->setWall(false);
  start->setVisited(true);

  // Frontier list (walls adjacent to maze)
  std::vector<std::shared_ptr<Cell>> frontier;

  // Add neighbors of start to frontier
  auto neighbors = getNeighbors(start);
  for (auto &n : neighbors) {
    if (n->isWall()) {
      frontier.push_back(n);
      n->setVisited(true);
    }
  }

  while (!frontier.empty()) {
    // Pick random wall from frontier
    std::uniform_int_distribution<> dist(0, frontier.size() - 1);
    int idx = dist(rng);
    auto wall = frontier[idx];
    frontier.erase(frontier.begin() + idx);

    // Count adjacent cells that are passages
    int x = wall->getX();
    int y = wall->getY();
    int passageCount = 0;

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; ++i) {
      int nx = x + dx[i];
      int ny = y + dy[i];
      if (isValid(nx, ny) && !grid[ny][nx]->isWall()) {
        passageCount++;
      }
    }

    // If exactly one adjacent cell is a passage, make this a passage
    if (passageCount == 1) {
      wall->setWall(false);

      // Add unvisited neighbors to frontier
      for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) {
          auto neighbor = grid[ny][nx];
          if (neighbor->isWall() && !neighbor->isVisited()) {
            frontier.push_back(neighbor);
            neighbor->setVisited(true);
          }
        }
      }
    }
  }

  // Reset visited flags
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      grid[y][x]->setVisited(false);
    }
  }
}

// Algorithm 3: Recursive Division
void Maze::generateRecursiveDivision() {
  // Start with empty grid
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      grid[y][x]->setWall(false);
    }
  }

  // Add border walls
  for (int y = 0; y < height; ++y) {
    grid[y][0]->setWall(true);
    grid[y][width - 1]->setWall(true);
  }
  for (int x = 0; x < width; ++x) {
    grid[0][x]->setWall(true);
    grid[height - 1][x]->setWall(true);
  }

  // Recursive division helper
  std::function<void(int, int, int, int)> divide = [&](int x, int y, int w,
                                                       int h) {
    if (w < 2 || h < 2)
      return;

    // Choose orientation
    bool horizontal = (h > w) || (h == w && (rng() % 2 == 0));

    if (horizontal) {
      // Draw horizontal wall
      std::uniform_int_distribution<> wallDist(y, y + h - 1);
      std::uniform_int_distribution<> gapDist(x, x + w - 1);

      int wallY = wallDist(rng);
      int gapX = gapDist(rng);

      for (int i = x; i < x + w; ++i) {
        if (i != gapX && isValid(i, wallY)) {
          grid[wallY][i]->setWall(true);
        }
      }

      // Recursively divide sub-regions
      divide(x, y, w, wallY - y);
      divide(x, wallY + 1, w, y + h - wallY - 1);
    } else {
      // Draw vertical wall
      std::uniform_int_distribution<> wallDist(x, x + w - 1);
      std::uniform_int_distribution<> gapDist(y, y + h - 1);

      int wallX = wallDist(rng);
      int gapY = gapDist(rng);

      for (int i = y; i < y + h; ++i) {
        if (i != gapY && isValid(wallX, i)) {
          grid[i][wallX]->setWall(true);
        }
      }

      // Recursively divide sub-regions
      divide(x, y, wallX - x, h);
      divide(wallX + 1, y, x + w - wallX - 1, h);
    }
  };

  // Start recursive division
  divide(1, 1, width - 2, height - 2);
}
