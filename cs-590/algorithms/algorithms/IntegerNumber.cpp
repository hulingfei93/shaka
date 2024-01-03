#include <string.h>
#include "IntegerNumber.h"

const char IntegerNumber::SIGN_POSITIVE = '+';
const char IntegerNumber::SIGN_NEGATIVE = '-';

IntegerNumber::IntegerNumber()
{
    intStr = new char[2];
    intStr[0] = SIGN_POSITIVE;
    intStr[1] = '0';
    len = 1;
}

IntegerNumber::IntegerNumber(const char *str)
{
    size_t strLength = strlen(str);
    len = 0;
    if (strLength == 0)
    {
        intStr = new char[2];
        intStr[0] = SIGN_POSITIVE;
        intStr[1] = '0'; 
        ++len;
    }
    else
    {
        intStr = new char[strLength + 1];
        int i, j;
        char c;
        for (i = strLength - 1, j = 1; i > 0; --i)
        {
            c = str[i];
            if (!isDigit(c))
                throw exception("Invalid char: " + c);

            intStr[j++] = c;
            ++len;
        }

        char firstChar = str[0];
        if (isSign(firstChar))
        {
            setSign(firstChar);
        }
        else if (isDigit(firstChar))
        {
            intStr[j] = firstChar;
            setSign(SIGN_POSITIVE);
            ++len;
        }
        else 
            throw exception("Invalid char: " + c);

        if (len == 0)
        {
            throw exception("Invalid Integer format.");
        }

        // normalization
        while (intStr[len] == '0' && len > 1) // remove redundant '0'
            --len;
        if (len == 1 && intStr[len] == '0') // normalize the zero with positive sign
            setSign(SIGN_POSITIVE);
    }
    
}

IntegerNumber::IntegerNumber(const IntegerNumber& intNumber)
{
    intStr = new char[intNumber.len + 1];

    for (size_t i = 0; i <= intNumber.len; ++i)
    {
        intStr[i] = intNumber.intStr[i];
    }
    len = intNumber.len;
}

IntegerNumber::~IntegerNumber()
{
    delete[] intStr;
    len = 0;
}

IntegerNumber& IntegerNumber:: operator=(const IntegerNumber& rhs)
{
    if (this != &rhs)
    {
        delete[] intStr;
        intStr = new char[rhs.len + 1];
        for (size_t i = 0; i <= rhs.len; ++i)
        {
            intStr[i] = rhs.intStr[i];
        }
        len = rhs.len;
    }
    return *this;
}

int IntegerNumber::ctoi(char c)
{
    return c - '0';
}

char IntegerNumber::itoc(int i)
{
    return i + '0';
}

bool IntegerNumber::isSign(char c) const
{
    return c == SIGN_POSITIVE || c == SIGN_NEGATIVE;
}

const char& IntegerNumber::getSign() const
{
    return *intStr;
}

void IntegerNumber::setSign(char sign)
{
    *intStr = sign;
}

bool IntegerNumber::isPositive() const
{
    return *intStr == SIGN_POSITIVE;
}

bool IntegerNumber::isNegative() const
{
    return *intStr == SIGN_NEGATIVE;
}

bool IntegerNumber::isDigit(char c) const
{
    return c >= '0' && c <= '9';
}

void IntegerNumber::invertIntStr(char* intStr)
{
    int len = strlen(intStr);
    char *start, *end, c;
    start = intStr;
    end = intStr + len - 1;
    while (start < end)
    {
        c = *start;
        *start++ = *end;
        *end-- = c;
    }
}

int IntegerNumber::compareWithoutSign(const char* str1, const char* str2, size_t len1, size_t len2)
{ 
    if (len1 > len2)
        return 1;
    else if (len1 < len2)
        return -1;
    else // equal length
    {
        int i = len1;
        while (i > 0 && str1[i] == str2[i])
            --i;

        if (i == 0) // equal
            return 0;

        if (str1[i] < str2[i])
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
}

// friend functions

bool operator == (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    if (lhs.len != rhs.len)
        return false;

    if (lhs.getSign() != rhs.getSign())
        return false;

    for (size_t i = 1; i <= lhs.len; ++i) // equal length and sign
    {
        if (lhs.intStr[i] != rhs.intStr[i])
            return false;
    }

    return true;
}

bool operator != (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    return !(lhs == rhs);
}

bool operator < (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    // if sign is different with each other, obtain return without seeing the digits
    if (lhs.getSign() != rhs.getSign())
    {
        if (lhs.isPositive()) // 3 > -2
            return false;
        else                  //-3 < 2
            return true;
    }
    if (lhs.len < rhs.len)
    {
        if (lhs.isPositive()) // 3 < 12 
            return true;
        else                  // -3 > -12
            return false;
    }
    else if (lhs.len > rhs.len)
    {
        if (lhs.isPositive()) // 12 > 3
            return false;
        else                  // -12 < -3
            return true;
    }
    else  // equal length and sign
    { 
        int i = lhs.len;
        while (i > 0 && lhs.intStr[i] == rhs.intStr[i])
            --i;

        if (i == 0) // equal
            return false;
        
        if (lhs.intStr[i] < rhs.intStr[i])
        {
            if (lhs.isPositive())  // 5432 < 5442
                return true;
            else                   // -5432 > -5442
                return false;
        }
        else
        {
            if (lhs.isPositive())  // 5442 > 5432
                return false;
            else                   // -5442 < -5432
                return true;
        }
    }

}

bool operator > (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    return rhs < lhs;
}

bool operator <= (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    return !(rhs < lhs);
}

bool operator >= (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    return !(lhs < rhs);
}

IntegerNumber operator + (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    size_t maxLen = lhs.len > rhs.len ? lhs.len : rhs.len;
    char *tmpIntStr;
    if (lhs.getSign() == rhs.getSign())
    {
        tmpIntStr = new char[maxLen + 3]; // 3 more characters, first for sign, second for possible carry portion, third for '\0'
        memset(tmpIntStr, '\0', maxLen + 3);
        IntegerNumber::addImp(tmpIntStr + 1, lhs.intStr, rhs.intStr, lhs.len, rhs.len);
        *tmpIntStr = lhs.getSign();
    }
    else
    {
        int compareResult = IntegerNumber::compareWithoutSign(lhs.intStr, rhs.intStr, lhs.len, rhs.len);
        tmpIntStr = new char[maxLen + 2]; // 2 more characters, one for sign, another for '\0'
        memset(tmpIntStr, '\0', maxLen + 2);
        if (lhs.getSign() == IntegerNumber::SIGN_POSITIVE)
        {
            if (compareResult >= 0)
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, lhs.intStr, rhs.intStr, lhs.len, rhs.len);
                *tmpIntStr = IntegerNumber::SIGN_POSITIVE;
            }
            else
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, rhs.intStr, lhs.intStr, rhs.len, lhs.len);
                *tmpIntStr = IntegerNumber::SIGN_NEGATIVE;
            }
        }
        else
        {
            if (compareResult > 0)
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, lhs.intStr, rhs.intStr, lhs.len, rhs.len);
                *tmpIntStr = IntegerNumber::SIGN_NEGATIVE;
            }
            else
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, rhs.intStr, lhs.intStr, rhs.len, lhs.len);
                *tmpIntStr = IntegerNumber::SIGN_POSITIVE;
            }
        }
    }
    
    IntegerNumber::invertIntStr(tmpIntStr + 1);
    IntegerNumber newIntegerNumber(tmpIntStr);
    delete[] tmpIntStr;
    return newIntegerNumber;
}

IntegerNumber& IntegerNumber::operator += (const IntegerNumber& rhs)
{
    *this = (*this + rhs);
    return *this;
}

IntegerNumber operator - (const IntegerNumber& lhs, const IntegerNumber& rhs)
{
    size_t maxLen = lhs.len > rhs.len ? lhs.len : rhs.len;
    char *tmpIntStr;
    if (lhs.getSign() != rhs.getSign())
    {
        tmpIntStr = new char[maxLen + 3]; // 3 more characters, first for sign, second for possible carry portion, third for '\0'
        memset(tmpIntStr, '\0', maxLen + 3);
        IntegerNumber::addImp(tmpIntStr + 1, lhs.intStr, rhs.intStr, lhs.len, rhs.len);
        *tmpIntStr = lhs.getSign();
    }
    else
    {
        int compareResult = IntegerNumber::compareWithoutSign(lhs.intStr, rhs.intStr, lhs.len, rhs.len);
        tmpIntStr = new char[maxLen + 2]; // 2 more characters, one for sign, another for '\0'
        memset(tmpIntStr, '\0', maxLen + 2);
        if (lhs.getSign() == IntegerNumber::SIGN_POSITIVE)
        {
            if (compareResult >= 0)
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, lhs.intStr, rhs.intStr, lhs.len, rhs.len);
                *tmpIntStr = IntegerNumber::SIGN_POSITIVE;
            }
            else
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, rhs.intStr, lhs.intStr, rhs.len, lhs.len);
                *tmpIntStr = IntegerNumber::SIGN_NEGATIVE;
            }
        }
        else
        {
            if (compareResult > 0)
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, lhs.intStr, rhs.intStr, lhs.len, rhs.len);
                *tmpIntStr = IntegerNumber::SIGN_NEGATIVE;
            }
            else
            {
                IntegerNumber::subtractImp(tmpIntStr + 1, rhs.intStr, lhs.intStr, rhs.len, lhs.len);
                *tmpIntStr = IntegerNumber::SIGN_POSITIVE;
            }
        }
    }
    IntegerNumber::invertIntStr(tmpIntStr + 1);
    IntegerNumber newIntegerNumber(tmpIntStr);
    delete[] tmpIntStr;
    return newIntegerNumber;
}

IntegerNumber& IntegerNumber::operator -= (const IntegerNumber& rhs)
{
     *this = (*this - rhs);
     return *this;
}

void IntegerNumber::addImp(char *dest, const char *str1, const char *str2, size_t len1, size_t len2)
    {
        int add = 0, leftAddNumber, rightAddNumber, sum, reminder, quotient;
        size_t i, j;
        for (i = 1, j = 1; i <= len1 && j <= len2; ++i, ++j)
        {
            leftAddNumber = IntegerNumber::ctoi(str1[i]);
            rightAddNumber = IntegerNumber::ctoi(str2[j]);
            sum = leftAddNumber + rightAddNumber + add;
            reminder = sum % 10;
            quotient = sum / 10;
            *dest++ = IntegerNumber::itoc(reminder);
            add = quotient;
        }
        while (i <= len1)
        {
            sum = IntegerNumber::ctoi(str1[i]) + add;
            reminder = sum % 10;
            quotient = sum / 10;
            *dest++ = IntegerNumber::itoc(reminder);
            add = quotient;
            ++i;
        }
        while (j <= len2)
        {
            sum = IntegerNumber::ctoi(str2[j]) + add;
            reminder = sum % 10;
            quotient = sum / 10;
            *dest++ = IntegerNumber::itoc(reminder);
            add = quotient;
            ++j;
        }

        if (add > 0)
            *dest = IntegerNumber::itoc(add);
    }

void IntegerNumber::subtractImp(char *dest, const char *str1, const char *str2, size_t len1, size_t len2)
{
    int add = 0, leftSubtractNumber, rightSubtractNumber, diff;
    size_t i, j;
    for (i = 1, j = 1; i <= len1 && j <= len2; ++i, ++j)
    {
        leftSubtractNumber = IntegerNumber::ctoi(str1[i]);
        rightSubtractNumber = IntegerNumber::ctoi(str2[j]);
        diff = leftSubtractNumber - rightSubtractNumber + add;
        if (diff < 0)
        {
            diff += 10;
            add = -1;
        }
        *dest++ = IntegerNumber::itoc(diff);
    }
    while (i <= len1)
    {
        diff = IntegerNumber::ctoi(str1[i]) + add;
        if (diff < 0)
        {
            diff += 10;
            add = -1;
        }
        *dest++ = IntegerNumber::itoc(diff);
        ++i;
    }
}

ostream& operator << (ostream &out, const IntegerNumber& iNumber)
{
     if (iNumber.isNegative())
         out << '-';
     int i = iNumber.len;
     while (i > 0)
     {
         out << iNumber.intStr[i--];
     }
     return out;
}


