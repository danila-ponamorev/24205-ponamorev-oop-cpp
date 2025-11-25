#include "FileReader.h"
#include <stdexcept>

FileReader::FileReader(const std::string& filename) : filename(filename), file(nullptr) {}

FileReader::~FileReader() {
    close();
}

void FileReader::open() {
    if (file) close();
    file = new std::ifstream(filename);
    if (!file->is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
}

void FileReader::close() {
    if (file) {
        file->close();
        delete file;
        file = nullptr;
    }
}

bool FileReader::has_next() {
    return file && !file->eof();
}

std::string FileReader::next() {
    std::string line;
    if (file && std::getline(*file, line)) {
        return line;
    }
    return "";
}

void FileReader::reset() {
    if (file) {
        file->clear();
        file->seekg(0);
    }
}

std::list<std::string> FileReader::read_all() {
    open();
    std::list<std::string> lines;
    while (has_next()) {
        std::string line = next();
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    close();
    return lines;
}