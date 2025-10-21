#include "main_wrapper.h"

#include "word_frequency_analyzer.h"
#include "csv_writer.h"
#include "file_reader.h"
#include "text_processor.h"

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <utility>
#include <format> 

int main_implementation(int argc, char* argv[]) {
        std::string input_file = argv[1];
    std::string output_file = argv[2];

    FileReader filereader(input_file);
    CSVWriter csvwriter(output_file);
    TextProcessor textprocessor;
    WordFrequencyAnalyzer analyzer;
    
    std::string line;
    std::list<std::string> words;
    // std::list<std::pair<std::string, int>>* sorted_words;

    filereader.open();
    while(filereader.has_next()) {
        line = filereader.next();
        words = textprocessor.extract_words(line);
        analyzer.update_frequency(words);
    }
    filereader.close();

    analyzer.sort();
    const std::list<std::pair<std::string, int>>* sorted_words = analyzer.get_sorted_word_frequency();
    const int total_words = analyzer.get_total_words();
    
    csvwriter.open();
    csvwriter.write_row("Слово,Частота,Частота (в %)");
    for(std::list<std::pair<std::string, int>>::const_iterator pair = sorted_words->begin(); pair != sorted_words->end(); ++pair) {
        line = pair->first + "," + std::to_string(pair->second) + "," + std::to_string(static_cast<float>(pair->second) / total_words * 100.0);
        csvwriter.write_row(line);
    }
    csvwriter.close();
    std::cout << "finished" << std::endl;
    
    return 0;
}
