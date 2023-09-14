/*
 * stringProcessing.cpp
 * Hannah Shader & Aidan Banerjee
 * 12/1/2022
 *
 * CS 15 Gerp Project (week 1)
 * 
 *  This file defines a class that can strip any string of any non-alphanumeric
 *  characters on the front and back. This functionality is used in project 
 *  gerp to strip any words read in from the file path before they are stored 
 *  in our ADT, and then when searching for words on the commandline, the words
 *  are also stripped, ensuring that only valid words as defined by the spec 
 *  are stored and found.
 * 
 */
 
#include <string>
#include <iostream>
#include "stringProcessing.h"

using namespace std;

/*
*  Name: stripNonAlphaNum
*  Purpose: Takes a string and strips the front and back of non-alphanumeric
*  characters
*  Parameters: a string
*  Return Value: the string with stripped characthers
*  Other: None 
*/
std::string stripNonAlphaNum(std::string input) {
    if (input == "") 
    {
        return "";
    }
    //strips the back
    while (not is_valid_char(input.back())) {
        input.pop_back();
        if (input.size() == 0) {
            return "";
        }
    }
    //strips the front
    int i = 0;
    while (not is_valid_char(input[i])) {
        i++;
    }
    input = &input[i];
    return input;
}

/*
*  Name: is_valid_char
*  Purpose: checks whether a character is alphanumeric
*  Parameters: a character
*  Return Value: a boolean of if it is or isnt alphanumeric
*  Other: None
*/
bool is_valid_char(char a) {

    if (a >= 48 and a <= 57) {
        return true;
    }
    if (a >= 65 and a <= 90) {
        return true;
    }
    if (a >= 97 and a <= 122) {
        return true;
    }
    return false;
}

