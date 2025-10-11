#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <list>
#include <fstream>

class FileReader {
private:
    std::string filename;
    std::ifstream* file;

public:
    FileReader(const std::string& filename);
    ~FileReader();

    void open();
    void close();
    bool has_next();
    std::string next();

    void reset();
    // std::list<std::string> read_lines(const std::string& filename);
};

#endif
