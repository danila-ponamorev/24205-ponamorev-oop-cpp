#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <string>
#include <list>
#include <fstream>

class FileWriter {
private:
    std::string filename;
    std::ofstream* file;

public:
    FileWriter(const std::string& filename);
    ~FileWriter();

    void open();
    void close();
    void write_line(const std::string& line);
    
    void write_all(const std::list<std::string>& lines);
};

#endif