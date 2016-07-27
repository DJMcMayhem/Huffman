#pragma once

#include <fstream>
#include <vector>

class bofstream
{
public:
    bofstream();
    ~bofstream();

public:
    void open(const char *filePath);
    void close();
    bool good();
    bool fail();
    bool eof();
    void clear();

    void put(bool);
    void put(std::vector<bool>);

private:
    void flushBufferToFile();

private:
    std::ofstream file;
    unsigned char buffer;
    short bitPosition;
};
