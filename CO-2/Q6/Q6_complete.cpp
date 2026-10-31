#include <iostream>
#include <cmath>

class Time {
    int h, m;

public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

int minutesBetween(const Time& a, const Time& b)
{
    // TODO 1: Using the private members h and m, return the absolute difference between
    // the two times in minutes
    int totalMinutesA = a.h * 60 + a.m;
    int totalMinutesB = b.h * 60 + b.m;
    return std::abs(totalMinutesA - totalMinutesB);
}

bool earlier(const Time& a, const Time& b)
{
    // TODO 2: Return true if a is earlier in the day than b
    if (a.h != b.h) {
        return a.h < b.h;
    }
    return a.m < b.m;
}

int main()
{
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << '\n';

    // Additional checks:
    // 1. Same time twice
    Time t1(10, 30), t2(10, 30);
    std::cout << "Same time (10:30 and 10:30): diff=" << minutesBetween(t1, t2) 
              << ", earlier=" << earlier(t1, t2) << '\n';

    // 2. Either side of noon (11:45 and 12:15)
    Time morning(11, 45), afternoon(12, 15);
    std::cout << "Either side of noon (11:45 and 12:15): diff=" << minutesBetween(morning, afternoon) 
              << ", earlier=" << earlier(morning, afternoon) << '\n';

    return 0;
}
