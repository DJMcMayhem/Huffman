#pragma once

#include <fstream>
#include <vector>

class bifstream
{
public:
    bifstream();
    ~bifstream();

public:
    void open(const char *filePath);
    void close();
    bool good();
    bool eof();
    void clear();
    bool fail();

    bool get();
    std::vector<bool> get(int bytes);
    unsigned char getByte();

private:
    std::ifstream file;
    unsigned char lastByteRead;
    short bitPosition;
};
