#include "Grid.h"
#include <algorithm>
#include <cstddef>

Grid::Grid(size_t width, size_t height) 
    : width(width), height(height), cells(height, std::vector<CellState>(width, CellState::DEAD)) {}

CellState Grid::getCell(int x, int y) const {
    x = (x + width) % width;
    y = (y + height) % height;
    return cells[y][x];
}

void Grid::setCell(int x, int y, CellState state) {
    x = (x + width) % width;
    y = (y + height) % height;
    cells[y][x] = state;
}

size_t Grid::getWidth() const { return width; }
size_t Grid::getHeight() const { return height; }

void Grid::setWidth(size_t newWidth) {
    resize(newWidth, height);
}

void Grid::setHeight(size_t newHeight) {
    resize(width, newHeight);
}

void Grid::resize(size_t newWidth, size_t newHeight) {
    std::vector<std::vector<CellState>> newCells(newHeight, std::vector<CellState>(newWidth, CellState::DEAD));
    
    for (size_t y = 0; y < std::min(height, newHeight); ++y) {
        for (size_t x = 0; x < std::min(width, newWidth); ++x) {
            newCells[y][x] = cells[y][x];
        }
    }
    
    width = newWidth;
    height = newHeight;
    cells = std::move(newCells);
}

int Grid::countAliveNeighbors(int x, int y) const {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            if (getCell(x + dx, y + dy) == CellState::ALIVE) {
                count++;
            }
        }
    }
    return count;
}

void Grid::clear() {
    for (auto& row : cells) {
        std::fill(row.begin(), row.end(), CellState::DEAD);
    }
}