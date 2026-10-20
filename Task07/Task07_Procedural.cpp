/**
 * @file Task07_Procedural.cpp
 * @brief Task 7: Procedural Implementation of Metro Fare System (C Style)
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>

// Global procedural state & free constants
const int PROC_BASE_PAISE[] = {1000, 2000, 3000};
const int PROC_SURCHARGE = 20;
const long long PROC_MIN_BALANCE = 5000;

// Raw C struct with public data - ZERO encapsulation
struct RawCard {
    unsigned long long id;
    long long balancePaise; // Publicly accessible: Any function can write negative or bypass rules!
};

long long proc_calculate_fare(int zoneIndex, bool isPeak) {
    long long base = PROC_BASE_PAISE[zoneIndex];
    if (isPeak) {
        base += (base * PROC_SURCHARGE) / 100LL;
    }
    return base;
}

bool proc_can_travel(long long balance, long long fare) {
    return (balance - fare) >= PROC_MIN_BALANCE;
}

bool proc_deduct(RawCard* card, long long fare) {
    if (card == nullptr) return false;
    if (proc_can_travel(card->balancePaise, fare)) {
        card->balancePaise -= fare;
        return true;
    }
    return false;
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "  TASK 7: PROCEDURAL IMPLEMENTATION DEMO            \n";
    std::cout << "====================================================\n\n";

    RawCard card{1001ULL, 10000LL}; // Rs 100.00
    std::cout << "Initial Card Balance: Rs " << card.balancePaise / 100 << ".00\n";

    // Legitimate journey
    long long fare1 = proc_calculate_fare(0, false); // Inner, off-peak
    if (proc_deduct(&card, fare1)) {
        std::cout << "Journey 1 (Inner Off-Peak) Approved. Deducted: Rs " << fare1 / 100 << ".00. Balance: Rs " << card.balancePaise / 100 << ".00\n";
    }

    // Flaw Demonstration: Procedural code cannot prevent rogue code from corrupting the balance!
    std::cout << "\n[VULNERABILITY DEMONSTRATION]: Rogue function directly writes to card.balancePaise:\n";
    card.balancePaise = -1500; // Directly corrupted!
    std::cout << "Corrupted Card Balance: Rs " << card.balancePaise / 100 << ".00 (NEGATIVE BALANCE APPEARED!)\n";

    std::cout << "\n====================================================\n";
    std::cout << "  PROCEDURAL VERSION FINISHED                       \n";
    std::cout << "====================================================\n";
    return 0;
}
