#ifndef PRESET_MAKER_H
#define PRESET_MAKER_H

#include <string>
#include <list>
#include "Universe.h"

class PresetMaker {
public:
    std::list<std::string> make(const Universe& universe);
};

#endif
