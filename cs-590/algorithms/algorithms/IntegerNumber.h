#ifndef _INTEGER_NUMBER_H_
#define _INTEGER_NUMBER_H_

#include <ostream>
#include <string.h>


using namespace std;

class IntegerNumber
{
public:
	IntegerNumber();
	IntegerNumber(const char *);
	IntegerNumber(const IntegerNumber&);
	~IntegerNumber();

	IntegerNumber& operator = (const IntegerNumber&);
	IntegerNumber& operator += (const IntegerNumber&);
	IntegerNumber& operator -= (const IntegerNumber&);

	friend bool operator == (const IntegerNumber&, const IntegerNumber&);
	friend bool operator != (const IntegerNumber&, const IntegerNumber&);
	friend bool operator < (const IntegerNumber&, const IntegerNumber&);
	friend bool operator > (const IntegerNumber&, const IntegerNumber&);
	friend bool operator <= (const IntegerNumber&, const IntegerNumber&);
	friend bool operator >= (const IntegerNumber&, const IntegerNumber&);
	friend IntegerNumber operator + (const IntegerNumber&, const IntegerNumber&);
	friend IntegerNumber operator - (const IntegerNumber&, const IntegerNumber&);
	friend ostream& operator<<(ostream &out, const IntegerNumber &);

	

private:

	bool isSign(char c) const;
	const char& getSign() const;
	void setSign(char sign);
	bool isPositive() const;
	bool isNegative() const;
	bool isDigit(char c) const;

	// first char restore the sign, other digits restored with inverted order
	char *intStr;
	size_t len;

	static int ctoi(char);
	static char itoc(int);
	static int compareWithoutSign(const char *, const char *, size_t, size_t);
	static void addImp(char *, const char *, const char *, size_t, size_t);
	static void subtractImp(char *, const char *, const char *, size_t, size_t);
	static void invertIntStr(char *);

	static const char SIGN_POSITIVE;
	static const char SIGN_NEGATIVE;

};

#endif