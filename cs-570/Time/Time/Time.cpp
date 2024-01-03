
#include "Time.h"

using namespace std;

Time::Time(int hours, int minites, double seconds)
{
    assert(hours >= 0 && hours < 24);
    assert(minites >= 0 && minites < 60);
    assert(seconds >= 0.0 && seconds < 60);
    this->hours_of_day = hours;
    this->minites_of_day = minites;
    this->seconds_of_day = seconds;
    this->total_seconds = hours * 3600 + minites * 60 + seconds;
}

Time::Time(double total_seconds)
{
    assert(total_seconds >= 0.0 && total_seconds < 24 * 3600);
    this->total_seconds = total_seconds;

    double sec_left = total_seconds;
    this->hours_of_day = static_cast<int>(sec_left / 3600);
    sec_left -= this->hours_of_day * 3600;
    this->minites_of_day = static_cast<int>(sec_left / 60);
    sec_left -= this->minites_of_day * 60;
    this->seconds_of_day = sec_left;
}

int Time::get_hours() const
{
    return hours_of_day;
}

int Time::get_minutes() const
{
    return minites_of_day;
}

double Time::get_seconds() const
{
    return seconds_of_day;
}

double Time::get_total_seconds() const
{
    return total_seconds;
}

Time::~Time()
{}

ostream& operator<<(ostream &out, const Time &time)
{ 
    out << "{ ";
    out << time.hours_of_day;
    out << ":";
    out << time.minites_of_day;
    out << ":";
    out << time.seconds_of_day;
    out << " }";
    return out;
}