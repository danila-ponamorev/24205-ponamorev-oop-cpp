#ifndef APPLICATION_H
#define APPLICATION_H

#include "Universe.h"
#include "FileReader.h"
#include "FileReader.h"
#include "PresetParser.h"
#include "CellState.h"
#include "PresetMaker.h"
#include "PresetLibrary.h"
#include "ConsoleRenderer.h"
#include "ConsoleCommandParser.h"
#include "ConsoleCommandExecutor.h"
#include "Config.h"

class Application {
private:
    Config config;
    Universe universe;
    // ConsoleCommandExecutor executor;

public:
    Application();
    Application(Config& config);
    void run();
    void setup(Config& config);

private:
    void runOffline();
    void runInteractive();
};

#endif