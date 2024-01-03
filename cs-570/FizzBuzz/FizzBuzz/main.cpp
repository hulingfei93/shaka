/*
Author: Lingfei Hu
CWID: 10394753
Date: 09/10/2014
Email: lhu4@stevens.edu
*/

#include<iostream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[])
{
	bool is_mult_5 = false;
	bool is_mult_3 = false;

	for (int i = 53; i <= 191; ++i)
	{
		is_mult_5 = (i % 5 == 0);
		is_mult_3 = (i % 3 == 0);
		if (is_mult_5 && is_mult_3)
		{
			cout << "FizzBuzz" << endl;
		}
		else if (is_mult_5)
		{
			cout << "Fizz" << endl;
		}
		else if (is_mult_3)
		{
			cout << "Buzz" << endl;
		}
		else
		{
			cout << i << endl;
		}
	}
	return 0;
}