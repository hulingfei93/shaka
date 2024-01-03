/*
	Author: Lingfei Hu
	CWID: 10394753
	Date: 09/03/2014
*/

#include<iostream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[])
{
	double d_number;
	cout << "please input a decimal number:" << endl; //ask the user to input d decimal number
	cin >> d_number;	//store that number
	
	cout << "integer variable:" << static_cast<int>(d_number) << endl; // print out as integer without storing

	int i_number = static_cast<int>(d_number); // store the number in an integer variable

	cout << "integer variable:" << i_number << endl; // print out the integer variable

	system("pause");
	return 0;
}