#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>

class Time {
    int h, m;

public:
    Time(int hh = 0, int mm = 0) : h(hh), m(mm) {}

    // Friend functions (Inline vs Non-inline)
    friend int minutesBetweenInline(const Time& a, const Time& b);
    friend int minutesBetweenNoInline(const Time& a, const Time& b);

    // Member function
    inline int minutesTo(const Time& b) const {
        return std::abs((b.h * 60 + b.m) - (this->h * 60 + this->m));
    }

    // Getters
    inline int getH() const { return h; }
    inline int getM() const { return m; }
};

// 1. Explicit Inline Friend Function
inline int minutesBetweenInline(const Time& a, const Time& b) {
    return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}

// 2. Non-Inline Friend Function (Forces actual call instruction)
#if defined(__GNUC__) || defined(__clang__)
__attribute__((noinline))
#elif defined(_MSC_VER)
__declspec(noinline)
#endif
int minutesBetweenNoInline(const Time& a, const Time& b) {
    return std::abs((a.h * 60 + a.m) - (b.h * 60 + b.m));
}

// 3. Calculation via Public Getters
inline int minutesBetweenGetters(const Time& a, const Time& b) {
    return std::abs((a.getH() * 60 + a.getM()) - (b.getH() * 60 + b.getM()));
}

int main()
{
    const int ITERATIONS = 10000000;
    std::cout << "=====================================================\n";
    std::cout << "Q6: INLINE vs NOINLINE BENCHMARK (" << ITERATIONS << " Calls)\n";
    std::cout << "=====================================================\n\n";

    Time departure(9, 45);
    Time arrival(13, 10);

    volatile long long checksum = 0;

    // 1. Friend Function WITH inline
    {
        long long localSum = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            localSum += minutesBetweenInline(departure, arrival);
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        checksum += localSum;
        std::cout << "1. Friend Function (WITH inline)    : " << std::setw(6) << elapsed << " ms\n";
    }

    // 2. Friend Function WITHOUT inline
    {
        long long localSum = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            localSum += minutesBetweenNoInline(departure, arrival);
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        checksum += localSum;
        std::cout << "2. Friend Function (WITHOUT inline) : " << std::setw(6) << elapsed << " ms\n";
    }

    // 3. Member Function
    {
        long long localSum = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            localSum += departure.minutesTo(arrival);
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        checksum += localSum;
        std::cout << "3. Member Function (minutesTo)       : " << std::setw(6) << elapsed << " ms\n";
    }

    // 4. Public Getters
    {
        long long localSum = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < ITERATIONS; ++i) {
            localSum += minutesBetweenGetters(departure, arrival);
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start).count();
        checksum += localSum;
        std::cout << "4. Public Getters Approach           : " << std::setw(6) << elapsed << " ms\n";
    }

    (void)checksum;
    return 0;
}
