
#include "Application.h"
#include "Application.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Config config = Config::parse(argc, argv);

        Application app;
        app.setup(config);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}