#include "FileWriter.h"
#include <stdexcept>

FileWriter::FileWriter(const std::string& filename) : filename(filename), file(nullptr) {}

FileWriter::~FileWriter() {
    close();
}

void FileWriter::open() {
    if (file) close();
    file = new std::ofstream(filename);
    if (!file->is_open()) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
}

void FileWriter::close() {
    if (file) {
        file->close();
        delete file;
        file = nullptr;
    }
}

void FileWriter::write_line(const std::string& line) {
    if (file) {
        *file << line << "\n";
    }
}

void FileWriter::write_all(const std::list<std::string>& lines) {
    open();
    for (const auto& line : lines) {
        write_line(line);
    }
    close();
}