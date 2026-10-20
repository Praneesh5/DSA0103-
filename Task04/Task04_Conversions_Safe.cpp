/**
 * @file Task04_Conversions_Safe.cpp
 * @brief Task 4: Hardened Type-Safe Implementation using static_cast and Brace Initialization
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>

int main()
{
    std::cout << "====================================================\n";
    std::cout << "  TASK 4: SAFE CONVERSIONS & HARDENED TYPING        \n";
    std::cout << "====================================================\n\n";

    // 1. Preventing unintended integer division
    int a{7};
    int b{2};
    // double badC{a / b}; // Still 3.0 because division completes before initialization!
    double safeD{static_cast<double>(a) / static_cast<double>(b)};
    std::cout << "1. Explicit Double Division: " << safeD << " (Exact 3.5)\n\n";

    // 2. Character preservation with static_cast
    char ch{static_cast<char>('A' + 2)};
    char nextCh{static_cast<char>(ch + 1)};
    std::cout << "2. Preserved Character Type: " << ch << " followed by " << nextCh << " (Printed as char 'D')\n\n";

    // 3. Explicit Boolean Logic
    bool flag{5 != 0}; // Explicit comparison prevents arbitrary integer truthiness ambiguity
    std::cout << "3. Explicit Boolean State: " << std::boolalpha << flag << '\n\n';

    // 4. Safe Signed vs Unsigned Comparison
    unsigned int u{1u};
    int s{-1};

    // Correct approach 1: C++20 std::cmp_less, or casting after sign check in C++17
    bool isLessThan = (s < 0) ? true : (static_cast<unsigned int>(s) < u);
    std::cout << "4. Safe Signed/Unsigned Comparison:\n";
    std::cout << "   s = -1, u = 1u\n";
    std::cout << "   Result: " << (isLessThan ? "s < u (Mathematically Correct!)" : "s >= u (Flawed!)") << "\n\n";

    // 5. Demonstration: Brace initialization prevents narrowing conversions!
    // double val = 3.7;
    // int truncated{val}; // COMPILE-TIME ERROR: narrowing conversion from 'double' to 'int' inside {}
    std::cout << "[COMPILE-TIME SAFETY] Uniform brace initialization (int x{3.7}) rejects narrowing conversions at compile time.\n";

    std::cout << "====================================================\n";
    std::cout << "  TASK 4 SAFE IMPLEMENTATION COMPLETED              \n";
    std::cout << "====================================================\n";
    return 0;
}
