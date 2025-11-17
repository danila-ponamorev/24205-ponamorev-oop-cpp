#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <memory>
#include <thread>
#include <chrono>
#include <set>
#include <utility>

enum class CellState { DEAD, ALIVE };

class Rule {
public:
    Rule(const std::string& ruleStr = "B3/S23") {
        setRule(ruleStr);
    }

    void setRule(const std::string& ruleStr) {
        birth.clear();
        survival.clear();
        
        size_t b_pos = ruleStr.find('B');
        size_t s_pos = ruleStr.find('S');
        
        if (b_pos == std::string::npos || s_pos == std::string::npos) {
            throw std::invalid_argument("Invalid rule format");
        }

        for (size_t i = b_pos + 1; i < s_pos - 1; ++i) {
            if (isdigit(ruleStr[i])) {
                birth.insert(ruleStr[i] - '0');
            }
        }

        for (size_t i = s_pos + 1; i < ruleStr.size(); ++i) {
            if (isdigit(ruleStr[i])) {
                survival.insert(ruleStr[i] - '0');
            }
        }
    }

    bool shouldBorn(int neighbors) const {
        return birth.find(neighbors) != birth.end();
    }

    bool shouldSurvive(int neighbors) const {
        return survival.find(neighbors) != survival.end();
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "B";
        for (int n : birth) ss << n;
        ss << "/S";
        for (int n : survival) ss << n;
        return ss.str();
    }

private:
    std::set<int> birth;
    std::set<int> survival;
};

class Grid {
public:
    Grid(size_t width = 50, size_t height = 50) 
        : width(width), height(height), cells(height, std::vector<CellState>(width, CellState::DEAD)) {}

    CellState getCell(int x, int y) const {
        x = (x + width) % width;
        y = (y + height) % height;
        return cells[y][x];
    }

    void setCell(int x, int y, CellState state) {
        x = (x + width) % width;
        y = (y + height) % height;
        cells[y][x] = state;
    }

    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }

    int countAliveNeighbors(int x, int y) const {
        int count = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                if (getCell(x + dx, y + dy) == CellState::ALIVE) {
                    count++;
                }
            }
        }
        return count;
    }

    void clear() {
        for (auto& row : cells) {
            std::fill(row.begin(), row.end(), CellState::DEAD);
        }
    }

private:
    size_t width, height;
    std::vector<std::vector<CellState>> cells;
};

class Universe {
public:
    Universe(const std::string& name = "Universe", const Rule& rule = Rule(), size_t width = 50, size_t height = 50)
        : name(name), rule(rule), grid(width, height), iteration(0) {}

    void tick(int n = 1) {
        for (int i = 0; i < n; ++i) {
            Grid newGrid(grid.getWidth(), grid.getHeight());
            
            for (size_t y = 0; y < grid.getHeight(); ++y) {
                for (size_t x = 0; x < grid.getWidth(); ++x) {
                    int neighbors = grid.countAliveNeighbors(x, y);
                    CellState current = grid.getCell(x, y);
                    
                    if (current == CellState::DEAD && rule.shouldBorn(neighbors)) {
                        newGrid.setCell(x, y, CellState::ALIVE);
                    } else if (current == CellState::ALIVE && rule.shouldSurvive(neighbors)) {
                        newGrid.setCell(x, y, CellState::ALIVE);
                    } else {
                        newGrid.setCell(x, y, CellState::DEAD);
                    }
                }
            }
            
            grid = newGrid;
            iteration++;
        }
    }

    void setCell(int x, int y, CellState state) { grid.setCell(x, y, state); }
    CellState getCell(int x, int y) const { return grid.getCell(x, y); }
    size_t getWidth() const { return grid.getWidth(); }
    size_t getHeight() const { return grid.getHeight(); }
    size_t getIteration() const { return iteration; }
    std::string getName() const { return name; }
    Rule getRule() const { return rule; }
    void setName(const std::string& newName) { name = newName; }
    void setRule(const Rule& newRule) { rule = newRule; }

private:
    std::string name;
    Rule rule;
    Grid grid;
    size_t iteration;
};

class FileReader {
public:
    static Universe readFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string line;
        std::string name = "Universe";
        Rule rule("B3/S23");
        std::vector<std::pair<int, int>> aliveCells;

        // Check format
        std::getline(file, line);
        if (line != "#Life 1.06") {
            throw std::runtime_error("Invalid file format");
        }

        // Read name and rule
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            if (line[0] == '#') {
                if (line.substr(0, 3) == "#N ") {
                    name = line.substr(3);
                } else if (line.substr(0, 3) == "#R ") {
                    rule.setRule(line.substr(3));
                }
            } else {
                // Parse coordinates
                std::stringstream ss(line);
                int x, y;
                ss >> x >> y;
                aliveCells.emplace_back(x, y);
            }
        }

        // Find bounds
        int minX = 0, maxX = 0, minY = 0, maxY = 0;
        for (const auto& coord : aliveCells) {
            minX = std::min(minX, coord.first);
            maxX = std::max(maxX, coord.first);
            minY = std::min(minY, coord.second);
            maxY = std::max(maxY, coord.second);
        }

        Universe universe(name, rule, maxX - minX + 10, maxY - minY + 10);
        
        // Place cells
        for (const auto& coord : aliveCells) {
            universe.setCell(coord.first - minX + 1, coord.second - minY + 1, CellState::ALIVE);
        }

        return universe;
    }
};

class FileWriter {
public:
    static void writeToFile(const Universe& universe, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create file: " + filename);
        }

        file << "#Life 1.06\n";
        file << "#N " << universe.getName() << "\n";
        file << "#R " << universe.getRule().toString() << "\n";

        for (size_t y = 0; y < universe.getHeight(); ++y) {
            for (size_t x = 0; x < universe.getWidth(); ++x) {
                if (universe.getCell(x, y) == CellState::ALIVE) {
                    file << x << " " << y << "\n";
                }
            }
        }
    }
};

class ConsoleRenderer {
public:
    static void render(const Universe& universe) {
        std::cout << "\033[2J\033[1;1H"; // Clear screen
        std::cout << "Name: " << universe.getName() << "\n";
        std::cout << "Rule: " << universe.getRule().toString() << "\n";
        std::cout << "Iteration: " << universe.getIteration() << "\n\n";

        for (size_t y = 0; y < universe.getHeight(); ++y) {
            for (size_t x = 0; x < universe.getWidth(); ++x) {
                std::cout << (universe.getCell(x, y) == CellState::ALIVE ? "■ " : "· ");
            }
            std::cout << "\n";
        }
        std::cout << std::flush;
    }
};

class PresetLibrary {
public:
    static Universe loadGlider() {
        Universe universe("Glider", Rule(), 10, 10);
        universe.setCell(1, 0, CellState::ALIVE);
        universe.setCell(2, 1, CellState::ALIVE);
        universe.setCell(0, 2, CellState::ALIVE);
        universe.setCell(1, 2, CellState::ALIVE);
        universe.setCell(2, 2, CellState::ALIVE);
        return universe;
    }

    static Universe loadBlinker() {
        Universe universe("Blinker", Rule(), 5, 5);
        universe.setCell(1, 2, CellState::ALIVE);
        universe.setCell(2, 2, CellState::ALIVE);
        universe.setCell(3, 2, CellState::ALIVE);
        return universe;
    }

    static Universe loadRPentomino() {
        Universe universe("R-pentomino", Rule(), 20, 20);
        universe.setCell(10, 10, CellState::ALIVE);
        universe.setCell(11, 10, CellState::ALIVE);
        universe.setCell(9, 11, CellState::ALIVE);
        universe.setCell(10, 11, CellState::ALIVE);
        universe.setCell(10, 12, CellState::ALIVE);
        return universe;
    }

    static Universe loadGosperGliderGun() {
        Universe universe("Gosper Glider Gun", Rule(), 40, 20);
        int coords[][2] = {{1,5},{1,6},{2,5},{2,6},{11,5},{11,6},{11,7},{12,4},{12,8},{13,3},{13,9},{14,3},{14,9},
                          {15,6},{16,4},{16,8},{17,5},{17,6},{17,7},{18,6},{21,3},{21,4},{21,5},{22,3},{22,4},{22,5},
                          {23,2},{23,6},{25,1},{25,2},{25,6},{25,7},{35,3},{35,4},{36,3},{36,4}};
        for (auto& coord : coords) {
            universe.setCell(coord[0], coord[1], CellState::ALIVE);
        }
        return universe;
    }
};

class ConsoleCommandParser {
public:
    static std::vector<std::string> parse(const std::string& input) {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string token;
        
        while (ss >> token) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
};

class ConsoleCommandExecutor {
private:
    Universe& universe;
    
public:
    ConsoleCommandExecutor(Universe& u) : universe(u) {}

    bool execute(const std::vector<std::string>& tokens) {
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
                FileWriter::writeToFile(universe, tokens[1]);
                std::cout << "Universe saved to " << tokens[1] << "\n";
            }
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
        
        return true;
    }

private:
    void showHelp() {
        std::cout << "Available commands:\n"
                  << "  tick [n] / t [n] - advance n iterations (default 1)\n"
                  << "  dump <filename> - save universe to file\n"
                  << "  help - show this help\n"
                  << "  exit - exit the program\n";
    }
};

enum class RunMode { INTERACTIVE, OFFLINE, DEFAULT };

class Config {
public:
    RunMode mode = RunMode::DEFAULT;
    std::string inputFile;
    std::string outputFile;
    size_t iterations = 0;

    static Config parse(int argc, char* argv[]) {
        Config config;
        
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "-i" || arg == "--iterations") {
                if (i + 1 < argc) {
                    config.iterations = std::stoul(argv[++i]);
                    config.mode = RunMode::OFFLINE;
                }
            } else if (arg.substr(0, 7) == "--iter=") {
                config.iterations = std::stoul(arg.substr(7));
                config.mode = RunMode::OFFLINE;
            } else if (arg == "-o" || arg == "--output") {
                if (i + 1 < argc) {
                    config.outputFile = argv[++i];
                }
            } else if (arg.substr(0, 9) == "--output=") {
                config.outputFile = arg.substr(9);
            } else {
                config.inputFile = arg;
            }
        }
        
        return config;
    }
};

class Application {
private:
    Config config;
    Universe universe;

public:
    Application(int argc, char* argv[]) {
        config = Config::parse(argc, argv);
        
        if (!config.inputFile.empty()) {
            universe = FileReader::readFromFile(config.inputFile);
        } else {
            // Load a preset universe if no file provided
            universe = PresetLibrary::loadGosperGliderGun();
        }
    }

    void run() {
        if (config.mode == RunMode::OFFLINE) {
            runOffline();
        } else {
            runInteractive();
        }
    }

private:
    void runOffline() {
        universe.tick(config.iterations);
        if (!config.outputFile.empty()) {
            FileWriter::writeToFile(universe, config.outputFile);
        } else {
            ConsoleRenderer::render(universe);
        }
    }

    void runInteractive() {
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
};

int main(int argc, char* argv[]) {
    try {
        Application app(argc, argv);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
