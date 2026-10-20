/**
 * @file Task10_Simulation_3M.cpp
 * @brief Task 10: High-Volume Scale Simulation (20k, 500k, 3M Journeys) & Precision/Overflow Analysis
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <climits>
#include <cstdint>

struct SimulationMetrics {
    long long journeyCount;
    int intTotalPaise;              // Demonstrates signed 32-bit overflow
    long long longLongTotalPaise;   // Exact integer accumulator
    double doubleTotalRupees;       // Demonstrates floating-point accumulation
    double driftRupees;
    double elapsedMilliseconds;
};

SimulationMetrics runSimulation(long long totalTrips) {
    // Representative distribution:
    // 40% Inner (Rs 10 off-peak / Rs 12 peak -> avg Rs 10.80 = 1080 paise)
    // 35% Middle (Rs 20 off-peak / Rs 24 peak -> avg Rs 21.60 = 2160 paise)
    // 25% Outer (Rs 30 off-peak / Rs 36 peak -> avg Rs 32.40 = 3240 paise)
    // Weighted average fare: 0.40*1080 + 0.35*2160 + 0.25*3240 = 432 + 756 + 810 = 1998 paise (Rs 19.98)

    const long long patternPaise[] = {1080LL, 2160LL, 3240LL, 1200LL, 2000LL};
    const double patternRupees[]   = {10.80,  21.60,  32.40,  12.00,  20.00};
    const size_t patternSize = 5;

    int intAcc = 0;
    long long longLongAcc = 0LL;
    double doubleAcc = 0.0;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < totalTrips; ++i) {
        size_t idx = static_cast<size_t>(i % patternSize);
        long long p = patternPaise[idx];
        double r = patternRupees[idx];

        intAcc += static_cast<int>(p);
        longLongAcc += p;
        doubleAcc += r;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    double durationMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    double exactExpectedRupees = static_cast<double>(longLongAcc) / 100.0;
    double drift = doubleAcc - exactExpectedRupees;

    return {totalTrips, intAcc, longLongAcc, doubleAcc, drift, durationMs};
}

int main() {
    std::cout << "========================================================================================\n";
    std::cout << "  TASK 10: METRO SCALE SIMULATION & PRECISION AUDIT                                     \n";
    std::cout << "  Evaluating Volumes: 1 Station (20k), 1 Line (500k), Entire Network (3M Journeys)     \n";
    std::cout << "========================================================================================\n\n";

    long long volumes[] = {20000LL, 500000LL, 3000000LL};

    std::cout << "+----------------+----------------+----------------+-----------------+----------------+\n";
    std::cout << "| Scale Tier     | Trips Processed| Long Long (Rs) | Double (Rs)     | Drift (Paise)  |\n";
    std::cout << "+----------------+----------------+----------------+-----------------+----------------+\n";

    for (long long vol : volumes) {
        SimulationMetrics m = runSimulation(vol);
        const char* tierName = (vol == 20000) ? "1 Station" : (vol == 500000) ? "1 Metro Line" : "Entire Network";

        std::cout << "| " << std::left << std::setw(15) << tierName
                  << "| " << std::right << std::setw(14) << m.journeyCount
                  << " | " << std::right << std::setw(14) << std::fixed << std::setprecision(2) << static_cast<double>(m.longLongTotalPaise) / 100.0
                  << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(4) << m.doubleTotalRupees
                  << " | " << std::right << std::setw(14) << std::fixed << std::setprecision(4) << m.driftRupees * 100.0
                  << " |\n";
    }
    std::cout << "+----------------+----------------+----------------+-----------------+----------------+\n\n";

    std::cout << "32-BIT SIGNED INTEGER OVERFLOW AUDIT (INT_MAX = " << INT_MAX << "):\n";
    for (long long vol : volumes) {
        SimulationMetrics m = runSimulation(vol);
        const char* tierName = (vol == 20000) ? "1 Station" : (vol == 500000) ? "1 Metro Line" : "Entire Network";
        std::cout << "  * " << tierName << " (" << vol << " trips):\n";
        std::cout << "      - Expected Exact Revenue: " << m.longLongTotalPaise << " paise (Rs " << m.longLongTotalPaise / 100 << ")\n";
        std::cout << "      - 32-bit int accumulator: " << m.intTotalPaise << " paise\n";
        if (m.longLongTotalPaise > INT_MAX) {
            std::cout << "      -> [CRITICAL WARNING]: INT_MAX EXCEEDED! Discrepancy: " 
                      << m.longLongTotalPaise - m.intTotalPaise << " paise! (Overflow occurred)\n";
        } else {
            std::cout << "      -> Fits safely within 32-bit int.\n";
        }
    }

    std::cout << "\n========================================================================================\n";
    std::cout << "  TASK 10 SIMULATION COMPLETE                                                           \n";
    std::cout << "========================================================================================\n";
    return 0;
}
