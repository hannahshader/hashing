/*
 * FSTreeTraversal.h
 * Hannah Shader & Aidan Banerjee
 * 12/1/2022
 *
 * CS 15 Gerp Project
 * 
 *  A file that declares a class for a tree, made up of file paths included 
 *  from DirNode.h. This class is made up of constructors, destructors, and a 
 *  couple of functions for accessing elements in the tree in different ways, 
 *  including returning the root node of the tree. Overall this class is used 
 *  in gerp to help with printing line paths, based on the directory structure.
 *  mapClass.h
 *
 */

#ifndef TREE_H
#define TREE_H

#include "DirNode.h"

// Class declaration of n-nery tree which is the representation of a
// file system.
class FSTree {

public:
    /*
     * creates an empty FSTree
     */
    FSTree();

    /*
     * creates a file system representation of the folder passed.
     * The folder will be "root", and its contents will form the
     * rest of the tree
     */
    FSTree(std::string rootName);

    /*
     * FSTree Copy constructor
     */
    FSTree(const FSTree &other);

    /*
     * Deletes all alocated memory using a post order delete
     * algorithm
     */
    ~FSTree();

    /*
     * Assignment overload for FSTree.
     */
    FSTree& operator=(const FSTree &other);

    /*
     * returns the root of the tree in order to traverse it.
     * The destructor will still delete all the allocated data.
     * The user should not manually delete the data.
     */
    DirNode *getRoot() const;

    /*
     * returns true if tree is empty
     */
    bool isEmpty();

    /*
     * destroys the tree and deletes all the allocated data.
     */
    void burnTree();

private:
    /*
     * Root of the n-ery tree
     */
    DirNode *root;

    /*
     * Helper Functions / Utilities (you dont need to worry
     * about these)
     */
    bool is_file(const char *path);

    bool is_dir(const char *path);

    std::string baseName(std::string const & path);

    DirNode *buildTree(std::string rootName);

    void burnTree(DirNode *root);

    DirNode *preOrderCopy(DirNode *currNode, DirNode *parentNode){

        DirNode *newNode = new DirNode();

        /*
         * copy data in currNode to newNode
         */
        newNode->setName(currNode->getName());
        newNode->setParent(parentNode);

        for (int i = 0; i < currNode->numFiles(); i++) {
            newNode->addFile(currNode->getFile(i));
        }

        /*
         * recurse and coy rest of tree
         */
        for (int i = 0; i < currNode->numSubDirs(); i++) {
            newNode->addSubDirectory(preOrderCopy(currNode->getSubDir(i), newNode));
        }

        /*
         * return newNode (copy of currNode)
         */
        return newNode;

    }
};

#endif
