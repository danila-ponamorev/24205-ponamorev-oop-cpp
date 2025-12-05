#ifndef CONSOLE_COMMAND_EXECUTOR_H
#define CONSOLE_COMMAND_EXECUTOR_H
#include <vector>
#include <string>
#include "Universe.h"
#include "ConsoleRenderer.h"
#include "PresetMaker.h"
#include "FileWriter.h"

class ConsoleCommandExecutor {
private:
    Universe& universe;
    
public:
    ConsoleCommandExecutor(Universe& u);
    ConsoleCommandExecutor();
    bool execute(const std::vector<std::string>& tokens);
    void tick(int n);
    void render();
    void dump(std::string file);

private:
    void showHelp();
};

#endif
