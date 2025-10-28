#include "csv_writer.h"
#include <fstream>
#include <string>
#include <list>
#include <stdexcept>
	
// void CSVWriter::write_results(const std::string& filename, const std::list<std::pair<std::string, int>>& sorted_words, int total_words) {
//     std::ofstream output_file(filename);
    
//     output_file << "Слово,Частота,Частота (в %)" << std::endl;

//     for(std::list<std::pair<std::string, int>>::const_iterator pair = sorted_words.begin(); pair != sorted_words.end(); ++pair) {
//         double percent = ((double)(*pair).second / total_words) * 100;
//         output_file << (*pair).first << "," << (*pair).second << "," << percent << std::endl;
//     }

//     output_file.close();
// }

CSVWriter::CSVWriter(const std::string& filename_out) {
    filename = filename_out;
    file = nullptr;
}

CSVWriter::~CSVWriter() {
    close();
}

void CSVWriter::open() {
    close();

    file = new std::ofstream(filename);
    if (!file->is_open()) {
        delete file;
        file = nullptr;
        throw std::runtime_error("Cannot open file: " + filename);
    }
}

void CSVWriter::close() {
    if (file && file->is_open()) {
        file->close();
    }
}

void CSVWriter::reset() {
    if (file && file->is_open()) {
        file->clear();
        file->seekp(0);
    }
}

void CSVWriter::write_row(const std::list<std::string>& line) {
    if(file && file->is_open()) {
        size_t size = line.size();
        for (std::list<std::string>::const_iterator i = line.begin(); i != line.end(); ++i) {
            if (i != line.end()) {
                *file << *i + ",";
            } else {
                *file << *i;
            }
            
        }
        *file << std::endl;
    }
}
