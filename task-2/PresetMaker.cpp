#include "PresetMaker.h"
#include <string>

std::list<std::string> PresetMaker::make(const Universe& universe) {
    std::list<std::string> lines;
    
    lines.push_back("#Life 1.06");
    lines.push_back("#N " + universe.getName());
    lines.push_back("#R " + universe.getRule().toString());
    lines.push_back("#S " + std::to_string(universe.getWidth()) + " " + std::to_string(universe.getHeight()));
    
    for (size_t y = 0; y < universe.getHeight(); ++y) {
        for (size_t x = 0; x < universe.getWidth(); ++x) {
            if (universe.getCell(x, y) == CellState::ALIVE) {
                std::string s;
                s = std::to_string(x) + " " + std::to_string(y);
                lines.push_back(s);
            }
        }
    }
    
    return lines;
}
