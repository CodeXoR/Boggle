//
//  smartPointer.h
//  Practical
//
//  Created by Michael Maranan on 4/7/18.

#ifndef smartPointer_h
#define smartPointer_h

/*
 Summary:
    a generic smart pointer class
 
    it's simpler to define generic classes in the header file
    that's why I also wrote the definitions in the header
 */
template <class T>
class SmartPtr
{
    // actual pointer managed by this class
    T * ptr;
public:
    // constructor
    /*
     @param name = p
     > pointer to an instance of type of T
     > defaults to null when no pointer is passed
     */
    explicit SmartPtr(T * p = NULL) { ptr = p; }
    
    // destructor
    ~SmartPtr() { delete(ptr); }
    
    /*
        overloading pointer dereferencing operator
     */
    T & operator * () { return * ptr; }
    
    /*
        overloding arrow operator so that members of T can be accessed like a pointer
    */
    T * operator -> () { return ptr; }
    
    /*
        overloading assignment operator so that a pointer to type of T
        can be set to a SmartPtr declared in a global scope
        this also makes creating an instance of SmartPtr more natural like an actual raw pointer
     */
    void operator = (T * p) { ptr = p; }
};

#endif /* smartPointer_h */
