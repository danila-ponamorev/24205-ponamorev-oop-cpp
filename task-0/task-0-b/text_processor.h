#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <list>

class TextProcessor {
private:
    bool is_delimiter(char c);
    char to_lower(char c);
public:
    std::list<std::string> extract_words(const std::string& line);
};

#endif
