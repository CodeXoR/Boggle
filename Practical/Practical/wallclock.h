//
//  wallclock.h
//  Practical
//
//  Created by Michael Maranan on 4/6/18.

#ifndef wallclock_h
#define wallclock_h

#include <string>

using namespace std;

/*
 Wallclock
    Write a function that accepts a wallclock time in digital format, e.g. "3:20" or "12:42", and
    returns what the angle between the hands would be on an analog clock that matches the given time.
    For example, with an input time of "12:30", the minute hand would be at the "6" (180 degrees) and
    the hour hand would be midway between the "12" and "1" (15 degrees). The return value would be 165 degrees (180 - 15).
 */

/*
 My thoughts on this question:
    Humbly, I thought at first look it seemed like this challenge is one that is rather easy to moderate in difficulty.
    With that in mind, it is easy to overlook the other important aspects of this challenge.
    I found that this is a good test of input handling and attention to detail.
    Some programmers could easily be complacent in answering this question and overlook the importance of checking for a correct input.
    As for the approach I took on implementing this function, initially I thought it would be great to make this a utility function under its own namespace.
    Instead of function as utility, I went on to implement this as a member function of class WallClock.
    I thought it made sense for extendability purposes and encapsulating any logically related functions pertaining
    to a particular use case of an object WallClock.
 */

/*
 Summary:
    This class encapsulates all functionality related to the usage of a WallClock.
 */
class WallClock {
public:
    /*
     finds the angle between the hands of a regular clock
        @param name = digitalFormat
            > time value in digital format passed by reference
        @return
            > returns an integer value equal to the angle between the hands of an analog clock that matches the given time
     */
    int getClockHandAngle(const string & digitalFormat);
};

#endif /* wallclock_h */
