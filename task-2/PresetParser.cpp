#include "PresetParser.h"
#include <sstream>

void PresetParser::parse(const std::list<std::string>& lines) {
    name = "Universe";
    rule = Rule("B3/S23");
    size_t width = 50, height = 50;
    cells.clear();
    
    for (const auto& line : lines) {
        if (line.empty()) continue;
        
        if (line[0] == '#') {
            if (line.substr(0, 3) == "#N ") {
                name = line.substr(3);
            } else if (line.substr(0, 3) == "#R ") {
                rule.setRule(line.substr(3));
            } else if (line.substr(0, 3) == "#S ") {
                std::stringstream ss(line.substr(3));
                ss >> width >> height;
            }
        } else {
            std::stringstream ss(line);
            int x, y;
            if (ss >> x >> y) {
                cells.emplace_back(x, y);
            }
        }
    }
    this->width = width;
    this->height = height;
}