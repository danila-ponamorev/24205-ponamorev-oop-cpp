#include "text_processor.h"
#include <string>
#include <list>

bool TextProcessor::is_delimiter(char c) {
    const std::string delimiters = " .,!@#$%^$*&+-=|?;:\t\n()[]{}<>\"'";
    return delimiters.find(c) != std::string::npos;
}

char TextProcessor::to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (c + 32) : c;
}

std::list<std::string> TextProcessor::extract_words(const std::string& text) {
    std::list<std::string> words;
    std::string word;

    for (char c : text) {
        if (is_delimiter(c)) {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += to_lower(c);
        }
    }
    
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}
