#include "file_reader.h"
#include <fstream>
#include <string>
#include <list>

std::list<std::string> FileReader::read_lines(const std::string& filename) {
    std::ifstream input_file(filename);

    std::list<std::string> lines;
    std::string line;
    while(std::getline(input_file, line)) {
        lines.push_back(line);
    }

    input_file.close();

    return lines;
}
