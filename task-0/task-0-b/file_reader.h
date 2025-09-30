#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <list>

class FileReader {
public:
    static std::list<std::string> read_lines(const std::string& filename);
};

#endif
