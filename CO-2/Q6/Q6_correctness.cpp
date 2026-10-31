#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>

class Time {
    int h, m;

public:
    Time(int hh = 0, int mm = 0) : h(hh), m(mm) {}
    
    // Friend functions
    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);

    // Member function alternatives
    int minutesTo(const Time& b) const {
        return (b.h * 60 + b.m) - (this->h * 60 + this->m);
    }
    bool isEarlier(const Time& b) const {
        return (this->h < b.h) || (this->h == b.h && this->m < b.m);
    }

    // Getters alternative
    int getH() const { return h; }
    int getM() const { return m; }

    void print() const {
        std::cout << std::setfill('0') << std::setw(2) << h << ":"
                  << std::setfill('0') << std::setw(2) << m;
    }
};

inline int minutesBetween(const Time& a, const Time& b) {
    return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}

inline bool earlier(const Time& a, const Time& b) {
    return (a.h < b.h) || (a.h == b.h && a.m < b.m);
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q6: TRAIN JOURNEY PLANNER - CORRECTNESS TEST SUITE\n";
    std::cout << "=====================================================\n\n";

    struct TestCase {
        Time t1, t2;
        int expectedDiff;
        bool expectedEarlier;
        std::string description;
    };

    TestCase tests[] = {
        { Time(9, 45), Time(13, 10), 205, true, "Standard departure & arrival (9:45 to 13:10)" },
        { Time(13, 10), Time(9, 45), 205, false, "Reverse order check (13:10 and 9:45)" },
        { Time(10, 30), Time(10, 30), 0, false, "Same time twice (10:30 and 10:30)" },
        { Time(11, 45), Time(12, 15), 30, true, "Times on either side of noon (11:45 and 12:15)" },
        { Time(0, 0), Time(23, 59), 1439, true, "Midnight (00:00) to late night (23:59)" },
        { Time(12, 0), Time(12, 1), 1, true, "Exact noon to 1 minute past noon" },
        { Time(14, 50), Time(14, 20), 30, false, "Same hour, earlier minute" }
    };

    bool allPassed = true;
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); ++i) {
        const auto& tc = tests[i];
        int actualDiff = minutesBetween(tc.t1, tc.t2);
        bool actualEarlier = earlier(tc.t1, tc.t2);

        bool diffOk = (actualDiff == tc.expectedDiff);
        bool earlierOk = (actualEarlier == tc.expectedEarlier);
        bool passed = diffOk && earlierOk;
        if (!passed) allPassed = false;

        std::cout << "Test " << (i + 1) << ": " << tc.description << '\n';
        std::cout << "  Input Times: ["; tc.t1.print(); std::cout << "] and ["; tc.t2.print(); std::cout << "]\n";
        std::cout << "  Diff: " << actualDiff << " (Expected: " << tc.expectedDiff << ") -> " 
                  << (diffOk ? "PASS" : "FAIL") << '\n';
        std::cout << "  Earlier: " << std::boolalpha << actualEarlier << " (Expected: " << tc.expectedEarlier << ") -> " 
                  << (earlierOk ? "PASS" : "FAIL") << '\n';
        std::cout << "  Result: " << (passed ? "ALL CHECKS PASSED" : "FAILED") << "\n\n";
    }

    std::cout << "Overall Correctness Status: " << (allPassed ? "100% VERIFIED SUCCESS" : "FAILURES DETECTED") << '\n';

    return 0;
}
