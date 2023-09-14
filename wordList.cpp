// The list represnts all of the case insentitve instances of a word
// This stores a linked list of all nodes 
// each node represents the case sensitive instance of a word
// class variable will be a pointer to the front of this list 
// (that will have public access)
// Front function gives us a pointer to the front 

#include "wordList.h"
#include <string>
#include <iostream>

using namespace std;

// void WordList::insertWord(int lineNum, int lineContents_index, 
//                 int filePath_index, std::string wordCaseSensitive) {
//     wordNode *newNode = new wordNode;
//     newNode->lineNum = lineNum;
//     newNode->lineContents_index = lineContents_index;
//     newNode->filePath_index = filePath_index;
//     newNode->wordCaseSensitive = wordCaseSensitive;
//     newNode->next = front;
//     front = newNode;
//     list_length++;
// }

// // void WordList::insertWord(wordNode *node) {
// //     wordNode *newNode = new wordNode;
// //     newNode = node;
// //     front = newNode;
// //     list_length++;
// //}

// wordNode *WordList::insertNodeReturnFront(wordNode *node) {
//     wordNode *newNode = new wordNode;
//     newNode = node;
//     newNode->next = front;
//     front = newNode;
//     list_length++;
//     return front;
// }

// WordList::WordList() {
//     wordNode *newNode = new wordNode;
//     front = nullptr;
// }

// WordList::~WordList() {
//     // deleteWordList();
// }

// WordList::WordList(wordNode *node) {
//     wordNode *newNode = new wordNode;
//     newNode = node;
//     newNode->next = front;
//     front = newNode;
//     list_length++;
// }

// // void WordList::deleteWordList()
// // {
// //     recursive_delete(front);
// // }

// // void WordList::recursive_delete(wordNode *node) {
// //     if (node == nullptr) {
// //             return;
// //     }
// //     cout << "recursive delete wordlist" << endl;
// //     recursive_delete(node->next);
// //     free(node);
// // }

// wordNode *WordList::get_front(){
//     return front;
// }