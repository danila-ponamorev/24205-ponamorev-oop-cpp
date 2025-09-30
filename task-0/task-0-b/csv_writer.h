#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <string>
#include <list>

class CSVWriter {
public:
    static void write_results(const std::string& filename, 
                            const std::list<std::pair<std::string, int>>& sorted_words, 
                            int total_words);
};

#endif
