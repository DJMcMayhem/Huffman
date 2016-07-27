#pragma once
#include "Byte.h"

struct CharCountNode
{
    Byte byte;
    int count;

    CharCountNode *left;
    CharCountNode *right;

    CharCountNode(Byte b, int i);
    void print();
};

bool isEmptyNode(CharCountNode node);

bool operator < (const CharCountNode &lhs, const CharCountNode &rhs);
bool operator > (const CharCountNode &lhs, const CharCountNode &rhs);
