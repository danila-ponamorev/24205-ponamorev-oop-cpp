#include "word_frequency_analyzer.h"
#include "text_processor.h"
#include <string>
#include <list>
#include <map>
#include <stdexcept>

// void WordFrequencyAnalyzer::analyze(const std::list<std::string>& lines) {
//     word_count.clear();
//     total_words = 0;

//     size_t lines_size = lines.size();
//     for(std::list<std::string>::const_iterator line = lines.begin(); line != lines.end(); ++line) {
//         std::list<std::string> words = TextProcessor::extract_words(*line);
//         for(std::list<std::string>::const_iterator word = words.begin(); word != words.end(); ++word) {
//             ++word_count[*word];
//             ++total_words;
//         }
//     }
// }

// bool WordFrequencyAnalyzer::compare(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
//         return a.second > b.second;
// }

// std::list<std::pair<std::string, int>> WordFrequencyAnalyzer::get_sorted_words(){
//     std::list<std::pair<std::string, int>> sorted_words(word_count.begin(), word_count.end());

//     sorted_words.sort(compare);

//     return sorted_words;
// }

//     int WordFrequencyAnalyzer::get_total_words() {return total_words;}
//     std::map<std::string, int>&  WordFrequencyAnalyzer::get_word_count() {return word_count;}

WordFrequencyAnalyzerPrototipe::WordFrequencyAnalyzerPrototipe() {
    word_frequency = new std::map<std::string, int>;
    sorted_word_frequency = new std::list<std::pair<std::string, int>>;
    total_words = 0;
}

WordFrequencyAnalyzerPrototipe::~WordFrequencyAnalyzerPrototipe() {
    delete word_frequency, sorted_word_frequency;
}

void WordFrequencyAnalyzerPrototipe::update_frequency(std::list<std::string>& words) {
    if (word_frequency == nullptr) throw std::runtime_error("Word frequency map is not initialized");

    for(std::list<std::string>::const_iterator word = words.begin(); word != words.end(); ++word) {
        ++(*word_frequency)[*word];
        ++total_words;
    }
}

void WordFrequencyAnalyzerPrototipe::sort() {
    if (word_frequency == nullptr) {
        delete sorted_word_frequency;
        throw std::runtime_error("Word frequency map is not initialized");
    }
    if (sorted_word_frequency == nullptr) {
        delete word_frequency;
        throw std::runtime_error("Sorted word frequency map is not initialized");
    }

    sorted_word_frequency->assign(word_frequency->begin(), word_frequency->end());
    sorted_word_frequency->sort([](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {return a.second > b.second;});
}

const int WordFrequencyAnalyzerPrototipe::get_total_words() const {
    return total_words;
}

const std::map<std::string, int>* WordFrequencyAnalyzerPrototipe::get_word_frequency() const {
    if (word_frequency == nullptr) throw std::runtime_error("Word frequency map is not initialized");
    return word_frequency;
}

const std::list<std::pair<std::string, int>>* WordFrequencyAnalyzerPrototipe::get_sorted_word_frequency() const {
    if (sorted_word_frequency == nullptr) throw std::runtime_error("Sorted word frequency map is not initialized");
    return sorted_word_frequency;

}
