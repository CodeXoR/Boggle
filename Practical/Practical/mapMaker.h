//
//  mapMaker.h
//  Practical
//
//  Created by Michael Maranan on 4/8/18.

#ifndef mapMaker_h
#define mapMaker_h

#include <string>
#include <vector>
#include "config.h"

using namespace std;

// trie Node
struct TrieNode
{
    TrieNode * child[NUM_LETTERS];
    
    // isLeaf is true if the node represents
    // end of a word
    bool leaf;
};

class MapMaker {

    // container for found words in a character set
    vector<string> * foundWords;
    
    // root TrieNode
    TrieNode * root;
    
    // returns new trie node (initialized to NULLs)
    TrieNode * getNode();
    
    /*
        If not present, inserts key into trie
        If the key is prefix of trie node, just
        marks leaf node
     */
    void insert(TrieNode * root, char * key);
    
    /*
        function to check that current location
        (i and j) is in matrix range
     */
    bool isSafe(int i, int j, bool visited[NUM_ROWS][NUM_COLUMNS]);
    
    // A recursive function to print all words present on boggle
    void searchWord(TrieNode * root, char boggle[][NUM_COLUMNS], int i,
                    int j, bool visited[][NUM_COLUMNS], string str);
    
public:
    // constructor
    MapMaker();
    
    // destructor
    ~MapMaker();
    
    /*
        maps list of available words in the dictionary constructable with the supplied letter matrix
        saves new map to a txt file
        @param = boggle (char[][])
            > character letter matrix representing the game board
        @param = mapName (string)
            > string map name passed by reference
     */
    void makeMap(char boggle[NUM_ROWS][NUM_COLUMNS], const string & mapName);
};
#endif /* mapMaker_h */
