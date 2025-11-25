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
    bool execute(const std::vector<std::string>& tokens);

private:
    void showHelp();
};

#endif
