#include "word_frequency_analyzer.h"
#include "csv_writer.h"
#include "file_reader.h"
#include <iostream>

int main(int argc, char* argv[]) {
    WordFrequencyAnalyzer analyzer;

    std::string input_file = argv[1];
    std::string output_file = argv[2];

    std::list<std::string> lines = FileReader::read_lines(input_file);

    analyzer.analyze(lines);

    std::list<std::pair<std::string, int>> sorted_words = analyzer.get_sorted_words();

    CSVWriter::write_results(output_file, sorted_words, analyzer.get_total_words());
    
    std::cout << "finished" << std::endl;
    
    return 0;
}
