/*
 * FSTreeTraversal.cpp
 * Hannah Shader & Aidan Banerjee
 * 12/1/2022
 *
 * CS 15 Gerp Project
 * 
 *  The file contains a recursive function which iterates through a FSTree, 
 *  finds each file, and prints the path to that file. Importantly, the order 
 *  of the paths printed is based on a pre-order traversal. The file is not 
 *  used in project gerp, but its implementation is used to print out specific
 *  file paths to show where different words are in the provided directories.
 *  Look to the compiling/running of the README section to see how to run the 
 *  ./TreeTraversal executable.
 * 
 */

#include "FSTree.h"
#include "DirNode.h"
#include <string>
#include <iostream>

void fsTreeTraversal(std::string root_name);
void code_helper(DirNode *root, std::string string);


using namespace std;

// MAIN USED TO RUN FSTREETRAVERSAL. UNCOMMENT TO RUN
// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         cerr << "Usage: ./treeTraversal [directory]" << endl;
//         return 1; 
//     }
//     string dirName = argv[1];
//     fsTreeTraversal(dirName);
// }

/*
*  Name: fsTreeTraversal
*  Purpose: Make an FSTree and print out every path in a tree
*  Parameters: a string name of a root directory
*  Return Value: None
*  Other: calls the helper function code_helper
*/
void fsTreeTraversal(std::string root_name) {
    FSTree tree = FSTree(root_name);
    code_helper(tree.getRoot(), root_name + "/");
}


/*
*  Name: code_helper
*  Purpose: Given a root, recursively build and prints string of file paths
*  for every file
*  Parameters: A DirNode root and an starting string
*  Return Value: void
*  Other: none
*/
void code_helper(DirNode *root, std::string string){
    if (root->numFiles() != 0) {
        for (int i = 0; i < root->numFiles(); i++) {
            std::cout << string;
            std::cout << root->getFile(i) << endl;
        }
    }
    if (not root->hasSubDir()) {
        return;
    }
    std::string newString;
    //Recursive call for each file
    for (int j = 0; j < root->numSubDirs(); j++){
        newString = string + root->getSubDir(j)->getName() + "/";
        code_helper(root->getSubDir(j), newString);
    }
}