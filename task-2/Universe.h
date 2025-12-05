#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <string>
#include "Grid.h"
#include "Rule.h"

class Universe {
public:
    Universe(const std::string& name, const Rule& rule, size_t width, size_t height);
    Universe();
    void tick(int n = 1);
    void setCell(int x, int y, CellState state);
    CellState getCell(int x, int y) const;
    size_t getWidth() const;
    size_t getHeight() const;
    void setWidth(size_t newWidth);
    void setHeight(size_t newHeight);
    size_t getIteration() const;
    std::string getName() const;
    Rule getRule() const;
    void setName(const std::string& newName);
    void setRule(const Rule& newRule);
    void setup(const std::string& name = "Universe", const Rule& rule = Rule(), size_t width = 50, size_t height = 50);

private:
    std::string name;
    Rule rule;
    Grid grid;
    size_t iteration;
};

#endif
