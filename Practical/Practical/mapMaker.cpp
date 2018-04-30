//
//  mapMaker.cpp
//  Practical
//
//  Created by Michael Maranan on 4/8/18.

#include <cstdio>
#include <fstream>
#include "mapMaker.h"

// convert letters of the alphabet to a 0 based integer index
#define char_int(c) ((int)c - (int)'A')

MapMaker::MapMaker() {
    root = getNode();
    foundWords = new vector<string>();
    
    // put all words in a dictionary of words into the trie
    ifstream dictionaryFile("dictionary.txt", ios::in);
    if(dictionaryFile.is_open()) {
        string line;
        while(!dictionaryFile.eof()) {
            getline(dictionaryFile, line);
            
            // convert string to char *
            char * cstr = new char[line.length() + 1];
            strcpy(cstr, line.c_str());
            
            insert(root, cstr);
        }
        dictionaryFile.close();
    }
}

MapMaker::~MapMaker() {
    delete root;
    delete foundWords;
}

TrieNode * MapMaker::getNode() {
    TrieNode * newNode = new TrieNode;
    newNode->leaf = false;
    for (int i =0 ; i< NUM_LETTERS; i++) {
        newNode->child[i] = NULL;
    }
    return newNode;
}

void MapMaker::insert(TrieNode * root, char * Key) {
    int n = strlen(Key);
    TrieNode * pchild = root;
    
    for (int i = 0; i < n; ++i) {
        int index = char_int(Key[i]);
        
        if (pchild->child[index] == NULL) {
            pchild->child[index] = getNode();
        }
        
        pchild = pchild->child[index];
    }
    
    // make last node as leaf node
    pchild->leaf = true;
}

bool MapMaker::isSafe(int i, int j, bool visited[NUM_ROWS][NUM_COLUMNS]) {
    return (i >= 0 && i < NUM_ROWS && j >= 0 &&
            j < NUM_COLUMNS && !visited[i][j]);
}

void MapMaker::searchWord(TrieNode * root, char boggle[NUM_ROWS][NUM_COLUMNS], int i,
                int j, bool visited[][NUM_COLUMNS], string str) {
    /*
        if word is found store it into a list of found words
        for writing into a map file later
        only saves words with length of 4 and above
     */
    if (root->leaf == true) {
        if(str.length() > 3) {
            foundWords->push_back(str);
        }
    }
    
    // If both i and j in  range and we visited
    // that element of matrix first time
    if (isSafe(i, j, visited)) {
        // make it visited
        visited[i][j] = true;
        
        // traverse all childs of current root
        for (int k =0; k < NUM_LETTERS; k++) {
            if (root->child[k] != NULL) {
                // current character
                char ch = (char)k + (char)'A' ;
                
                // Recursively search reaming character of word
                // in trie for all 8 adjacent cells of boggle[i][j]
                
                // bottom right
                if (isSafe(i + 1,j + 1,visited) && boggle[i + 1][j + 1] == ch) {
                    searchWord(root->child[k], boggle, i + 1, j + 1, visited, str + ch);
                }
                
                // right
                if (isSafe(i, j + 1, visited) && boggle[i][j + 1] == ch) {
                    searchWord(root->child[k], boggle,i, j + 1, visited, str + ch);
                }
                
                // top right
                if (isSafe(i - 1, j + 1, visited) && boggle[i - 1][j + 1] == ch) {
                    searchWord(root->child[k], boggle, i - 1, j + 1, visited, str + ch);
                }
                
                // bottom
                if (isSafe(i + 1, j, visited) && boggle[i + 1][j] == ch) {
                    searchWord(root->child[k], boggle, i + 1, j, visited, str + ch);
                }
                
                // bottom left
                if (isSafe(i + 1, j - 1, visited) && boggle[i + 1][j - 1] == ch) {
                    searchWord(root->child[k], boggle, i + 1, j - 1, visited, str + ch);
                }
                
                // left
                if (isSafe(i, j - 1, visited) && boggle[i][j - 1] == ch) {
                    searchWord(root->child[k], boggle, i, j-1, visited, str+ch);
                }
                
                // top left
                if (isSafe(i - 1,j - 1,visited) && boggle[i - 1][j - 1] == ch) {
                    searchWord(root->child[k], boggle, i - 1, j - 1, visited, str + ch);
                }
                
                // top
                if (isSafe(i - 1, j, visited) && boggle[i - 1][j] == ch) {
                    searchWord(root->child[k], boggle, i - 1, j, visited, str + ch);
                }
            }
        }
        
        // make current element unvisited
        visited[i][j] = false;
    }
}

void MapMaker::makeMap(char boggle[NUM_ROWS][NUM_COLUMNS], const string & mapName) {
    
    foundWords->clear();
    
    // Mark all characters as not visited
    bool visited[NUM_ROWS][NUM_COLUMNS];
    memset(visited, false, sizeof(visited));
    
    TrieNode * pchild = root;
    
    string str = "";
    
    // character array of all supplied letters delimited by a comma ','
    char letterSet[(NUM_ROWS * NUM_COLUMNS) * 2];
    int arrSize = sizeof(letterSet) / sizeof(letterSet[0]);
   
    char * p = &letterSet[0];
    
    // traverse all matrix elements
    for (int i = 0 ; i < NUM_ROWS; i++) {
        for (int j = 0 ; j < NUM_COLUMNS ; j++) {
            // store letter to letterSet for writing into a file later
            *p++ = boggle[i][j];
            if(p != &letterSet[arrSize - 1]) {
                *p++ = ',';
            }
            
            // we start searching for word in dictionary
            // if we found a character which is child
            // of Trie root
            if (pchild->child[char_int(boggle[i][j])] ) {
                str = str + boggle[i][j];
                searchWord(pchild->child[char_int(boggle[i][j])],
                           boggle, i, j, visited, str);
                str = "";
            }
        }
    }
    
    // create new map file
    ofstream newMap;
    newMap.open(mapName, ios::out);
    
    // point p back to beginning of letter set
    p = &letterSet[0];
    
    // write letter set to first line of new map file
    newMap << p << "\n";
    
    // write number of words to complete level at second line of map file
    newMap << (foundWords->size() / 8) + 1 << "\n";
    
    // write all words found to new map file
    for(string s : *foundWords) {
        newMap << s + "\n";
    }
    
    newMap.close();
}
