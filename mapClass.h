/*
 * mapClass.h
 * Hannah Shader & Aidan Banerjee
 * 12/1/2022
 *
 * CS 15 Gerp Project
 * 
 *  This file contains the class, mapClass, which contains most of the 
 *  functions and logic for executing the gerp project. The class populates the
 *  data structure with the words and their pathways taken from files in the
 *  directory tree. It also contains a command loop to execute commands, 
 *  search through the data structure, and return the instances of certain 
 *  words, or switch the output file. This class will pull from 
 *  stringProcessing for some functionality. It also contains the private
 *  member functions which are used to define the ADT.
 * 
 */

#ifndef MAPCLASS_H
#define MAPCLASS_H

#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "FSTree.h"
#include "DirNode.h"
#include "wordNodeStruct.h"
#include "stringProcessing.h"

using namespace std;

struct holdoneword {
    std::string case_insensitive_key;
    vector<wordNode> case_sensitives;
};
struct canholdmany {
    bool filled;
    vector<holdoneword> vector_of_chained;
};

class mapClass
{
    public:
        mapClass();
	    ~mapClass();

        void open_or_die_for_output(ofstream &stream, string &file_name);
        void store_filepath(std::string &root_name);
        void command_loop(ofstream &to_output);

    private:

        double maxLoadFactor = 0.75; 
        static const int INITIAL_TABLE_SIZE = 100000;
        int numberKeys = 0; //stores the number of unqiue case sensitive words
        int numberSlots = INITIAL_TABLE_SIZE; // indices in arrayKeyValues
        canholdmany *main_arr;
        vector<std::string> *line_contents;
        vector<std::string> *line_path;

        void printInsensitive(holdoneword word_variations, ofstream &read_out);
        bool printSensitive(holdoneword word_variations, 
        std::string word_case_sensitive, ofstream &read_out);

        void get_line(ifstream &input, long &filepath_index);
        void open_close(long &filepath_index);
        void insertChaining(std::string &key, wordNode &value);
        int bucketFor(std::string &key);
        int wordMatch(std::string &word, canholdmany &search);
        std::string toLower(std::string &word);
        void rehash();

        void get_word(int &lineNum, int &lineContents_index, 
        long &filePath_index, istream &line);

        void printHelper(int i, holdoneword word_variations, 
        ofstream &read_out);

        void storeFP_helper(DirNode *root, std::string string);
        void rehashing_helper_insert(holdoneword word, canholdmany *array);
};

#endif