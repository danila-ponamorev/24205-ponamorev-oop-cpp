#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <list>

class TextProcessor {
public:
    static bool is_delimiter(char c);
    static char to_lower(char c);
    static std::list<std::string> extract_words(const std::string& text);
};

#endif
