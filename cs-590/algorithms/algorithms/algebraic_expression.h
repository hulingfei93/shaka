#ifndef _ALGEBRAIC_EXPRESSION_H_
#define _ALGEBRAIC_EXPRESSION_H_

#include <iostream>
#include <assert.h>
#include <fstream>
#include <string>
#include "Queue.h"
#include "Stack.h"

using namespace std;

class algebraic_expression
{
public:
	void execute(const string& file_path);
private:
	void read_data_file(const string& file_path);
	void convert_infix_to_postfix();
	int evaluate();
	int get_precedence(char c) const;
	bool isdigit(char c) const;
	int ctoi(char) const;
	void clear();

	Stack<int> eval;
	Stack<char> opStack;
	Queue<char> postQ, infixQ;
};

#endif