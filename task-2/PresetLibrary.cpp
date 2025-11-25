#include "PresetLibrary.h"

Universe PresetLibrary::loadGlider() {
    Universe universe("Glider", Rule(), 10, 10);
    universe.setCell(1, 0, CellState::ALIVE);
    universe.setCell(2, 1, CellState::ALIVE);
    universe.setCell(0, 2, CellState::ALIVE);
    universe.setCell(1, 2, CellState::ALIVE);
    universe.setCell(2, 2, CellState::ALIVE);
    return universe;
}

Universe PresetLibrary::loadBlinker() {
    Universe universe("Blinker", Rule(), 5, 5);
    universe.setCell(1, 2, CellState::ALIVE);
    universe.setCell(2, 2, CellState::ALIVE);
    universe.setCell(3, 2, CellState::ALIVE);
    return universe;
}

Universe PresetLibrary::loadRPentomino() {
    Universe universe("R-pentomino", Rule(), 20, 20);
    universe.setCell(10, 10, CellState::ALIVE);
    universe.setCell(11, 10, CellState::ALIVE);
    universe.setCell(9, 11, CellState::ALIVE);
    universe.setCell(10, 11, CellState::ALIVE);
    universe.setCell(10, 12, CellState::ALIVE);
    return universe;
}

Universe PresetLibrary::loadGosperGliderGun() {
    Universe universe("Gosper Glider Gun", Rule(), 40, 20);
    int coords[][2] = {{1,5},{1,6},{2,5},{2,6},{11,5},{11,6},{11,7},{12,4},{12,8},{13,3},{13,9},{14,3},{14,9},
                      {15,6},{16,4},{16,8},{17,5},{17,6},{17,7},{18,6},{21,3},{21,4},{21,5},{22,3},{22,4},{22,5},
                      {23,2},{23,6},{25,1},{25,2},{25,6},{25,7},{35,3},{35,4},{36,3},{36,4}};
    for (auto& coord : coords) {
        universe.setCell(coord[0], coord[1], CellState::ALIVE);
    }
    return universe;
}