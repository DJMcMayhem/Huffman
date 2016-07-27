#pragma once

#include <string>

class Byte
{
public:
    Byte(unsigned char c, bool isTerminator = false);
    Byte();
    ~Byte();

    unsigned char getChar() const;
    bool getIsTerminator() const;
    std::string getPrintable() const;
    std::string getHexPrintable() const;

    bool operator == (unsigned char c);

private:
    unsigned char c;
    bool isTerminator;
};

bool operator < (const Byte &lhs, const Byte &rhs);
