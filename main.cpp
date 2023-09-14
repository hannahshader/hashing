/*
 * main.cpp
 * Hannah Shader & Aidan Banerjee
 * 12/1/2022
 *
 * CS 15 Gerp Project
 * 
 *  This file is used to run the gerp project using a mapClass object. Will 
 *  run a main and take in arguments from the command line and then execute 
 *  the function to store data from files into the ADT, and then execute a 
 *  function to run the query command loop, and then be sure to close all open 
 *  file streams.
 * 
 */

#include <string>
#include <iostream>
#include <fstream>
#include "stringProcessing.h"
#include "mapClass.h"
#include "FSTree.h"
#include "DirNode.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
        exit (EXIT_FAILURE);
    }
    std::string inputDirectory = argv[1];
    std::string outputFile = argv[2];
    mapClass curr;
    ofstream stream;
    curr.open_or_die_for_output(stream, outputFile);
    curr.store_filepath(inputDirectory);
    curr.command_loop(stream);

    return 0;

}