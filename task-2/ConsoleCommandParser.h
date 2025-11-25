#ifndef CONSOLE_COMMAND_PARSER_H
#define CONSOLE_COMMAND_PARSER_H
#include <string>
#include <vector>

class ConsoleCommandParser {
public:
    static std::vector<std::string> parse(const std::string& input);
};

#endif
