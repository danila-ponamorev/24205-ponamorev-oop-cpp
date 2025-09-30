#include "csv_writer.h"
#include <fstream>
#include <string>
#include <list>

void CSVWriter::write_results(const std::string& filename, const std::list<std::pair<std::string, int>>& sorted_words, int total_words) {
    std::ofstream output_file(filename);
    
    output_file << "Слово,Частота,Частота (в %)" << std::endl;

    for(std::list<std::pair<std::string, int>>::const_iterator pair = sorted_words.begin(); pair != sorted_words.end(); ++pair) {
        double percent = ((double)(*pair).second / total_words) * 100;
        output_file << (*pair).first << "," << (*pair).second << "," << percent << std::endl;
    }

    output_file.close();
}

