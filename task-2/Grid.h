#ifndef GRID_H
#define GRID_H
#include <vector>
#include <cstddef>
#include "CellState.h"

class Grid {
public:
    Grid(size_t width = 50, size_t height = 50);
    // Grid();
    void setup(size_t width, size_t height);
    CellState getCell(int x, int y) const;
    void setCell(int x, int y, CellState state);
    size_t getWidth() const;
    size_t getHeight() const;
    void setWidth(size_t newWidth);
    void setHeight(size_t newHeight); 
    int countAliveNeighbors(int x, int y) const;
    void clear();

private:
    size_t width, height;
    std::vector<std::vector<CellState>> cells;

    void resize(size_t newWidth, size_t newHeight);
};

#endif
