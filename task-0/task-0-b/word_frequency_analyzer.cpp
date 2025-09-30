#include "word_frequency_analyzer.h"
#include "text_processor.h"
#include <string>
#include <list>
#include <map>

void WordFrequencyAnalyzer::analyze(const std::list<std::string>& lines) {
    word_count.clear();
    total_words = 0;

    size_t lines_size = lines.size();
    for(std::list<std::string>::const_iterator line = lines.begin(); line != lines.end(); ++line) {
        std::list<std::string> words = TextProcessor::extract_words(*line);
        for(std::list<std::string>::const_iterator word = words.begin(); word != words.end(); ++word) {
            ++word_count[*word];
            ++total_words;
        }
    }
}

bool WordFrequencyAnalyzer::compare(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    }

std::list<std::pair<std::string, int>> WordFrequencyAnalyzer::get_sorted_words(){
    std::list<std::pair<std::string, int>> sorted_words(word_count.begin(), word_count.end());

    sorted_words.sort(compare);

    return sorted_words;
}

    int WordFrequencyAnalyzer::get_total_words() {return total_words;}
    std::map<std::string, int>&  WordFrequencyAnalyzer::get_word_count() {return word_count;}
