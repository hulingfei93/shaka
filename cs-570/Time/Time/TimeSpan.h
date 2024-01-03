#ifndef _TIMESPAN_H_
#define _TIMESPAN_H_

#include <ostream>
#include <assert.h>
#include "Time.h"

using namespace std;

class TimeSpan
{
public:
    TimeSpan();
    TimeSpan(double);
    ~TimeSpan();

    double get_hours() const;
    double get_minutes() const;
    double get_seconds() const;

    friend TimeSpan operator+(const TimeSpan&, const TimeSpan&);
    friend Time operator+(const Time&, const TimeSpan&);
    friend Time operator+(const TimeSpan&, const Time&);
    friend ostream& operator<<(ostream&, const TimeSpan&);

private:
    double seconds;
};

#endif