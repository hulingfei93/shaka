#ifndef _TIME_H_
#define _TIME_H_

#include <ostream>
#include <assert.h>

using namespace std;

class Time
{
public:
    Time(int hours = 0,
        int minites = 0,
        double seconds = 0.0);
    Time(double);
    
    ~Time();

    int get_hours() const;
    int get_minutes() const;
    double get_seconds() const;
    double get_total_seconds() const;

    friend ostream& operator<<(ostream&, const Time&);

private:
    double total_seconds;
    int hours_of_day;
    int minites_of_day;
    double seconds_of_day;
};

#endif