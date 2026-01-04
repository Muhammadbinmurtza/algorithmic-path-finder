# Quick Reference: Adding Callbacks to Remaining Algorithms

## DFSPathfinder.cpp
Add parameter to signature (line 6-8):
```cpp
std::vector<Cell> DFSPathfinder::findPath(
    const Maze &maze,
    std::shared_ptr<Cell> start,
    std::shared_ptr<Cell> goal,
    std::function<void(std::shared_ptr<Cell>)> onVisit) {
```

Add callback in main loop (after line ~35, after stack.pop()):
```cpp
// Call animation callback if provided
if (onVisit) {
    onVisit(current);
}
```

## DijkstraPathfinder.cpp  
Add parameter to signature:
```cpp
std::vector<Cell> DijkstraPathfinder::findPath(
    const Maze &maze,
    std::shared_ptr<Cell> start,
    std::shared_ptr<Cell> goal,
    std::function<void(std::shared_ptr<Cell>)> onVisit) {
```

Add callback after extracting from priority queue:
```cpp
// Call animation callback if provided
if (onVisit) {
    onVisit(currentNode.cell);
}
```

## AStarPathfinder.cpp
Add parameter to signature:
```cpp
std::vector<Cell> AStarPathfinder::findPath(
    const Maze &maze,
    std::shared_ptr<Cell> start,
    std::shared_ptr<Cell> goal,
    std::function<void(std::shared_ptr<Cell>)> onVisit) {
```

Add callback after extracting from priority queue:
```cpp
// Call animation callback if provided
if (onVisit) {
    onVisit(currentNode.cell);
}
```
