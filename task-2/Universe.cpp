#include "Universe.h"

Universe::Universe(const std::string& name, const Rule& rule, size_t width, size_t height)
    : name(name), rule(rule), grid(width, height), iteration(0) {}

void Universe::tick(int n) {
    for (int i = 0; i < n; ++i) {
        Grid newGrid(grid.getWidth(), grid.getHeight());
        
        for (size_t y = 0; y < grid.getHeight(); ++y) {
            for (size_t x = 0; x < grid.getWidth(); ++x) {
                int neighbors = grid.countAliveNeighbors(x, y);
                CellState current = grid.getCell(x, y);
                
                if (current == CellState::DEAD && rule.shouldBorn(neighbors)) {
                    newGrid.setCell(x, y, CellState::ALIVE);
                } else if (current == CellState::ALIVE && rule.shouldSurvive(neighbors)) {
                    newGrid.setCell(x, y, CellState::ALIVE);
                } else {
                    newGrid.setCell(x, y, CellState::DEAD);
                }
            }
        }
        
        grid = newGrid;
        iteration++;
    }
}

void Universe::setWidth(size_t newWidth) { grid.setWidth(newWidth); }
void Universe::setHeight(size_t newHeight) { grid.setHeight(newHeight); }
void Universe::setCell(int x, int y, CellState state) { grid.setCell(x, y, state); }
CellState Universe::getCell(int x, int y) const { return grid.getCell(x, y); }
size_t Universe::getWidth() const { return grid.getWidth(); }
size_t Universe::getHeight() const { return grid.getHeight(); }
size_t Universe::getIteration() const { return iteration; }
std::string Universe::getName() const { return name; }
Rule Universe::getRule() const { return rule; }
void Universe::setName(const std::string& newName) { name = newName; }
void Universe::setRule(const Rule& newRule) { rule = newRule; }