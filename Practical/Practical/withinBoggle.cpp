//
//  withinBoggle.cpp
//  Practical
//
//  Created by Michael Maranan on 4/6/18.

#include <queue>
#include <sstream>
#include <fstream>
#include "withinBoggle.h"
#include "consoleUtils.h"

WithinBoggle::WithinBoggle() {
    // initialize random seed for randomizing level maps
    srand(time(NULL));
    
    // set current number of levels
    numLevels = 10;
}

WithinBoggle::~WithinBoggle() {
    // clear the word map before destroying
    words.clear();
}

void WithinBoggle::createRandomGame() {
    
    // reset all game data
    words.clear();
    foundWords.clear();
    
    /*
        create a new random game based of a txt file representing a level
        this function expects a properly formatted level map
        so no input format checking is done
     */
    
    /*
        randomizes level map file by level numbers within the range of (0 - set number of levels)
     */
    string levelMapFile = "level0.txt";
    int levelNumber = rand() % numLevels;
    levelMapFile[5] = levelNumber + '0';
    
    ifstream boggleMap(levelMapFile, ios::in);
    if(boggleMap.is_open()) {
        
        string line;
        
        // get all boggleMap letters from the first line of text
        int letterIndex = 0;
        char * cPtr;
        
        getline(boggleMap, line);
        
        cPtr = &line[0];
        
        /*
            look through each character of the extracted line
            and set current available characters for creating words
            this ignores any character delimiter ','
         */
        while(cPtr != &line[line.length()]) {
            char c = * cPtr;
            if(c != ',') {
                letters[letterIndex] = c;
                letterIndex++;
            }
            cPtr++;
        }
        
        // get level completion requirement at the sixth line of level map file
        getline(boggleMap, line);
        numWordsToCompleteLevel = stoi(line);
        
        // get all available words in the level and cache them into a word map
        while(!boggleMap.eof()) {
            getline(boggleMap, line);
            words.insert({line, false});
        }
        
        boggleMap.close();
    }
    
    displayWordTray();
}

void WithinBoggle::rewardRegularPoints(const int & wordLength) {
    switch (wordLength) {
        case 1:
        case 2:
        case 3:
            score = 0;
        case 4:
            score = 1;
            break;
        case 5:
            score = 2;
            break;
        case 6:
            score = 3;
        case 7:
            score = 4;
        default:
            // 8 or more letters
            score = 11;
            break;
    }
}

void WithinBoggle::rewardBonusPoints(const char & blockColor) {
    switch (blockColor) {
            // Triple letter
        case 'r':
            score += 3;
            break;
            // Double letter
        case 'g':
            score += 2;
            break;
            // Double Score
        case 'b':
            scoreMultiplier = 2;
            break;
        default:
            break;
    }
}

int WithinBoggle::submitWord(const string & letterSequence) {
    
    /*
        used stream to split string input
        and queue to store split parts of the string
     */
    istringstream letterStream(letterSequence);
    queue<std::string> letterStringIndices;
    
    // temp variable to hold split string
    string letterStringIndex;
    
    // split string via delimiter until end is reached
    while(getline(letterStream, letterStringIndex, INPUT_DELIMITER)) {
        letterStringIndices.push(letterStringIndex);
    }
    
    // there is no input supplied
    if(letterStringIndices.empty()) {
        puts("\nInvalid input.\n");
        return 1;
    }
    
    /*
        q or Q can be an alternate input to go back to the main menu
        Note:
            I found a weird issue where sometimes the input has a trailing newline
            thus the added checks for newline characters
     */
    if(letterStringIndices.front() == "q" || letterStringIndices.front() == "Q" ||
       letterStringIndices.front() == "q\n" || letterStringIndices.front() == "Q\n") {
        consoleUtils::clearScreen(10);
        displayRules();
        return 0;
    }
    
    // expected word length based on input
    int expectedWordLength = letterStringIndices.size();
    
    // character array representing the word found with size of expected word length
    char word[expectedWordLength];
    
    // array index of word character array
    int letterIndex = 0;
    
    // apply regular scoring based on word length
    rewardRegularPoints(expectedWordLength);
    
    // score multiplier to be applied later on
    scoreMultiplier = 1;
    
    // variables for checking valid traversal
    int lastBlockIndex = -1;
    /*
        0 = cardinal traversal
        1 = diagonal traversal
        with the current rules there can only be one type of traversal
     */
    int lastTraversal = -1;
    int currentTraversal = -1;
    bool traversalIsLegal = true;
    
    // process each letter string index and find the appropriate character letters via index
    while(!letterStringIndices.empty()) {
        
        // get current letter string index at the front of the queue
        string blockLetterIndexString = letterStringIndices.front();
        
        /*
            sometimes input has trailing newline character
            remove trailing '\n' if any
         */
        if(blockLetterIndexString[blockLetterIndexString.length()-1] == '\n') {
            blockLetterIndexString.pop_back();
        }
        
        // find the corresponding letter block represented by index in the current set map of letter blocks
        auto letterBlocks_it = LETTER_BLOCKS.find(blockLetterIndexString);
        
        // return if letter index input is invalid
        if(letterBlocks_it == LETTER_BLOCKS.end()) {
            puts("\nInvalid input.\n");
            return 1;
        }
        
        // apply bonus points based on color codes
        rewardBonusPoints(letterBlocks_it->second);
        
        // convert the string letter index into an integer
        int blockLetterIndex = stoi(blockLetterIndexString);
        
        // check for legal traversal
        if(traversalIsLegal) {
            if(lastBlockIndex >= 0) {
                if(blockLetterIndex == (lastBlockIndex - 1) || blockLetterIndex == (lastBlockIndex + 1) ||
                   blockLetterIndex == (lastBlockIndex + NUM_COLUMNS) || blockLetterIndex == (lastBlockIndex - NUM_COLUMNS)) {
                    // cardinal traversal
                    currentTraversal = 0;
                }
                else {
                    // diagonal traversal
                    currentTraversal = 1;
                }
            }
            
            if(lastTraversal >= 0 && lastTraversal != currentTraversal) {
                traversalIsLegal = false;
            }
            
            lastBlockIndex = blockLetterIndex;
            lastTraversal = currentTraversal;
        }
        
        if(blockLetterIndex < NUM_BLOCKS) {
            /*
                get the letter character from the current array of letters available
                extracted letter is to be stored in the word character array
             */
            word[letterIndex] = letters[blockLetterIndex];
            letterStringIndices.pop();
            letterIndex++;
        }
        else {
            // supplied letter index invalid or out of range
            puts("\nInvalid input.\n");
            return 1;
        }
    }
    
    // another check for legal traversal
    if(traversalIsLegal) {
        // even traversal therefore only cardinal movements are legal
        if(expectedWordLength % 2 == 0) {
            traversalIsLegal = currentTraversal == 0;
        }
        else { // odd traversal therefore only diagonal movements are legal
            traversalIsLegal = currentTraversal == 1;
        }
    }
    
    // apply any score multiplier if any
    score *= scoreMultiplier;
    
    
    /*
        at this point input is valid, next step is to find word in current word map
        if letter traversal is valid reward player with the appropriate score computed beforehand
     */
    
    /*
        convert word character array into a string
        Note:
            I'm getting stray characters at the end of this converted char[] to string
            added a simple loop to check for strays and remove them at the end of the string
    */
    string sWord = word;
    
    while(sWord.size() != expectedWordLength) {
        sWord.pop_back();
    }
    
    // find current word string in the current word map
    auto words_it = words.find(sWord);
    
    // if the map iterator is not at the end of the map the word is found
    if(words_it != words.end()) {
        // check if the word is valid for scoring
        if(traversalIsLegal) {
            // add new valid word to found words if not already added
            string foundWord = words_it->first;
            
            if(words_it->second == false) {
                Word newWord;
                newWord.name = foundWord;
                newWord.value = score;
                // add new word to found words for displaying to the scoreboard
                foundWords.push_back(newWord);
                displayWordTray();
                displayScores();
            }
            else {
                // found word is already accounted for
                printf("\n%s is already listed as found.\n", foundWord.c_str());
            }
        }
        else {
            // not a valid traversal
            printf("\n%s is a word but supplied sequence is an illegal traversal.\n", words_it->first.c_str());
        }
        words_it->second = true;
    }
    else {
        // word found is either not a word or is too short to be scored
        printf("\n%s is not a word found or considered to be scored.\n", sWord.c_str());
    }
    
    // Level Complete
    if(numWordsToCompleteLevel == foundWords.size()) {
        return 8;
    }
    else {
        // return to main loop
        return 1;
    }
}

void WithinBoggle::displayRules() {
    printf("                                WITHIN BOGGLE                            \n"
           "HOW TO PLAY:                                                           \n\n"
           
            "   You have 3 minutes to find as many words as you can in a 5x5 grid. \n\n"
           
           "The scoring is as follows:                                             \n\n"
           
            "   - 4 Letters            : 1 point                                     \n"
            "   - 5 Letters            : 2 points                                    \n"
            "   - 6 Letters            : 3 points                                    \n"
            "   - 7 Letters            : 4 points                                    \n"
            "   - 8 or More Letters    : 11 points                                 \n\n"
           
           "The board adopts a Scrabble-like convention.                             \n"
           "If your word includes letters on the colored squares below,              \n"
           "the score for that word should be adjusted as follows:                 \n\n"
           
            "   - Red:   'Triple Letter' (standard score + 3 points)                 \n"
            "   - Green: 'Double Letter' (standard score + 2 points)                 \n"
            "   - Blue:  'Double Score' (standard score * 2)                       \n\n"
           
           "To move along the grid to connect words, the following rules apply:    \n\n"
           
            "   - On even numbered letter traversals (0, 2, 4 etc)                   \n"
            "     you may only move in a cardinal left/right/up/down direction.      \n"
            "   - On odd numbered traversals (1, 3, 5 etc)                           \n"
            "     you may only move in a diagonal direction                        \n\n"
           
           "Anatomy of a Letter Block:                                                \n\n"
           "    * * * * * * *                                                           \n"
           "    * r         *  <- block color (r = red, g = green, b = blue, w = white) \n"
           "    *           *                                                           \n"
           "    *     A     *  <- block letter                                          \n"
           "    *           *                                                           \n"
           "    *         0 *  <- block letter index (0 - 24)                           \n"
           "    * * * * * * *                                                         \n\n"
           
           "Input a valid sequence of letter indices to submit found words:             \n"
           "    e.g.                                                                    \n"
           "        0,1,2,7 (even traversal and only cardinal) valid                    \n"
           "        5,1,6,7 (even traversal but has diagonal) invalid                   \n"
           "        3,7,11  (odd traversal and only diagonal) valid                   \n\n");
}

void WithinBoggle::displayWordTray() {
    
    /*
        display each blocks of the boggle word tray from top to bottom
        block data adjusts based on predefined letter blocks map
        and current available letters of the game
     */
    for(int i = 0; i < NUM_BLOCKS; i+=NUM_COLUMNS) {
        int fl = i, l = i+1, m = i+2, r = i+3, fr = i+4;
      printf("\n  * * * * * * *  * * * * * * *  * * * * * * *  * * * * * * *  * * * * * * *      \n"
               "  * %c         *  * %c         *  * %c         *  * %c         *  * %c         *      \n"
               "  *           *  *           *  *           *  *           *  *           *      \n"
               "  *     %c     *  *     %c     *  *     %c     *  *     %c     *  *     %c     * \n"
               "  *           *  *           *  *           *  *           *  *           *      \n"
               "  *        %*.*d *  *        %*.*d *  *        %*.*d *  *        %*.*d *  *        %*.*d *      \n"
               "  * * * * * * *  * * * * * * *  * * * * * * *  * * * * * * *  * * * * * * *      \n",
             LETTER_BLOCKS.at(to_string(fl)), LETTER_BLOCKS.at(to_string(l)), LETTER_BLOCKS.at(to_string(m)), LETTER_BLOCKS.at(to_string(r)), LETTER_BLOCKS.at(to_string(fr)),
             letters[fl], letters[l], letters[m], letters[r], letters[fr],
             fl < 10 ? 2 : 0, 1, fl,
             l < 10 ? 2 : 0, 1, l,
             m < 10 ? 2 : 0, 1, m,
             r < 10 ? 2 : 0, 1, r,
             fr < 10 ? 2 : 0, 1,fr);
    }
    printf("\n\n");
}

void WithinBoggle::displayScores() {
    consoleUtils::printLine(75);
    printf("Found Words:\n\n\t");
    
    int wrapIndex = 5;
    int wordIndex = 0;
    int totalScore = 0;
    
    /*
        did traversal via iterator here so that I can query the current end of the vector easily
        and remove any trailing ',' if number of words is not at wrap length
    */
    for(vector<Word>::iterator it = foundWords.begin(); it != foundWords.end(); ++it) {
        int score = it->value;
        totalScore += score;
        bool wrap = wordIndex > 0 && wordIndex % wrapIndex == 0;
        printf("%s = %d%s", it->name.c_str(), score, wrap || (it+1) == foundWords.end() ? "" : ", ");
        if(wrap) {
            printf("\n\t");
            wordIndex = 0;
        }
        wordIndex++;
    }
    
    printf("\n\nTotal Score: %d\n", totalScore);
    consoleUtils::printLine(75);
}
