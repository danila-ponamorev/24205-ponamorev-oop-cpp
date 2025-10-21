#ifndef WORD_FREQUENCY_ANALYZER_H
#define WORD_FREQUENCY_ANALYZER_H

#include <string>
#include <list>
#include <map>

// class WordFrequencyAnalyzer {
// private:
//     std::map<std::string, int> word_count;
//     int total_words = 0;
    
// public:
//     void analyze(const std::list<std::string>& lines);
//     std::list<std::pair<std::string, int>> get_sorted_words();
//     int get_total_words();
//     std::map<std::string, int>& get_word_count();
    
    
// private:
//     bool compare(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b);
// };

class WordFrequencyAnalyzer {
private:
    std::map<std::string, int>* word_frequency;
    std::list<std::pair<std::string, int>>* sorted_word_frequency;
    int total_words;
    
public:
    WordFrequencyAnalyzer();
    ~WordFrequencyAnalyzer();

    void update_frequency(std::list<std::string>& words);
    void sort();

    const std::map<std::string, int>* get_word_frequency() const;
    const std::list<std::pair<std::string, int>>* get_sorted_word_frequency() const;
    const int get_total_words() const;

    void reset();
};

#endif
