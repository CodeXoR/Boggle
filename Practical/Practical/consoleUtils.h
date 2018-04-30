//
//  consoleUtils.h
//  Practical
//
//  Created by Michael Maranan on 4/7/18.

#ifndef consoleUtils_h
#define consoleUtils_h

/*
    This is a utility namespace meant
    to encapsulate any functionality related to
    printing characters to the console
 */
namespace consoleUtils {
    
    /*
        prints a succeeding '-' characters representing a line of variable length
            @param = lineLength
                > integer length of line to be printed passed by reference
     */
    void printLine(const int & lineLength);
    
    /*
        prints 10 new lines to the console per iteration
            @param = numCalls
                > integer number of iterations passed by reference
     */
    void clearScreen(const int & numCalls);
};
#endif /* consoleUtils_h */
