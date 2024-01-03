#ifndef _REVERSE_POLISH_NOTATION_
#define _REVERSE_POLISH_NOTATION_

#include <iostream>
#include <ctype.h>
#include <stack>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <stdlib.h>
#include <math.h>

using namespace std;

typedef struct
{
    string RPN;
    double value;
    bool is_valid;
} result_item;

class buffer
{
public:
    buffer(size_t size)
    {
        this->size = size;
        buf = new char[this->size];
    }
    ~buffer()
    {
        delete[] buf;
    }
    void unget(char c)
    {
        if (static_cast<size_t>(bufp) >= size)
            throw exception("unget: overflow.");

        buf[bufp++] = c;
    }
    string get()
    {
        if (is_empty())
            throw exception("get: empty.");

        size_t length = bufp;
        char *temp = new char[length + 1];

        memcpy(temp, buf, length);
        temp[length] = '\0';

        clear();

        string str(temp);
        delete[] temp;
        return str;
    }
    bool is_empty()
    {
        return bufp == 0;
    }
    void clear()
    {
        bufp = 0;
    }
private:
    char *buf;          // buffer storage
    int bufp = 0;       // next free position in buffer
    size_t size;
};

class reverse_polish_notation
{
public:
    reverse_polish_notation();
    ~reverse_polish_notation();
    void start();

private:
    void execute();
    void get_data();
    void compute();
    void output_result();

    double evaluate(string& RPN);
    bool parse(string& RPN, deque<string>& postfix_q);

    bool is_decimal_char(char c);
    bool is_whitespace(char c);
    bool is_operator(char c);
    

    string file_path;
    vector<result_item> results;
    buffer _buffer;

    static const size_t BUFFER_SIZE;
};

#endif