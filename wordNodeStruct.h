#ifndef WORDNODESTRUCT_H
#define WORDNODESTRUCT_H

#include <string>
#include <iostream>

using namespace std;

struct wordNode {
    int lineNum;
    int lineContents_index;
    int filePath_index;
    std::string wordCaseSensitive;
};

#endif
