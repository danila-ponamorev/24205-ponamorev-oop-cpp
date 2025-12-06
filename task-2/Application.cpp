#include "Application.h"
#include <iostream>

Application::Application(Config& config) {
    setup(config);
}

Application::Application() : universe(), config() {};

void Application::run() {
    if (config.mode == RunMode::OFFLINE) {
        runOffline();
    } else {
        runInteractive();
    }
}

void Application::setup(Config& config) {
    if (!config.inputFile.empty()) {
        FileReader fileReader(config.inputFile);
        PresetParser presetParser;

        fileReader.open();
        std::list<std::string> preset = fileReader.read_all();
        fileReader.close();

        presetParser.parse(preset);

        const std::string name = presetParser.get_name();
        const Rule rule = presetParser.get_rule();
        const std::vector<std::pair<int, int>> cells = presetParser.get_cells();
        const size_t width = presetParser.get_width();
        const size_t height = presetParser.get_height();

        universe.setName(name);
        universe.setRule(rule);
        universe.setWidth(width);
        universe.setHeight(height);
        for (const auto& cell : cells) {
            universe.setCell(cell.first, cell.second, CellState::ALIVE);
        }

    }
    else if(config.presetUniverse == "Glider") {
        universe = PresetLibrary::loadGlider();
    } else if(config.presetUniverse == "Blinker") {
        universe = PresetLibrary::loadBlinker();
    }else if(config.presetUniverse == "RPentomino") {
        universe = PresetLibrary::loadRPentomino();
    } else if(config.presetUniverse == "GosperGliderGun") {
        universe = PresetLibrary::loadGosperGliderGun();
    } else {
        std::cout << "WARNING: Failed to load Universe preset. Name not recognized. Loading deafult Universe preset." << std::endl;
        universe = PresetLibrary::loadGosperGliderGun();
    }
}

void Application::runOffline() {
    ConsoleCommandExecutor executor(universe);

    executor.tick(config.iterations);
    if (!config.outputFile.empty()) {
        executor.dump(config.inputFile); 
    } else {
        executor.render();
    }
}

void Application::runInteractive() {
    ConsoleCommandExecutor executor(universe);
    executor.render();
    
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        auto tokens = ConsoleCommandParser::parse(input);
        if (!executor.execute(tokens)) {
            break;
        }
    }
}