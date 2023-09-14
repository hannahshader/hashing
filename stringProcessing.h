/*
 * stringProcessing.h
 * Hannah Shader & Aidan Banerjee
 * 12/1/2022
 *
 * CS 15 Gerp Project (week 1)
 * 
 *  This file defines a class that can strip any string of any non-alphanumeric
 *  characters on the front and back. This functionality is used in project 
 *  gerp to strip any words read in from the file path before they are stored
 *  in our ADT, and then when searching for words on the commandline, the
 *  words are also stripped, ensuring that only valid words as defined by 
 *  the spec are stored and found.
 * 
 */

#ifndef STRINGPROCESSING_H
#define STRINGPROCESSING_H

#include <string>
#include <iostream>

std::string stripNonAlphaNum(std::string input);
bool is_valid_char(char a);

#endif