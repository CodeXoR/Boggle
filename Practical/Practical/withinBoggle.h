//
//  withinBoggle.h
//  Practical
//
//  Created by Michael Maranan on 4/6/18.

#ifndef withinBoggle_h
#define withinBoggle_h

#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>
#include "config.h"

using namespace std;

/*
 WITHIN Boggle
     In the far future, the game Boggle has been abandoned in favor of WITHIN Boggle, which adds additional complexity to this fun pastime.
     Write a program to generate a valid WITHIN Boggle word tray and then solve for the best possible score, displaying the tray and the results in text form.
     The rules for WITHIN Boggle should be considered exactly the same as for regular Boggle, except in the following specific ways:
        • The board is 5x5
        • On even numbered letter traversals (0, 2, 4 etc) you may only move in a cardinal left/right/up/down direction.
          On odd numbered traversals (1, 3, 5 etc) you may only move in a diagonal direction
        • The board adopts a Scrabble-like convention.
          If your word includes letters on the colored squares below,
          the score for that word should be adjusted as follows:
            ◦ Red: ‘Triple Letter’ score (add 3 to the standard Boggle score for the word)
            ◦ Green: ‘Double letter’ score (add 2 to the standard Boggle score for the word)
            ◦ Blue: Double word score (double the score for the word)
            ◦ ‘Letter-centric’ scores should be applied prior to computing ‘word-centric’ scores
 */

/*
 My thoughts on this question:
    Honestly, I haven't played Boggle before so I did a bit of research about the game before tackling this question.
    After playing some games of Boggle I familiarized myself to the game's common rules.
    I found the game to be fun but I thought it was difficult to find words. It was a good game nonetheless.
 
    Overall, I thought this question touches on all aspects of programming.
    Here's a list of what I had to think about while answering this queston:
        - design pattern (how should I design the game systems for scalability, readability, modularity, and maintainability)
        - data structures and algorithms (which DSA should I use for best use case performance)
        - code structure for easy debugging
        - tests and debugging for delivering the best UX
        - use of the standard template library for efficiency
        - UI and UX design
        - level design
        - gameplay
 
    For design pattern, I chose object oriented because it is great for encapsulation and code-reuse.
    It also is a good design for easy scalability and readability.
 
    For DSA, I found the need to cache some variable words that will be avaialable based on level.
    I needed a data structure that has fast lookup of elements based on key.
    For caching data, based on what I needed, I thought unordered_map would be a good fit.
    unordered_map has O(1) runtime search for an element based on key
 
    I also needed a container for available letter characters based on level.
    For this, I went and used a character array since the
    size of the container will always be constant therefore an array is a great fit
 
    Another thing I needed was another associative container for referencing
    data about the current state of the game board.
    I needed to know what's the color of each letter blocks of the board for scoring purposes.
    Since I was already using unordered_map for caching words, I also used an unordered_map for this purpose
    because it is associative, has great search speed, and the elements didn't need to be sorted.
 
    I used a vector container for storing all found words for displaying on the game's scoreboard later.
 
    I also used queue for temporary storage and processing of user inputs. I primarily used this for storing split strings.
 
    The toughest challenge with this question was finding all the valid words in the dictionary that can be found on a specific matrix of letters.
    I chose to separate that functionality into a separate tool which I named map maker because I didn't want it to affect the main game's runtime.
    The map maker is intended for making a txt files that can be loaded to represent a single level of a game.
    I used a Trie data structure for finding available words in the dictionary. I chose Trie because I needed something faster than a Graph in this case.
    The Graph solution was fine for a smaller board but it took so long for a 5x5 matrix of letters that I didn't wait for it to finish.
 
    For code structure and debugging, because I separated my classes into declaration and implementation files, I didn't
    have a lot of trouble tracking down bugs and fixing them. The implementations of members in this class is modular enough
    for easy maintainability.
 
    UI and UX is probably the area where I had to pause and think about the longest.
    Because the game would have to run in a console application, I had to make sure that the game would
    be displayed on the console screen with great formatting so that it'd be easy to read.
    Also, probably the biggest UX decision I had to think about was what should the user input be.
    I went back and forth from supplying letter index sequence vs typing a word.
    After careful consideration, I went ahead and implemented letter index sequence as user input.
    I went this route because, I needed to make sure that the player is submitting a proper word based on a set rule of legal traversal.
    There are some words that could be connected together via different sequences of characters of which other sequences result to an illegal traversal.
    In contrast, if I implemented word as input, I wouldn't be able to detect if the player intentionally found the word through a legal traversal.
 
    As for level design, At first I thought of just implementing a random automatic level creation when starting a new game.
    With this approach, depending on the current combination of letters, it would take up a lot of processing time to find words and cache them in a word map.
    Also I would need a huge data file for dictionary word look up to check for a proper word.
    I thought it'd be a bad idea to it this way, so I went on to implement levels based on a predefined level meta-data.
    With this approach, the processing time of finding words no longer affect the game's runtime performance.
    It would be easier to delegate writing new levels to another member of the team if working in an actual game development environment.
    Also this would give way to adding new level configurations easily for game updates.
 */

// custom data type to represent found words in game
struct Word {
    string name;
    int value;
};

// number of letter blocks on the game board
const int NUM_BLOCKS = NUM_ROWS * NUM_COLUMNS;

// user input delimiter
const char INPUT_DELIMITER = ',';

// game board letter blocks configuration and color coding
const unordered_map<string, char> LETTER_BLOCKS = {
    {"0",'r'},  {"1",'w'},  {"2",'w'},  {"3",'w'},  {"4",'r'},
    {"5",'w'},  {"6",'g'},  {"7",'w'},  {"8",'g'},  {"9",'w'},
    {"10",'w'}, {"11",'w'}, {"12",'b'}, {"13",'w'}, {"14",'w'},
    {"15",'w'}, {"16",'g'}, {"17",'w'}, {"18",'g'}, {"19",'w'},
    {"20",'r'}, {"21",'w'}, {"22",'w'}, {"23",'w'}, {"24",'r'}
};

/*
 Summary:
    This class encapsulates all functionality of the Within Boggle game
 */
class WithinBoggle {
    /*
        character array of current letters available on the game board.
        array size is constant equal to predefined number of blocks
     */
    char letters[NUM_BLOCKS];
    
    // number of levels available
    int numLevels;
    
    // number of words needed to complete the current level
    int numWordsToCompleteLevel = 99;
    
    // current score
    int score;
    
    // current score multiplier for bonuses
    int scoreMultiplier;
    
    // pointer to the current level word map
    unordered_map<string, bool> words;
    
    // pointer to a list of found words in current level
    vector<Word> foundWords;
    
    /*
        calculates scores based on word length according to the regular rules of Boggle
        @param = wordLength (int)
            > length of word submitted by player passed by reference
     */
    void rewardRegularPoints(const int & wordLength);
    
    /*
        calculates scores based on modified rules of Within Boggle
        @param = blockColor (char)
            > block color of selected letter passed by reference
     */
    void rewardBonusPoints(const char & blockColor);
public:
    // constructor
    WithinBoggle();
    // destructor
    ~WithinBoggle();
    
    // creates a random level based on available level maps
    void createRandomGame();
    
    // displays the rules of Within Boggle
    void displayRules();
    
    // displays the game board on the console screen
    void displayWordTray();
    
    // displays the current player score based on found words
    void displayScores();
    
    /*
        processes submitted letter sequence by player
        @param = letterSequence (string)
            > letter sequence if indices submitted by player passed by reference
     */
    int submitWord(const string & letterSequence);
};

#endif /* withinBoggle_h */
