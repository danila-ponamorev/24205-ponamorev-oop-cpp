#ifndef CONFIG_H
#define CONFIG_H
#include <string>

enum class RunMode { INTERACTIVE, OFFLINE, DEFAULT };

class Config {
public:
    RunMode mode = RunMode::DEFAULT;
    std::string presetUniverse;
    std::string inputFile;
    std::string outputFile;
    size_t iterations = 0;

    static Config parse(int argc, char* argv[]);
};

#endif
