#include "file_reader.h"
#include <fstream>
#include <string>
#include <list>

// std::list<std::string> FileReader::read_lines(const std::string& filename) {
//     std::ifstream input_file(filename);

//     std::list<std::string> lines;
//     std::string line;
//     while(std::getline(input_file, line)) {
//         lines.push_back(line);
//     }

//     input_file.close();

//     return lines;
// }

FileReader::FileReader(const std::string& filename_in) {
    filename = filename_in;
    file = nullptr;
}

FileReader::~FileReader() {
    close();
}

void FileReader::open() {
    close();

    file = new std::ifstream(filename);
    if (!file->is_open()) {
        delete file;
        file = nullptr;
        throw std::runtime_error("Cannot open file: " + filename);
    }
}

void FileReader::close() {
    if (file && file->is_open()) {
        file->close();
    }
}   

bool FileReader::has_next() {
    return file && file->is_open() && file->peek() != EOF;
}

std::string FileReader::next() {
    if(!has_next()) throw std::runtime_error("EOF");

    std::string line;
    
    if (!std::getline(*file, line)) throw std::runtime_error("Connot read from file");

    return line;
}

void FileReader::reset() {
    if (file && file->is_open()) {
        file->clear();
        file->seekg(0);
    }
}
