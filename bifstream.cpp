#include "bifstream.h"
#include <iostream>

bifstream::bifstream()
{
    lastByteRead = 0;
    bitPosition = -1;
}

bifstream::~bifstream()
{
    this->close();
}

void bifstream::open(const char *filePath)
{
    file.open(filePath, std::ios_base::in | std::ios_base::binary);
}

void bifstream::close()
{
    file.close();
}

bool bifstream::good()
{
    return file.good();
}

bool bifstream::eof()
{
    return file.eof();
}

void bifstream::clear()
{
    file.clear();
}

bool bifstream::fail()
{
    return file.fail();
}

bool bifstream::get()
{
    if (bitPosition < 0)
    {
        bool b = file.fail();
        lastByteRead = file.get();
        bitPosition = 7;
    }

    unsigned char shiftedByte = lastByteRead >> bitPosition;
    bitPosition--;
    return (shiftedByte & 0x01) == 1;
}

std::vector<bool> bifstream::get(int bytes)
{
    std::vector<bool> bitset;
    for (int i = 0; i < bytes; i++)
    {
        bitset.push_back(get());
    }

    return bitset;
}

unsigned char bifstream::getByte()
{
    unsigned char c = 0x00;
    for (int i = 0; i < 8; i++)
    {
        if (get())
        {
            c |= (0x80 >> i);
        }
    }

    return c;
}
