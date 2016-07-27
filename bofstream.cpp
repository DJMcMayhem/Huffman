#include "bofstream.h"

bofstream::bofstream()
{
    buffer = 0x00;
    bitPosition = 7;
}

bofstream::~bofstream()
{
    file.close();
}

void bofstream::open(const char *filePath)
{
    file.open(filePath, std::ios_base::out | std::ios_base::binary);
}

void bofstream::close()
{
    if (bitPosition != 7)
        flushBufferToFile();
}

bool bofstream::good()
{
    return file.good();
}

bool bofstream::fail()
{
    return file.fail();
}

bool bofstream::eof()
{
    return file.eof();
}

void bofstream::clear()
{
    file.clear();
}

void bofstream::flushBufferToFile()
{
    file.put(buffer);
    buffer = 0x00;
    bitPosition = 7;
}

void bofstream::put(bool bit)
{
    buffer |= (bit << bitPosition);
    bitPosition--;
    if (bitPosition < 0)
        flushBufferToFile();
}

void bofstream::put(std::vector<bool> bitset)
{
    for (auto i = bitset.begin(); i != bitset.end(); i++)
        put(*i);
}
