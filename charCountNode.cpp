#include <iostream>
#include "charCountNode.h"

CharCountNode::CharCountNode(Byte b, int i) : byte(b.getChar(), b.getIsTerminator())
{
    count = i;
    left = nullptr;
    right = nullptr;
}

void CharCountNode::print()
{
    std::cout << "Char:\t" << byte.getPrintable() << "\n";
    std::cout << "Count:\t" << count << "\n";
    std::cout << "Left:\t" << left << "\n";
    std::cout << "Right:\t" << right << "\n";
}

bool operator < (const CharCountNode &lhs, const CharCountNode &rhs)
{
    return (lhs.count < rhs.count);
}

bool operator > (const CharCountNode &lhs, const CharCountNode &rhs)
{
    return (lhs.count > rhs.count);
}

bool isEmptyNode(CharCountNode node)
{
    return (node.count == 0);
}
