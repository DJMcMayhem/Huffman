#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include "bifstream.h"
#include "bofstream.h"
#include "Byte.h"
#include "charCountNode.h"

typedef std::vector<bool> Bitset;
typedef std::map<Byte, Bitset> CharMap;

const bool LEFT = true;
const bool RIGHT = false;

CharMap getCharMap(std::vector<unsigned char> &dataString);
std::vector<CharCountNode> getCharFreqs(std::vector<unsigned char> &dataString);
void traverseTree(CharCountNode *currentNode, CharMap &charMap, Bitset &path);
std::vector<unsigned char> getString(std::string inputFilePath, bool &worked);

bool writeKeyToFile(bofstream &outputFile, CharMap &map);
bool writeHumanReadableKeyToFile(std::ofstream &outputFile, CharMap &map);
bool writeMessageToFile(std::vector<unsigned char> &dataString, bofstream &outputFile, CharMap &map);

void printBitset(Bitset bits);
Bitset charToBitset(char c);

CharMap readKeyFromFile(bifstream &input);
std::string readMessageFromFile(bifstream &input, CharMap &charMap);

void writeKeyAndMessage();
void decodeMessage();

int main()
{
    writeKeyAndMessage();
    decodeMessage();
}

void writeKeyAndMessage()
{
    bool worked;
    std::string originalTextPath = "Huffman\\original.txt";
    std::vector<unsigned char> dataString = getString(originalTextPath, worked);

    if (!worked)
    {
        std::cout << "Failed opening the file at '" << originalTextPath.c_str() << "'. Aborting\n";
        return;
    }

    CharMap map = getCharMap(dataString);

    bofstream keyOutput;
    std::string keyOutputPath = "Huffman\\keyOutput.key";
    keyOutput.open(keyOutputPath.c_str());
    if (keyOutput.fail())
    {
        std::cout << "Failed opening the file at '" << keyOutputPath.c_str() << "'. Aborting\n";
        return;
    }

    writeKeyToFile(keyOutput, map);

    bofstream messageOutput;
    std::string messageOutputPath = "Huffman\\messageOutput.huff";
    messageOutput.open(messageOutputPath.c_str());
    if (messageOutput.fail())
    {
        std::cout << "Failed opening the file at '" << messageOutputPath.c_str() << "'. Aborting\n";
        return;
    }

    std::ofstream humanReadableOutput;
    std::string humanReadableOutputPath = "Huffman\\humanReadableKey.txt";
    humanReadableOutput.open(humanReadableOutputPath.c_str());
    if (humanReadableOutput.fail())
    {
        std::cout << "Failed opening the file at '" << humanReadableOutputPath.c_str() << "'. Aborting\n";
        return;
    }

    writeHumanReadableKeyToFile(humanReadableOutput, map);
    writeMessageToFile(dataString, messageOutput, map);

    keyOutput.close();
    messageOutput.close();
}

void decodeMessage()
{
    bifstream keyInput;
    std::string keyInputPath = "Huffman\\keyOutput.key";
    keyInput.open(keyInputPath.c_str());
    if (keyInput.fail())
    {
        std::cout << "Failed opening the file at '" << keyInputPath.c_str() << "'. Aborting\n";
        return;
    }

    CharMap mapFromFile = readKeyFromFile(keyInput);

    bifstream messageInput;
    std::string messageInputPath = "Huffman\\messageOutput.huff";
    messageInput.open(keyInputPath.c_str());
    if (messageInput.fail())
    {
        std::cout << "Failed opening the file at '" << messageInputPath.c_str() << "'. Aborting\n";
        return;
    }

    std::string message = readMessageFromFile(messageInput, mapFromFile);

    std::ofstream outputOriginal;
    std::string outputOriginalPath = "Huffman\\decodedMessage.txt";
    outputOriginal.open(outputOriginalPath.c_str());
    if (outputOriginal.fail())
    {
        std::cout << "Failed opening the file at '" << outputOriginalPath.c_str() << "'. Aborting";
        return;
    }

    outputOriginal << message.c_str();

    std::ofstream humanReadableOutput;
    std::string humanReadableOutputPath = "Huffman\\humanReadableKeyDecoded.txt";
    humanReadableOutput.open(humanReadableOutputPath.c_str());
    if (humanReadableOutput.fail())
    {
        std::cout << "Failed opening the file at '" << humanReadableOutputPath.c_str() << "'. Aborting\n";
        return;
    }

    writeHumanReadableKeyToFile(humanReadableOutput, mapFromFile);

    outputOriginal.close();
    keyInput.close();
    messageInput.close();
}

std::string readMessageFromFile(bifstream &NOTinput, CharMap &charMap)
{
    //Construct a map of <Bitset, Byte> rather than <Byte, Bitset>
    std::map<Bitset, Byte> bitMap;
    for (auto iter = charMap.begin(); iter != charMap.end(); iter++)
        bitMap[iter->second] = iter->first;

    std::string result;

    Bitset currentBits;

    bifstream input;
    input.open("Huffman\\messageOutput.huff");
    if (input.fail())
        return "";

    while (!input.eof())
    {
        currentBits.push_back(input.get());
        if (bitMap.find(currentBits) != bitMap.end())
        {
            if (bitMap[currentBits].getIsTerminator())  
                break;

            result += bitMap[currentBits].getChar();
            currentBits = {};
        }
    }

    return result;
}

std::vector<unsigned char> getString(std::string inputFilePath, bool &worked)
{
    std::vector<unsigned char> string;
    std::ifstream inputFile;
    inputFile.open(inputFilePath.c_str(), std::ios::in | std::ios::binary);
    worked = inputFile.good();

    if (worked)
    {
        while (true)
        {
            unsigned char c = inputFile.get();
            if (inputFile.fail())
                break;
            string.push_back(c);
        }
    }

    inputFile.close();
    return string;
}

CharMap readKeyFromFile(bifstream &input)
{
    CharMap map;

    bool firstByte = true;

    while (true)
    {
        unsigned char c = input.getByte();
        Bitset key;
        Bitset nextBitPair = input.get(2);
        while (nextBitPair[0])
        {
            key.push_back(nextBitPair[1]);

            nextBitPair = input.get(2);
            if (input.fail())
                break;
        }

        map.insert(std::pair<Byte, Bitset>(Byte(c, firstByte), key));
        if (nextBitPair[1] == 1)
            break;

        firstByte = false;
    }
    return map;
}

bool writeKeyToFile(bofstream &outputFile, CharMap &map)
{
    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        unsigned char c = iter->first.getChar();
        Bitset key = iter->second;
        outputFile.put(charToBitset(c));
        for (auto bit: key)
        {
            outputFile.put(true);
            outputFile.put(bit);
        }

        outputFile.put(false);
        if (std::next(iter) == map.end())
            outputFile.put(true);
        else
            outputFile.put(false);
    }

    return true;
}

bool writeHumanReadableKeyToFile(std::ofstream &outputFile, CharMap &map)
{
    auto i = map.find(Byte(0x00, false));
    if (i != map.end())
        int i = 7;

    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        outputFile << iter->first.getPrintable() << iter->first.getHexPrintable();
        for (auto i: iter->second)
            outputFile << int(i);
        outputFile << "\n";
    }

    return true;
}

bool writeMessageToFile(std::vector<unsigned char> &dataString, bofstream &outputFile, CharMap &map)
{
    for (auto c: dataString)
    {
        outputFile.put(map[c]);
    }

    outputFile.put(map.begin()->second);    //The first value in the map is always the terminator
    outputFile.close();

    return true; 
}

CharMap getCharMap(std::vector<unsigned char> &dataString)
{
    CharMap map;
    std::vector<CharCountNode> freqs = getCharFreqs(dataString);

    freqs.push_back(CharCountNode(Byte(0x00, true), 1));        //The terminator

    if (freqs.size() == 1)      //In case the program is given an empty file. Otherwise, we wouuld get an error dereferencing the top of an empty heap.
    {
        map[freqs[0].byte] = std::vector<bool> {true};
        return map;
    }

    std::priority_queue<CharCountNode, std::vector<CharCountNode>, std::greater<CharCountNode>> heap(freqs.begin(), freqs.end());

    while (heap.size() > 2)
    {
        CharCountNode *left = new CharCountNode(heap.top());
        std::ifstream originalInputFile;
        heap.pop();
        CharCountNode *right = new CharCountNode(heap.top());
        heap.pop();

        CharCountNode newNode(0, right->count + left->count);   //The "char" value (0) is irrelevant since we will traverse through this node without looking at the char.
        newNode.left = left;
        newNode.right = right;

        heap.push(newNode);
    }

    CharCountNode *left = new CharCountNode(heap.top());
    heap.pop();
    CharCountNode *right = new CharCountNode(heap.top());
    heap.pop();

    CharCountNode *head = new CharCountNode(0, right->count + left->count);
    head->left = left;
    head->right = right;

    Bitset path = {};
    traverseTree(head, map, path);

    return map;
}

std::vector<CharCountNode> getCharFreqs(std::vector<unsigned char> &dataString)
{
    std::vector<CharCountNode> freqs;
    for (unsigned int i = 0; i < 256; i++)
        freqs.push_back(CharCountNode((unsigned char) i, 0));

    std::string data = "";

    for (auto byte: dataString)
    {
        freqs[byte].count++;
    }

    freqs.erase(std::remove_if(freqs.begin(), freqs.end(), isEmptyNode), freqs.end());
    return freqs;
}

void traverseTree(CharCountNode *currentNode, CharMap &map, Bitset &path)
{
    if (currentNode->left != nullptr && currentNode->right != nullptr)  //Not to the bottom yet, recurse again.
    {
        path.push_back(LEFT);
        traverseTree(currentNode->left, map, path);
        path.pop_back();

        path.push_back(RIGHT);
        traverseTree(currentNode->right, map, path);
        path.pop_back();
    }

    else    //Hit the buttom, add this value to the map
    {
        auto iter = map.begin();
        map.insert(iter, std::pair<Byte, Bitset>(currentNode->byte, path));
    }

    delete currentNode;
}

void printBitset(Bitset bits)
{
    for (auto i : bits)
        std::cout << (i) ? "1" : "0";
}

Bitset charToBitset(char c)
{
    Bitset bits = {};
    for (int i = 7; i >= 0; i--)
    {
        bits.push_back((c >> i) & 0x01);
    }
    return bits;
}
