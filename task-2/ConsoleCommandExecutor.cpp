#include "ConsoleCommandExecutor.h"
#include <iostream>
#include <string>

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
        tick(n);
        render();
        
    } else if (command == "dump") {
        if (tokens.size() < 2) {
            std::cout << "Usage: dump <filename>\n";
        } else {
            dump(tokens[1]);
            std::cout << "Universe saved to " << tokens[1] << "\n";
        }
    } else {
        std::cout << "Unknown command. Type 'help' for available commands.\n";
    }

    return true;
}

void ConsoleCommandExecutor::tick(int n) {
    universe.tick(n);
}

void ConsoleCommandExecutor::render() {
    ConsoleRenderer::render(universe);
}

void ConsoleCommandExecutor::dump(std::string file) {
     FileWriter fileWriter(file);
            PresetMaker presetMaker;
            fileWriter.open();
            std::list<std::string> preset = presetMaker.make(universe);
            fileWriter.write_all(preset);
            fileWriter.close();
}

void ConsoleCommandExecutor::showHelp() {
    std::cout << "Available commands:\n"
              << "  tick [n] or t [n] - advance n iterations (default 1)\n"
              << "  dump <filename> - save universe to file\n"
              << "  help - show this help\n"
              << "  exit - exit the program\n";
}