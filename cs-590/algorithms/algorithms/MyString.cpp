#include "MyString.h"

// Default constructor
String::String() {
    rep = new char[1];
    rep[0] = '\0';
    len = 0;
}
// Constructor. Converts a char* object to a String object
String::String(const char *s) {
    len = 0;
    const char *tmp = s;
    while (*tmp) {
        ++len;
        ++tmp;
    }
    // so now len is the same as the length of *s
    rep = new char[len + 1]; // need room for trailing 0
    for (int i = 0; i <= len; ++i)
        rep[i] = s[i];
}
// Copy constructor.
String::String(const String &obj) {
    len = 0;
    char *tmp = obj.rep;
    while (*tmp) {
            ++len;
        ++tmp;
    }
    // so now len is the same as the length of obj.rep
    rep = new char[len + 1];
    for (int i = 0; i <= len; ++i)
        rep[i] = obj.rep[i];
}
// Destructor
String::~String() {
    delete[] rep;
    len = 0;
}
// Assignment operator
const String& String::operator=(const String &rhs) {
    if (this != &rhs) {
        len = 0;
        char *tmp = rhs.rep;
        while (*tmp) {
            ++len;
            ++tmp;
        }
        // so now len is the same as the length of obj.rep
        delete[] rep;
        rep = new char[len + 1];
        for (int i = 0; i <= len; ++i)
            rep[i] = rhs.rep[i];
    }
    return *this;
}
// Mutator operator[]
char& String::operator[](int index) {
    // chech whether the index is within bounds
    assert(index <len && index >= 0);
    return rep[index];
}
// Accessor operator[]
char String::operator[](int index) const {
    // chech whether the index is within bounds
        assert(index <len && index >= 0);
    return rep[index];
}
// Get the length (number of characters) of a String object
int String::Length() const {
    return len;
}
// Friend functions for == comparison
bool operator==(const String &lhs, const String &rhs) {
    if (lhs.Length() == 0) {
        if (rhs.Length() == 0)
            return true;
        else
            return false;
    }
    else {
        int shorter = lhs.Length();
        if (rhs.Length() < shorter)
            shorter = rhs.Length();
        for (int i = 0; i < shorter; i++) {
            if (lhs.rep[i] == rhs.rep[i])
                continue;
            else
                return false;
        }
        if (lhs.Length() == rhs.Length())
            return true;
        else
            return false;
    }
}// end of function operator==
// Friend functions for < comparison
bool operator<(const String &lhs, const String &rhs) {
    if (lhs.Length() == 0) {
        if (rhs.Length() == 0)
            return false;
        else
            return true;
    }
    else {
            int shorter = lhs.Length();
        if (rhs.Length() < shorter)
            shorter = rhs.Length();
        for (int i = 0; i < shorter; i++) {
            if (lhs.rep[i] == rhs.rep[i])
                continue;
            else if (lhs.rep[i] < rhs.rep[i])
                return true;
            else //(lhs.rep[i] > rhs.rep[i])
                return false;
        }
        if (lhs.Length() == rhs.Length())
            return false;
        else if (lhs.Length() < rhs.Length())
            return true;
        else
            return false;
    }
}// end of function operator<
/****************************************************************
The operator== and operator< can be easily implemented if we include
the C++ standard header file string.h which has the following string
comparing function
int strcmp(const char *left, const char *right);
The strcmp returns integers values as follows:
if left < right it returns value < 0
if left > right it returns value > 0
if left == right it returns value = 0
So we can code == and < operators using this function as follows:
bool operator<(const String &lhs, const String &rhs) {
return (strcmp( lhs.rep, rhs.rep) == 0);
}
bool operator<(const String &lhs, const String &rhs) {
return (strcmp( lhs.rep, rhs.rep) < 0);
}
********************************************************************/
// Friend functions for > comparison
bool operator>(const String &lhs, const String &rhs) {
    return (rhs < lhs);
}
// Friend functions for <= comparison
bool operator<=(const String &lhs, const String &rhs) {
    return !(rhs < lhs);
}
// Friend functions for >= comparison
bool operator>=(const String &lhs, const String &rhs) {
    return !(lhs < rhs);
}
// Friend functions for != comparison
bool operator!=(const String &lhs, const String &rhs) {
    return !(lhs == rhs);
}
// Friend function for string concatination
String operator+(const String &lhs, const String &rhs) {
    int strLength = lhs.len + rhs.len + 1;
    char *tmpStr = new char[strLength];
    for (int i = 0; i < lhs.len; ++i)
        tmpStr[i] = lhs.rep[i];
    for (int i = 0; i <= rhs.len; ++i)
        tmpStr[lhs.len + i] = rhs.rep[i];
    String retStr(tmpStr);
    delete[] tmpStr;
    return retStr;
}
// Return C style character string
const char* String::C_StyleString() const {
    return rep;
}
// Friend function for output
ostream& operator<<(ostream &out, const String &obj) {
    return out << obj.C_StyleString();
}