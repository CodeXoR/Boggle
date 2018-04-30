//
//  wallclock.cpp
//  Practical
//
//  Created by Michael Maranan on 4/6/18.

#include <cstdio>
#include <sstream>
#include "wallclock.h"

int WallClock::getClockHandAngle(const string & digitalFormat) {
    
    // used istringstream for splitting a string by a specific delimiter e.g. ','
    istringstream timeStream(digitalFormat);
    
    // expected number of time value input should always be 2
    const uint expectedNumValues = 2;
    
    /*
        I used an array to store separated time value inputs.
        expected size of container is constant so I thought an array would be great to use.
     */
    string timeValues[expectedNumValues];
    
    // temporary variable for storing separated time value
    string timeValue;
    
    // time values array index
    uint valueIndex = 0;
    
    // this will only run twice extracting at most two string time values delimited by ':'
    while(getline(timeStream, timeValue, ':')) {
        timeValues[valueIndex] = timeValue;
        valueIndex++;
        if(valueIndex >= expectedNumValues) {
            break;
        }
    }
    
    // if only one time value is supplied, input is invalid
    if(valueIndex < 2) {
        puts("\t\tInvalid Input. (Please supply digital time format e.g. 3:16)");
        return -1;
    }
    
    // temporary variables to store time values in hours and minutes
    int hrs = 0;
    int minutes = 0;
    
    /*
        convert string time value to an integer value
        if time value string can't be converted to an integer value, input is invalid
     */
    try {
        hrs = stoi(timeValues[0]);
        minutes = stoi(timeValues[1]);
    }
    catch (invalid_argument & ia) {
        puts("\t\tInvalid Input. (Please supply digital time format e.g. 7:47)");
        return -1;
    }
    
    /*
        valid hours input is (0-12) and valid minutes input is (0-59), anything below or over will be an invalid input
     */
    if(hrs < 0 || minutes < 0 || hrs > 12 || minutes > 59) {
        puts("\t\tInvalid Input. (Please supply digital time format e.g. 8:28)");
        return -1;
    }
    
    // convert hours to degrees and wrap value around a maximum value of 360
    hrs = (hrs * 30) % 360;
    // add movement to the hour hand relative to how much the minutes hand have moved
    hrs = hrs + (minutes / 2);
    // convert minutes to degrees
    minutes *= 6;
    
    // return the absolute value of the difference between hours and minutes in degrees
    return abs(hrs - minutes);
}
