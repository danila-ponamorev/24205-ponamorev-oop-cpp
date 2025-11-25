#include "ConsoleRenderer.h"
#include <cstdio>

void ConsoleRenderer::render(const Universe& universe) {
    std::cout << "Name: " << universe.getName() << "\n";
    std::cout << "Rule: " << universe.getRule().toString() << "\n";
    std::cout << "Iteration: " << universe.getIteration() << "\n\n";

    for (size_t y = 0; y < universe.getHeight(); ++y) {
        for (size_t x = 0; x < universe.getWidth(); ++x) {
            std::cout << (universe.getCell(x, y) == CellState::ALIVE ? "■ " : "· ");
        }
        std::cout << "\n";
    }
    // std::cout << std::flush;
}
