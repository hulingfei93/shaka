#ifndef _ALGEBRAIC_EXPRESSION_TEST_H_
#define _ALGEBRAIC_EXPRESSION_TEST_H_

#include <iostream>
#include"algebraic_expression.h"

using namespace std;

void algebraic_expression_test()
{
	try
	{
		algebraic_expression expression;
		expression.execute("infile.dat");
		expression.execute("infile1.dat");
		expression.execute("infile2.dat");
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
}

#endif