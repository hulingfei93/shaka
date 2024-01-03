#include<iostream>
#include"Time.h"
#include"TimeSpan.h"


using namespace std;

int main(int argc, char **argv)
{
	Time dt2(13, 30);
	cout << dt2 << endl;
	
	TimeSpan ts1(65.5);
	cout << ts1 << endl;
	cout << ts1.get_minutes() << endl;
	cout << ts1.get_seconds() << endl;

	TimeSpan ts2(54.5);
	cout << ts1 + ts2 << endl;

	cout << dt2 + ts1 << endl;
	cout << ts1 + dt2 << endl;

	return 0;
}