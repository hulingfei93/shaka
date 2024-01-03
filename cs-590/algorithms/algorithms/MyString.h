#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include <assert.h>
#include <ostream>

using namespace std;

class String {

public:
    String();
    String(const char *s);
    String(const String &s);
    ~String();
    
    const String& operator=(const String &rhsObject);
    //10. Mutator operator[]
    // Usage: aStringObject[1] = 'M';
    char& operator[](int index);
    //11. Accessor operator[]
    // Usage: char ch = aStringObject[1];
    char operator[](int index) const;
    
    int Length() const;
    //13. Friend functions for == comparison
    // Usage: if (aStringObject == anotherStringObj) {...} or
    // if (aStringObject == "hello") {...} or
    // if ("hello" == aStringObj) {...} or
    friend bool operator==
        (const String &lhsObject, const String &rhsObject);
    // The other five comparison operators
    // !=, <, >, <=, >= are similarly handled as in line 13.
    friend bool operator<
        (const String &lhsObject, const String &rhsObject);
    friend bool operator>
        (const String &lhsObject, const String &rhsObject);
    friend bool operator<=
        (const String &lhsObject, const String &rhsObject);
    friend bool operator>=
        (const String &lhsObject, const String &rhsObject);
    friend bool operator!=
        (const String &lhsObject, const String &rhsObject);
    //14. Friend function for string concatination
    // Usage: StringOne = StringTwo + StringThree or
    // StringOne = "hello" + StringTwo or
    // StringOne = StringTwo + "hello"
    friend String operator+(const String &lhs, const String &rhs);
    
    const char* C_StyleString() const;
    //16. Friend function for output
    // Usage: cout << aStringObj;
    friend ostream& operator<<(ostream &out, const String &obj);
    
private:
    
    char *rep;
    int len;
};
#endif