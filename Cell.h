#ifndef CELL_H
#define CELL_H

#include "PathStats.h" // For Coordinate struct

/**
 * @brief Represents a single cell in the maze grid.
 * It stores its position, its state (wall, visited, path), and provides
 * methods to manage this state.
 */
class Cell {
public:
    /**
     * @brief Construct a new Cell object.
     * @param x The x-coordinate of the cell.
     * @param y The y-coordinate of the cell.
     */
    Cell(int x = 0, int y = 0);

    // --- Getters ---
    Coordinate getCoords() const;
    int getX() const;
    int getY() const;
    bool isWall() const;
    bool isVisited() const;
    bool isPath() const;

    // --- Setters ---
    void setWall(bool isWall);
    void setVisited(bool isVisited);
    void setPath(bool isPath);

    /**
     * @brief Resets the transient state of the cell (visited and path flags).
     * The wall status and coordinates are considered permanent and are not reset.
     */
    void reset();

    /**
     * @brief Compares two Cell objects based on their coordinates.
     * @param other The cell to compare against.
     * @return true if the cells have the same coordinates.
     */
    bool operator==(const Cell& other) const;

    /**
     * @brief Compares two Cell objects based on their coordinates.
     * @param other The cell to compare against.
     * @return true if the cells have different coordinates.
     */
    bool operator!=(const Cell& other) const;

private:
    Coordinate coords;
    bool wall = false;
    bool visited = false;
    bool path = false;
};

#endif // CELL_H
