#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <string>
#include <list>

class CSVWriter {
private:
    std::string filename;
    std::ofstream *file;
    
public:
    CSVWriter(const std::string& filename_out);
    ~CSVWriter();
    
    void open();
    void close();

    void write_row(const std::string& line);
    
    void reset();
};

#endif
