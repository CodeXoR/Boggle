//
//  consoleUtils.cpp
//  Practical
//
//  Created by Michael Maranan on 4/7/18.

#include <cstdio>
#include <string>
#include "consoleUtils.h"

using namespace std;

namespace consoleUtils {
    void printLine(const int & lineLength) {
        string line;
        // set the capacity of the line explicitly to avoid resizing
        line.reserve(lineLength);
        for (int i = 0; i < lineLength; ++i) {
            line += "-";
        }
        puts(line.c_str());
    }
    
    void clearScreen(const int & numCalls) {
        for(int i = 0; i < numCalls; ++i) {
            puts("\n\n\n\n\n\n\n\n\n\n");
        }
    }
};
