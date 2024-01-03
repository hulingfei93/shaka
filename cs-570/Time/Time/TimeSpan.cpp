#include "TimeSpan.h"

TimeSpan::TimeSpan() : seconds(0.0) {}

TimeSpan::TimeSpan(double seconds)
{
	assert(seconds >= 0.0);
	this->seconds = seconds;
}

TimeSpan::~TimeSpan(){}

double TimeSpan::get_hours() const
{
	return seconds / 3600;
}

double TimeSpan::get_minutes() const
{
	return seconds / 60;
}
double TimeSpan::get_seconds() const
{
	return seconds;
}

TimeSpan operator+(const TimeSpan &lhs, const TimeSpan &rhs)
{
	TimeSpan ts(lhs.seconds + rhs.seconds);
	return ts;
}

Time operator+(const Time &time, const TimeSpan &ts)
{
	Time t(time.get_total_seconds() + ts.seconds);
	return t;
}

Time operator+(const TimeSpan &ts, const Time &time)
{
	return time + ts;
}

ostream& operator<<(ostream &out, const TimeSpan& time)
{
	double sec_left = time.seconds;
	int hours, minites;
	hours = static_cast<int>(sec_left / 3600);
	sec_left -= hours * 3600;
	minites = static_cast<int>(sec_left / 60);
	sec_left -= minites * 60;

	out << "{";
	out << hours;
	out << ":";
	out << minites;
	out << ":";
	out << sec_left;
	out << "}";
	return out;
}