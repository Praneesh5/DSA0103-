/**
 * @file Task03_DataTypes_Precision.cpp
 * @brief Task 3: Data Type Selection, Type Compatibility, and Integer Paise vs Double Precision Analysis
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cstdint>

int main() {
    std::cout << "====================================================\n";
    std::cout << "  TASK 3: DATA TYPES, COMPATIBILITY & PRECISION     \n";
    std::cout << "====================================================\n\n";

    // 1. Data Type Inventory and Size Verification
    std::cout << "1. DATA TYPE SIZES AND CAPACITIES ON THIS SYSTEM:\n";
    std::cout << "   - bool:               " << sizeof(bool)               << " byte  (Flags: peak status, eligibility)\n";
    std::cout << "   - char:               " << sizeof(char)               << " byte  (Zone labels, single chars)\n";
    std::cout << "   - int:                " << sizeof(int)                << " bytes (Hour 0-23, Minute 0-59, Zone index)\n";
    std::cout << "   - long long:          " << sizeof(long long)          << " bytes (Monetary paise, card balance)\n";
    std::cout << "   - unsigned long long: " << sizeof(unsigned long long) << " bytes (Cumulative network journey count)\n";
    std::cout << "   - float:              " << sizeof(float)              << " bytes (UNSAFE FOR MONEY)\n";
    std::cout << "   - double:             " << sizeof(double)             << " bytes (UNSAFE FOR MONEY DUE TO DRIFT)\n\n";

    // 2. Network Aggregation Capacity: Proof of int Overflow
    std::cout << "2. CAPACITY EVALUATION: CAN 'int' STORE A DAY'S NETWORK REVENUE?\n";
    std::cout << "   - INT_MAX:       " << INT_MAX << " paise (= Rs " << INT_MAX / 100 << '.' << INT_MAX % 100 << ")\n";
    std::cout << "   - LLONG_MAX:     " << LLONG_MAX << " paise\n";

    long long networkJourneys = 3000000LL; // 3 Million journeys across network
    long long avgFarePaise = 2000LL;       // Rs 20.00 average fare
    long long expectedTotalPaise = networkJourneys * avgFarePaise; // 6,000,000,000 paise

    std::cout << "   - Daily Volume:  " << networkJourneys << " journeys\n";
    std::cout << "   - Avg Fare:      " << avgFarePaise << " paise (Rs 20.00)\n";
    std::cout << "   - Total Revenue: " << expectedTotalPaise << " paise (Rs " << expectedTotalPaise / 100 << ")\n";

    int overflowedIntTotal = static_cast<int>(networkJourneys * avgFarePaise);
    std::cout << "   - Accumulated in signed 32-bit int: " << overflowedIntTotal << " paise\n";
    if (overflowedIntTotal < 0) {
        std::cout << "   [CRITICAL FAILURE] Signed int overflowed into NEGATIVE revenue: " << overflowedIntTotal << "!\n";
    }
    std::cout << "   [EVALUATION] 'int' is STRICTLY INSUFFICIENT. 'long long' is MANDATORY.\n\n";

    // 3. Precision Drift Experiment: double Rupees vs long long Paise
    std::cout << "3. FLOATING-POINT DRIFT EXPERIMENT:\n";
    std::cout << "   Simulating 1,000,000 fares of Rs 20.20 (2020 paise) each...\n";

    long long totalPaise = 0LL;
    double totalDoubleRupees = 0.0;
    const long long singleFarePaise = 2020LL;
    const double singleFareRupees = 20.20;
    const int iterations = 1000000;

    for (int i = 0; i < iterations; ++i) {
        totalPaise += singleFarePaise;
        totalDoubleRupees += singleFareRupees;
    }

    double exactExpectedRupees = static_cast<double>(totalPaise) / 100.0;
    double driftRupees = totalDoubleRupees - exactExpectedRupees;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "   - Exact Total (from long long paise): Rs " << exactExpectedRupees << '\n';
    std::cout << "   - Floating Total (from double rupees): Rs " << totalDoubleRupees << '\n';
    std::cout << "   - Accumulated Precision Drift:        Rs " << driftRupees << " (" << driftRupees * 100.0 << " paise)\n";
    std::cout << "   [EVALUATION] IEEE 754 floating-point drift creates financial loss/discrepancies.\n";
    std::cout << "                Integer paise guarantees exact penny-accurate financial balance.\n\n";

    // 4. Type Compatibility and Mixed Expressions
    std::cout << "4. TYPE COMPATIBILITY IN MIXED EXPRESSIONS:\n";
    int base = 1000;
    double multiplier = 1.20; // 20% surcharge
    // Implicit conversion: base promoted to double, expression is double, then truncated back
    long long surchargedFare = static_cast<long long>(base * multiplier);
    std::cout << "   - int * double -> double -> long long: " << surchargedFare << " paise\n";

    // Pure integer arithmetic: zero floating-point conversions
    long long pureIntFare = base + (base * 20LL) / 100LL;
    std::cout << "   - Pure integer arithmetic (base + base*20/100): " << pureIntFare << " paise\n";

    std::cout << "====================================================\n";
    std::cout << "  TASK 3 COMPLETED SUCCESSFULLY                     \n";
    std::cout << "====================================================\n";
    return 0;
}
