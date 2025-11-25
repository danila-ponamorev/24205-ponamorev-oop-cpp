#include "ConsoleCommandExecutor.h"
#include <iostream>

ConsoleCommandExecutor::ConsoleCommandExecutor(Universe& u) : universe(u) {}

bool ConsoleCommandExecutor::execute(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return true;

    const std::string& command = tokens[0];

    if (command == "exit") {
        return false;
    } else if (command == "help") {
        showHelp();
    } else if (command == "tick" || command == "t") {
        int n = 1;
        if (tokens.size() > 1) n = std::stoi(tokens[1]);
        universe.tick(n);
        ConsoleRenderer::render(universe);
    } else if (command == "dump") {
        if (tokens.size() < 2) {
            std::cout << "Usage: dump <filename>\n";
        } else {
            FileWriter fileWriter(tokens[1]);
            PresetMaker presetMaker;
            fileWriter.open();
            std::list<std::string> preset = presetMaker.make(universe);
            fileWriter.write_all(preset);
            fileWriter.close();
            //FileWriter::writeToFile(universe, tokens[1]);
            std::cout << "Universe saved to " << tokens[1] << "\n";
        }
    } else {
        std::cout << "Unknown command. Type 'help' for available commands.\n";
    }
    
    return true;
}

void ConsoleCommandExecutor::showHelp() {
    std::cout << "Available commands:\n"
              << "  tick [n] or t [n] - advance n iterations (default 1)\n"
              << "  dump <filename> - save universe to file\n"
              << "  help - show this help\n"
              << "  exit - exit the program\n";
}