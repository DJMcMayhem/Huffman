#include "Byte.h"

Byte::Byte(unsigned char c, bool isTerminator)
{
    this->c = c;
    this->isTerminator = isTerminator;
}

Byte::Byte()
{
}

Byte::~Byte()
{
}

unsigned char Byte::getChar() const
{
    return c;

}

bool Byte::getIsTerminator() const
{
    return isTerminator;
}

bool operator < (const Byte &lhs, const Byte &rhs)
{
    if (lhs.getChar() == rhs.getChar())
        return (lhs.getIsTerminator() > rhs.getIsTerminator()); //So that the terminator (0x00, true) is first

    return (lhs.getChar() < rhs.getChar());
}

bool Byte::operator == (unsigned char c)
{
    return (this->c == c);
}

std::string Byte::getPrintable() const
{
    if (c == '\n')
        return "\\n ";
    if (c == '\t')
        return "\\t ";

    std::string s = "   ";
    s[0] = char(c);
    return s;
}

std::string Byte::getHexPrintable() const
{
    char hexOutput[10];
    if (!isTerminator)
        sprintf_s(hexOutput, " (0x%02x):\t", c);
    else
        sprintf_s(hexOutput, " (Term):\t");

    return std::string(hexOutput);
}
