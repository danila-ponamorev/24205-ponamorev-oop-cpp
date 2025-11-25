#ifndef PRESET_LIBRARY_H
#define PRESET_LIBRARY_H
#include "Universe.h"

class PresetLibrary {
public:
    static Universe loadGlider();
    static Universe loadBlinker();
    static Universe loadRPentomino();
    static Universe loadGosperGliderGun();
};

#endif
