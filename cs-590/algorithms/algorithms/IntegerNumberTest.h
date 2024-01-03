#ifndef _INTEGER_NUMBER_TEST_H_
#define _INTEGER_NUMBER_TEST_H_

#include <iostream>
#include"IntegerNumber.h"

using namespace std;

void IntegerNumberTest()
{
	IntegerNumber A; // IntegerNumber object A
	// is created and A contains the integer 0

	IntegerNumber B("-12345678954688709764347890"); // IntegerNumber
	// object B is created and B contains the negative number shown
	// within the quotes " "

	IntegerNumber C = "5678954688709764347890"; // IntegerNumber
	// object C is created and C contains the positive number shown
	// within the quotes " "

	IntegerNumber D(B); // IntegerNumber object D is created and
	//D contains the number that B contains

	IntegerNumber E(B); // IntegerNumber object E is created and
	// E contains the number that B contains

	A = B; // assigns the value of A to that of B
	cout << A << " " << B << endl; // output to screen the

	// integers in A and B
	C = B + C;
	cout << C << endl; // output to screen the sum of the

	// numbers B and C
	D = B - C;
	cout << D << endl; // output to screen the number B - C

	if (A < B) {
		E += B;
		cout << E << endl; // output to screen the sum of E and B 
	}
	if (A >= B) {
		E -= B;
		cout << E << endl; // output to screen the number E - B 
	}
	if (A == B || C != D) {
		cout << A << " " << E << " " << D << endl; // output to screen the numbers A, E, and D
	}
}

#endif