/**
 * @file Task06_CompleteCode.cpp
 * @brief Task 6: Completed Metro Fare Logic and Invariant Validation
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };

constexpr int BASE_PAISE[] = {1000, 2000, 3000}; // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50 must remain on the card

long long fare(Zone z, bool peak)
{
    // TODO 1: Use a switch on z to pick the base fare from BASE_PAISE, then add
    // PEAK_SURCHARGE_PERCENT % in peak hours using integer arithmetic only
    long long base = 0;
    switch (z) {
        case Zone::Inner:
            base = BASE_PAISE[0];
            break;
        case Zone::Middle:
            base = BASE_PAISE[1];
            break;
        case Zone::Outer:
            base = BASE_PAISE[2];
            break;
        default:
            base = 0;
            break;
    }

    if (peak) {
        base += (base * PEAK_SURCHARGE_PERCENT) / 100LL;
    }
    return base;
}

bool canTravel(long long balance, long long f)
{
    // TODO 2: Return true only if the balance left after paying the fare is at least
    // MIN_BALANCE_PAISE
    return (balance - f) >= MIN_BALANCE_PAISE;
}

// Helper to determine peak status based on exact hour (24-hour clock)
bool isPeakHour(int hour) {
    // 8-10 AM (8 to <10) and 5-8 PM (17 to <20)
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

int main()
{
    std::cout << "====================================================\n";
    std::cout << "  TASK 6: FARE LOGIC EXECUTION & VERIFICATION       \n";
    std::cout << "====================================================\n\n";

    long long balance = 10000; // Rs 100.00 in paise
    struct Trip { 
        Zone zone; 
        bool peak; 
        const char* description;
    };

    // Original trips from assignment + Boundary trips at 10 AM and 8 PM
    Trip trips[] = {
        {Zone::Inner,  false, "Trip 1: Inner, Off-Peak"},
        {Zone::Outer,  true,  "Trip 2: Outer, Peak (+20%)"},
        {Zone::Middle, false, "Trip 3: Middle, Off-Peak"},
        // Boundary test cases specified in Task 6:
        {Zone::Inner,  isPeakHour(10), "Boundary 1: 10:00 AM (Off-peak check)"},
        {Zone::Middle, isPeakHour(20), "Boundary 2: 20:00 / 8:00 PM (Off-peak check)"}
    };

    std::cout << "Initial Card Balance: Rs " << balance / 100 << ".00\n";
    std::cout << "----------------------------------------------------\n";

    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        std::cout << std::left << std::setw(42) << t.description << " | ";
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << " | Balance Rs " << balance / 100 << '.' << std::setw(2)
                  << std::setfill('0') << balance % 100 << std::setfill(' ') << '\n';
    }

    std::cout << "----------------------------------------------------\n";
    std::cout << "Final Retained Balance: Rs " << balance / 100 << '.' << std::setw(2)
              << std::setfill('0') << balance % 100 << std::setfill(' ') 
              << " (Strictly >= Rs 50.00)\n\n";

    std::cout << "====================================================\n";
    std::cout << "  TASK 6 EXECUTION COMPLETE                         \n";
    std::cout << "====================================================\n";
    return 0;
}
