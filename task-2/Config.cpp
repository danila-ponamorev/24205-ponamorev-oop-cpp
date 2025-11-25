#include "Config.h"
#include <string>

Config Config::parse(int argc, char* argv[]) {
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