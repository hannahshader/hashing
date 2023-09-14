/*
 * mapClass.cpp
 * Hannah Shader & Aidan Banerjee
 * 12/1/2022
 *
 * CS 15 Gerp Project
 * 
 *  The mapClass.cpp file defines all the functions for inserting word 
 *  instances for the gerp project, based on words read in from files from
 *   a specified directory. The main function used in this class is the 
 *  commandloop(for executing the project) along private helper functions
 *   for storing and populating the ADT. The class aims to work on large 
 *  data-sets, but it may take anywhere from 1-10 minutes to index words 
 *  from a large directory. However, accessing words after should be near 
 *  constant time. The class can be used from the command line, based on the
 *  command loop, and case sensitive/insensitive searches may be defined 
 *  using @i, and other functionality can be found in the function headers.
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <ostream>
#include "mapClass.h"
#include "DirNode.h"
#include "FSTree.h"
#include <vector>
#include <sstream>
#include "wordNodeStruct.h"
#include "stringProcessing.h"

using namespace std;

/*
 * name:      mapClass
 * purpose:   Initializes an instance of mapClass
 * arguments: none
 * returns:   none
 * effects: Creates a new map on the heap which is an array of structs 
 * canholdmany
 * These structs will hold a bool saying whether they're filled and a struct. 
 * This struct will have keys and values. Hash values will correspond to 
 * indices of this array of this array.
 * Innitializes every slot on the map to filled.
 * Creates vector storing line contents on the heap.
 * Creates a vecotr storing line numbers on the heap.
 */
mapClass::mapClass() {
    main_arr = new canholdmany[numberSlots];
    line_contents = new vector<std::string>;
    line_path = new vector<std::string>;

    for (int i = 0; i < numberSlots; i++) {
        main_arr[i].filled = false;
    }
}

mapClass::~mapClass() {
    delete [] main_arr;
    delete line_contents;
    delete line_path;
}


/*
*  Name: command_loop
*  Purpose: Runs a command loop that takes input from the command line
*  and executes gerp based on the commands. It will either quit, switch
*  files, or search the ADT for a word with case sensitive or insensitive
*  Parameters: An output stream to read all outputs to
*  Return Value: Void
*  Other: When reading from a file, if it reaches the end of file the 
*  loop will finish and return
*/
void mapClass::command_loop(ofstream &to_output){
    bool endLoop = false;
    bool printed = false;
    std::string command;

    while (endLoop == false) {
        std::cout << "Query? ";
        std::cin >> command;
        if (command == "@i" or command == "@insensitive") {
            std::string in_string;
            std::cin >> in_string; //read in word make alphanum and lowercase
            std::string orig_case = stripNonAlphaNum(in_string);
            in_string = stripNonAlphaNum(in_string);
            in_string = toLower(in_string);
            int index_to_print = bucketFor(in_string); //hash word
            vector<holdoneword> to_search = 
            main_arr[index_to_print].vector_of_chained; //get all word versions
            for (unsigned long i = 0; i < to_search.size(); i++) {
                if (to_search.at(i).case_insensitive_key == in_string) {
                    printInsensitive(to_search.at(i), to_output);
                    printed = true; //print each version of word
                } 
            }
            if (printed == false) {
                to_output << orig_case << " Not Found." << endl;
            }   //catch if not found
        } else if (command == "@f") {
            std::string output_file; // get new file name
            std::cin >> output_file;
            to_output.close(); //close filestream
            open_or_die_for_output(to_output, output_file); //open new file
        } else if (command == "@q" or command == "@quit") {
            to_output.close();
            endLoop = true; // end commandloop
        } else { //print case sensitive vvv
            std::string in_string_lower = toLower(command); 
            in_string_lower = stripNonAlphaNum(in_string_lower);
            command = stripNonAlphaNum(command);
            int index_to_print = bucketFor(in_string_lower);
            vector<holdoneword> to_search = 
            main_arr[index_to_print].vector_of_chained; //instances of word
            printed = false;
            for (unsigned long i = 0; i < to_search.size(); i++) {
                if (to_search.at(i).case_insensitive_key == in_string_lower) {
                    printed =       // print case sensitive word
                    printSensitive(to_search.at(i), command, to_output);
                }
            }
            if (printed == false) {
                to_output << command 
                << " Not Found. Try with @insensitive or @i." << endl;
            }
        } 
        if (std::cin.eof()) {   // quit if reaches end of input file
            std::cout << "Query? ";
            to_output.close();
            endLoop = true;
        }
    }
    std::cout << "Goodbye! Thank you and have a nice day." << endl;
}

/*
*  Name: open_or_die_for_output
*  Purpose: Opens an output file and sees if the function is open. If not
*  it exits with EXIT_FAILURE
*  Parameters: an output stream and a file name string
*  Return Value: void
*  Other: None
*/
void mapClass::open_or_die_for_output(ofstream &stream, string &file_name){
    stream.open(file_name);
    if (not stream.is_open()) {
    exit (EXIT_FAILURE);
    }
}


/*
*  Name: rehashing_helper_insert
*  Purpose: takes an struct of all instances of a word and stores it in a 
*  larger struct that holds a vector of a bunch of the original struct
*  Parameters: A holdoneword struct and a canholdmany struct which holds
*  instances of holdoneword
*  Return Value: None
*  Other: None
*/
void mapClass::rehashing_helper_insert(holdoneword word, canholdmany *array) {
    int indexInsert = bucketFor(word.case_insensitive_key);
    array[indexInsert].filled = true;
    array[indexInsert].vector_of_chained.push_back(word);
}


/*
*  Name: insertChaining
*  Purpose: Inserts a wordnode onto the 3d ADT based on whether there is
*  isn’t a collision and handles collisions with chaining
*  Parameters: Takes a wordnode struct and a key to the hash table
*  Return Value: Void
*  Other: None
*/
void mapClass::insertChaining(std::string &key, wordNode &value) {
    int indexInsert = bucketFor(key);
    if (main_arr[indexInsert].filled == false) {  //Insert w/out chaining
        vector<wordNode> this_case_sensitives;  //get vector of one wordNode
        this_case_sensitives.push_back(value);
        holdoneword to_add_holdoneword; //populate the holdoneword
        to_add_holdoneword.case_insensitive_key = key;
        to_add_holdoneword.case_sensitives = this_case_sensitives;
        main_arr[indexInsert].filled = true; //push wordnode on ADT
        main_arr[indexInsert].vector_of_chained.push_back(to_add_holdoneword);
    } else {
        //Insert with chaining: check to see if its the same value
        int chained_insert_index = wordMatch(key, main_arr[indexInsert]);
        if (chained_insert_index >= 0) {
            //get the vector of chained at the hash value and insert
            main_arr[indexInsert].vector_of_chained.
            at(chained_insert_index).case_sensitives.push_back(value);
        } else if (chained_insert_index == -1) {
            //get vector of wordNode with just one wordNode 
            vector<wordNode> this_case_sensitives;
            this_case_sensitives.push_back(value);
            holdoneword to_add_holdoneword; //populate the holdoneword 
            to_add_holdoneword.case_insensitive_key = key;
            to_add_holdoneword.case_sensitives = this_case_sensitives;
            main_arr[indexInsert].vector_of_chained.
            push_back(to_add_holdoneword);
        }
    }

}


/*
*  Name: rehash
*  Purpose: Takes a struct with the vector of each instance of a word and
*  and pushes it onto a vector in an index of the main array. 
*  Parameters: 
*  Return Value: 
*  Other: 
*/
void mapClass::rehash() {
    int temp = numberSlots;
    numberSlots  *= 2;
    canholdmany *new_array = new canholdmany[numberSlots];
    for (int i = 0; i < numberSlots; i++) {
        new_array[i].filled = false;
    }
    for (int j = 0; j < temp; j++) {
        //loop through old array and each holdoneword in vector_of_chained
        //and rehash
        if (main_arr[j].filled == true) {
            while (main_arr[j].vector_of_chained.size() > 0) {
                rehashing_helper_insert(main_arr[j].
                vector_of_chained.back(), new_array);
                main_arr[j].vector_of_chained.pop_back();
            }
        }
    }
    delete [] main_arr;
    main_arr = new_array;
}


/*
*  Name: open_close
*  Purpose: A function that will open a file based on the file index and 
*  close it after all the lines from the file have been read.
*  Parameters: Takes in the filepath index which can be used to find a
*  file
*  Return Value: Void
*  Other: calls a helper function get_line to read every file line
*/
void mapClass::open_close(long &filepath_index) {
    ifstream input;
    string   line;
    
    input.open(line_path->at(filepath_index));
    if (not input.is_open()) {
        cerr << "unopened file" << endl;
    }

    get_line(input, filepath_index);    // begin storing the lines and words

    input.close();

}


/*
*  Name: get_line
*  Purpose: Reads each line from a file and then stores each word in a 
*  3d vector ADT
*  Parameters: The input stream of a file and the filepath index to be
*  passed into the helper function get_word
*  Return Value: Void
*  Other: calls a helper function get_word, which stores each line’s words
*/
void mapClass::get_line(ifstream &input, long &filepath_index){
    int linenum = 0;
    std::string line;
    while (getline(input, line)) { // get each line and other info for print
        line_contents->push_back(line);
        int index_of_contents = line_contents->size() - 1;
        linenum++;
        stringstream to_pass;
        to_pass.str(line);
        get_word(linenum, index_of_contents, filepath_index, to_pass);
        // reads in every word from the line
    }   

}


/*
*  Name: get_word
*  Purpose: Populates a wordnode struct with values passed in for each
*  word on a line and then inserts it onto the ADT using a helper func.
*  Parameters: A line number, the number of lines, an index for the 
*  file path vector, and an input stream for the line. 
*  Return Value: Void
*  Other: Will check the load factor each time after inserting a word for
*  the hashtable ADT, and if it is too high, the table with expand and
*  Rehash
*/
void mapClass::get_word(int &lineNum1, int &lineContents_index1, 
                        long &filePath_index1, istream &line) {
    std::string word;
    while (line) {
        line >> word;
        word = stripNonAlphaNum(word);
        if (word != "") {
            wordNode curr;
            curr.lineNum = lineNum1;
            curr.lineContents_index = lineContents_index1;
            curr.filePath_index = filePath_index1;
            curr.wordCaseSensitive = word;
            std::string word_lowercase = toLower(word);
            insertChaining(word_lowercase, curr);   //call main insert func
            numberKeys++;
            // Checking size and rehashing if too full
            double loadFactor = (1.0 * numberKeys) / numberSlots;
            if (loadFactor > maxLoadFactor) {
                rehash();
            }
            word = "";
            
        }
    }
}


/*
*  Name: wordMatch
*  Purpose: Takes a string word and a runs through the key index word of 
*  the second smallest vector in the 3d vector ADT and sees if any 
*  instances of the word already exist exist
*  Parameters: A word string and a struct containing a vector of structs 
*  which each contain a word key index
*  Return Value: return the int index of where in the vector the word
*  Instances exist, or returns -1 if no instance exists
*  Other: None
*/
int mapClass::wordMatch(std::string &word, canholdmany &search) {
    for (unsigned long i = 0; i < search.vector_of_chained.size(); i++) {
        if (search.vector_of_chained.at(i).case_insensitive_key == word){
            return i;
        }
    }
    return -1;
}

std::string mapClass::toLower(std::string &word) {
    int length = word.length();
    std::string result = "";
    for (int i = 0; i < length; i++) {
        char curr_char = word[i];
        if (curr_char <= 'Z' && curr_char >= 'A') {
            result.push_back(curr_char - ('Z' - 'z'));
        } else {
            result.push_back(curr_char);
        }
    }
    return result;
}


/*
*  Name: bucketFor
*  Purpose: takes a string key, and hashes the lowercase version, then 
*  mods the hash by the current main array size
*  Parameters: A string which represents a word key for a hash table
*  Return Value: the integer index hash value
*  Other: None
*/
int mapClass::bucketFor(std::string &key){
    key = toLower(key);
    size_t hashValue = std::hash<std::string>{}(key);
    return hashValue % numberSlots;
}


/*
*  Name: printInsensitive
*  Purpose: Prints all stored case insensitive version of a word and their 
*  lines and linepaths and line number to an output file
*  Parameters: A holdoneword struct with all instances of a word and an
*  output stream for printing the results
*  Return Value: Void
*  Other: Uses a helper for the actual print line, called printHelper
*/
void mapClass::printInsensitive(holdoneword word_variations, 
                                ofstream &read_out) {
    printHelper(0, word_variations, read_out);
    for (unsigned long i = 1; i < word_variations.case_sensitives.size(); i++) {
    if ((word_variations.case_sensitives.at(i).lineNum != 
    word_variations.case_sensitives.at(i - 1).lineNum) or 
    (line_path->at(word_variations.case_sensitives.at(i).filePath_index) != 
    line_path->at(word_variations.case_sensitives.at(i - 1).filePath_index))) {
        printHelper(i, word_variations, read_out);
    }
    }
}


/*
*  Name: printHelper
*  Purpose: prints one line to output of a filepath, line number, and
*  line for a specific word instance
*  Parameters: Takes an index of the vector printed, the struct 
*  holdoneword with the vector of all instances of the word, and an
*  output stream for printing
*  Return Value: void
*/
void mapClass::printHelper(int i, holdoneword word_variations, 
                           ofstream &read_out){
    wordNode curr = word_variations.case_sensitives.at(i);
    read_out << line_path->at(curr.filePath_index) << ":";
    read_out << curr.lineNum << ": ";
    read_out << line_contents->at(curr.lineContents_index) << endl;
}


/*
*  Name: printSensitive
*  Purpose: Prints all stored case sensitive version of a word and their 
*  lines and linepaths and line number to an output file
*  Parameters: A holdoneword struct with all instances of a word and an
*  output stream for printing the results
*  Return Value: bool of whether it could find the word
*  Other: Uses a helper for the actual print line, called printHelper
*/
bool mapClass::printSensitive(holdoneword word_variations, 
                              std::string word_case_sensitive, 
                              ofstream &read_out) {
    int prev_index = 0;
    int size = word_variations.case_sensitives.size();
    //sets prev_index to first instance of case sensitive word
    while (word_variations.case_sensitives.at(prev_index).wordCaseSensitive != 
    word_case_sensitive) {
        prev_index++;
        if (prev_index >= size){
            return false;
        }
    }
    printHelper(prev_index, word_variations, read_out);

    for (unsigned long i = prev_index; i < 
    word_variations.case_sensitives.size(); i++) {
        if ((word_variations.case_sensitives.at(i).lineNum != 
        word_variations.case_sensitives.at(prev_index).lineNum) or 
        (line_path->at(word_variations.case_sensitives.at(i).filePath_index) !=
        line_path->at(word_variations.case_sensitives.at(prev_index).
        filePath_index))) {
            if (word_variations.case_sensitives.at(i).wordCaseSensitive == 
            word_case_sensitive) {
                printHelper(i, word_variations, read_out);
                prev_index = i;
            }
        }
    }
    return true;
}


/*
*  Name: store_filepath
*  Purpose: Creates FSTree which stores each directory and its files
*  Parameters: A string for the name of the root directory
*  Return Value: Void
*  Other: Calls a helper function which will create the file paths
*/
void mapClass::store_filepath(std::string &root_name) {
    FSTree tree = FSTree(root_name);
    storeFP_helper(tree.getRoot(), root_name + "/");
}


/*
*  Name: storeFP_helper
*  Purpose: Takes a DirNode root and recursively creates iterates through 
*  the tree and creates file paths for each file and pushes them onto a 
*  vector of strings called line_path
*  Parameters: A root DirNode and a string to track paths
*  Return Value: Void
*  Other: Runs open_close which stores the file runs get_line, a function
*  reads all the lines from the file
*/
void mapClass::storeFP_helper(DirNode *root, std::string string){
    if (root->numFiles() != 0) {
    for (int i = 0; i < root->numFiles(); i++) {
        std::string filePathString = string + root->getFile(i);
        line_path->push_back(filePathString);
        long to_open = line_path->size() - 1;
        open_close(to_open);
    }
    }
    if (not root->hasSubDir()) {
    return;
    }
    std::string newString;
    for (int j = 0; j < root->numSubDirs(); j++){
    newString = string + root->getSubDir(j)->getName() + "/";
    storeFP_helper(root->getSubDir(j), newString);
    }
}