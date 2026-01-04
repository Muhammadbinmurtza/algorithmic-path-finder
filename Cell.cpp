#include "Cell.h"

Cell::Cell(int x, int y) : coords{x, y}, wall(false), visited(false), path(false) {}

Coordinate Cell::getCoords() const {
    return coords;
}

int Cell::getX() const {
    return coords.x;
}

int Cell::getY() const {
    return coords.y;
}

bool Cell::isWall() const {
    return wall;
}

bool Cell::isVisited() const {
    return visited;
}

bool Cell::isPath() const {
    return path;
}

void Cell::setWall(bool isWall) {
    this->wall = isWall;
}

void Cell::setVisited(bool isVisited) {
    this->visited = isVisited;
}

void Cell::setPath(bool isPath) {
    this->path = isPath;
}

void Cell::reset() {
    visited = false;
    path = false;
}

bool Cell::operator==(const Cell& other) const {
    return coords == other.coords;
}

bool Cell::operator!=(const Cell& other) const {
    return !(*this == other);
}
