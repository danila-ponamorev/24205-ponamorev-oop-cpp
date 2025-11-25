#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H
#include "Universe.h"
#include <iostream>

class ConsoleRenderer {
public:
    static void render(const Universe& universe);
};

#endif