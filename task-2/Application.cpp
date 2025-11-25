#include "Application.h"
#include <iostream>

Application::Application(int argc, char* argv[]) {
    config = Config::parse(argc, argv);
    
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

        //Universe universe(name, rule, width, height);
        universe.setName(name);
        universe.setRule(rule);
        universe.setWidth(width);
        universe.setHeight(height);
        for (const auto& cell : cells) {
            universe.setCell(cell.first, cell.second, CellState::ALIVE);
        }


        // universe = FileReader::readFromFile(config.inputFile);
    } else {
        universe = PresetLibrary::loadGosperGliderGun();
    }
}

void Application::run() {
    if (config.mode == RunMode::OFFLINE) {
        runOffline();
    } else {
        runInteractive();
    }
}

void Application::runOffline() {
    universe.tick(config.iterations);
    if (!config.outputFile.empty()) {
        FileWriter fileWriter(config.outputFile);
        PresetMaker presetMaker;
        fileWriter.open();
        std::list<std::string> preset = presetMaker.make(universe);
        fileWriter.write_all(preset);
        fileWriter.close();
        // FileWriter::writeToFile(universe, config.outputFile);

    } else {
        ConsoleRenderer::render(universe);
    }
}

void Application::runInteractive() {
    ConsoleRenderer::render(universe);
    ConsoleCommandExecutor executor(universe);
    
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