//
//  main.cpp
//  Practical
//
//  Created by Michael Maranan on 4/6/18.


/*
    test directives
    set 0 to disable, 1 to enable
 */
#define WALLCLOCK 0
#define WITHIN_BOGGLE 0
#define LEVEL_MAKER 1

#include <chrono> 
#include "withinBoggle.h"
#include "wallclock.h"
#include "consoleUtils.h"
#include "smartPointer.h"
#include "mapMaker.h"

// 3 minutes of play time measured in seconds
const long long int gameTimeout = 180;

// smart pointer to within boggle instance
static SmartPtr<WithinBoggle> wb;

// forward declared functions for game flow
void startGame();
int call(const char * input);
const char * getInput();
const char * mainMenuPrompt();
const char * gamePrompt();
const char * mapMakerPrompt();

// jump table for function callback
void (*callbacks[])() = { startGame, nullptr };
// function pointer array length
int fLength;

// game loop
int main(int argc, const char * argv[]) {
    
#if WALLCLOCK // WallClock Test
    
    WallClock wClock;
    puts("Correctness Test");
    printf("\tinput = %s | angle = %d\n", "12:30", wClock.getClockHandAngle("12:59"));
    printf("\tinput = %s  | angle = %d\n", "3:20", wClock.getClockHandAngle("3:20"));
    printf("\tinput = %s | angle = %d\n", "12:42", wClock.getClockHandAngle("12:42"));
    printf("\tinput = %s  | angle = %d\n", "7:47", wClock.getClockHandAngle("7:47"));
    printf("\tinput = %s  | angle = %d\n", "8:28", wClock.getClockHandAngle("8:28"));
    puts("Wrong Input Test");
    puts("\tnegative number = '-8:28'");
    wClock.getClockHandAngle("-8:28");
    puts("\tsingle number = '8'");
    wClock.getClockHandAngle("8");
    puts("\tblank = ' '");
    wClock.getClockHandAngle("");
    puts("\tminutes over 60 = '8:68'");
    wClock.getClockHandAngle("8:68");
    puts("\thour over 12 = '13:08'");
    wClock.getClockHandAngle("13:08");
    puts("\tnumber more than two characters = '112321:08'");
    wClock.getClockHandAngle("112321:08");
    puts("\tletters supplied = 'ab:cd'");
    wClock.getClockHandAngle("ab:cd");
    puts("\tadjacent delimiters = '8::28'");
    wClock.getClockHandAngle("8::28");
    
#elif WITHIN_BOGGLE // Within Boggle Test
    
    // initialize smart pointer to within boggle
    wb = new WithinBoggle();
    
    // get the length of current jump table
    while (callbacks[fLength] != nullptr) {
        fLength++;
    }
    
    // time variables
    chrono::time_point<chrono::steady_clock> start;
    long long int timeElapsedInSeconds = 0;
    long long int timeLeft = gameTimeout;
    int minutes = 0;
    int seconds = 0;
    
    // display rules of the game
    wb->displayRules();
    
    int control = 0;
    while(true) {
        switch (control) {
            case 0: // main menu
                control = call(mainMenuPrompt());
                if(control == 1) {
                    start = chrono::steady_clock::now();
                }
                continue;
            case 1: // play loop
                
                // get time left and display to console
                timeElapsedInSeconds = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
                timeLeft = gameTimeout - timeElapsedInSeconds;
                minutes = (int)((float)timeLeft / 60);
                seconds = (int)((((float)timeLeft / 60) - minutes) * 60);
                if(seconds < 0) {
                    seconds = 0;
                }
                printf("\nTime Left: %d:%s%d%s\n", minutes, seconds < 10 && seconds > 0 ? "0" : "", seconds, seconds == 0 ? "0" : "");
                
                // continue in play loop if there's still time left
                if(timeLeft > 0) {
                    control = wb->submitWord(gamePrompt());
                }
                else {
                    puts("\nYou ran out of time. GAME OVER :(");
                    control = 0;
                }
                continue;
            case 8: // level completed
                puts("\nAll words found. LEVEL COMPLETE!!!\n\n");
                control = call(mainMenuPrompt());
                continue;
            default:
                /*
                 Quit Game
                 Note:
                    Quitting the game has no associated function in the callbacks jump table
                    to avoid any unexpected behaviors in this game loop avoid declaring a case for (length of callbacks array + 1)
                */
                break;
        }
        break;
    }

    puts("\nQuitting Game.\n");
    
#elif LEVEL_MAKER // tool for making boggle levels
    
    MapMaker mm;
    // to make a new map just change the characters in this matrix
    char boggle[NUM_ROWS][NUM_COLUMNS] = {
        {'R','E','N','Q','E'},
        {'N','Y','L','N','U'},
        {'S','N','Z','N','H'},
        {'A','B','O','S','L'},
        {'C','M','A','L','B'}
    };
    // change the filename according to your naming convention
    mm.makeMap(boggle, "level0.txt");
    
#endif
    
    return 0;
}

void startGame() {
    consoleUtils::clearScreen(10);
    wb->createRandomGame();
}

// call function on jump table based on player selection
int call(const char * input) {
    char selection = input[0];
    
    // convert character input to 0 based integer index
    int funcIndex = (int) selection - '0';
    funcIndex--;
    
    if(funcIndex < 0 || funcIndex > fLength) {
        printf("\nInvalid selection (Please input number in range of (%d - %d)\n\n", 1, fLength + 1);
    }
    else {
        // only run function if function pointer is not pointing to nullptr
        if(funcIndex < fLength) {
            callbacks[funcIndex]();
        }
        // return 1 based function index for continuing in the play loop
        return funcIndex + 1;
    }
    
    return 0;
}

// get user input
const char * getInput() {
    
    fflush(stdout);
    fflush(stdin);
    const int bufferSize = 16;
    static char input[bufferSize];
    fgets(input, bufferSize, stdin);
    
    return input;
}

// display main menu prompt
const char * mainMenuPrompt() {
    
    consoleUtils::printLine(75);
    puts("Menu:");
    puts("\t1. New Game");
    puts("\t2. Quit Game");
    consoleUtils::printLine(75);
    printf(">> ");
    
    return getInput();
}

// display game prompt
const char * gamePrompt() {
    
    consoleUtils::printLine(75);
    puts("Input a valid sequence of letter indices to submit found words: \n"
         "  e.g.                                                          \n"
         "      0,1,2,7 (even traversal and only cardinal) valid          \n"
         "      5,1,6,7 (even traversal but has diagonal) invalid         \n"
         "      3,7,11  (odd traversal and only diagonal) valid         \n\n"
         "type in 'q' or 'Q' to go back to main menu                        ");
    consoleUtils::printLine(75);
    printf(">> ");
    
    return getInput();
}
