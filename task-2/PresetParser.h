#ifndef PRESET_PARSER_H
#define PRESET_PARSER_H

#include <string>
#include <list>
#include <vector>
#include <utility>
#include "Rule.h"

class PresetParser {
private:
    std::string name;
    Rule rule;
    std::vector<std::pair<int, int>> cells;
    size_t width, height;

public:
    void parse(const std::list<std::string>& lines);

    std::string get_name() const { return name; }
    Rule get_rule() const { return rule; }
    std::vector<std::pair<int, int>> get_cells() const { return cells; }
    size_t get_width() const { return width; }
    size_t get_height() const { return height; }
};

#endif