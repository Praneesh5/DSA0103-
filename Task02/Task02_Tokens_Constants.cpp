/**
 * @file Task02_Tokens_Constants.cpp
 * @brief Task 2: Demonstrating Tokens, Keywords, Identifiers, and Constants in C++
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>
#include <iomanip>
#include <type_traits>

// ============================================================================
// 1. CONSTANTS: Evaluation of #define vs const vs constexpr
// ============================================================================

// A. Macro approach (Legacy C style): Untyped, un-scoped, textual replacement
#define LEGACY_INNER_FARE 1000
#define LEGACY_SURCHARGE 20
#define UNSAFE_FARE_CALC(f) f + f * LEGACY_SURCHARGE / 100 // Vulnerable to precedence bugs!

// B. const approach: Runtime-allocated or read-only variable (compiler dependent)
const int RUNTIME_CONST_INNER = 1000;

// C. constexpr approach (Modern C++17): Guaranteed compile-time constant, strongly typed, scoped
namespace MetroFareConfig {
    constexpr long long BASE_INNER_PAISE    = 1000LL; // Rs 10.00
    constexpr long long BASE_MIDDLE_PAISE   = 2000LL; // Rs 20.00
    constexpr long long BASE_OUTER_PAISE    = 3000LL; // Rs 30.00
    constexpr int       PEAK_SURCHARGE_PCT  = 20;     // 20%
    constexpr long long MIN_BALANCE_PAISE   = 5000LL; // Rs 50.00
}

// ============================================================================
// 2. ENUMERATIONS: enum vs enum class
// ============================================================================

// Unscoped enum: Pollutes enclosing scope, implicitly converts to int, dangerous!
enum UnscopedZone {
    U_Inner,
    U_Middle,
    U_Outer
};

// Scoped enum (enum class): Strongly typed, no implicit int conversion, scoped
enum class Zone : unsigned char {
    Inner  = 0,
    Middle = 1,
    Outer  = 2
};

// ============================================================================
// 3. COMPILE-TIME EVALUATION HELPER
// ============================================================================
constexpr long long calculatePeakFare(long long baseFare) {
    return baseFare + (baseFare * MetroFareConfig::PEAK_SURCHARGE_PCT) / 100LL;
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "  TASK 2: TOKENS, IDENTIFIERS, AND CONSTANTS DEMO   \n";
    std::cout << "====================================================\n\n";

    // Demonstration of compile-time evaluation
    constexpr long long compileTimePeakOuter = calculatePeakFare(MetroFareConfig::BASE_OUTER_PAISE);
    static_assert(compileTimePeakOuter == 3600LL, "Compile-time assertion: Outer peak fare must be 3600 paise (Rs 36.00)");
    std::cout << "[STATIC ASSERTION PASSED] Compile-time Peak Outer Fare: " << compileTimePeakOuter << " paise (Rs 36.00)\n\n";

    // 1. Tokens Breakdown
    std::cout << "1. FIVE KINDS OF TOKENS IN THIS DEMO:\n";
    std::cout << "   - Keywords:    constexpr, enum, class, long, return, sizeof, static_assert\n";
    std::cout << "   - Identifiers: MetroFareConfig, BASE_INNER_PAISE, calculatePeakFare, Zone\n";
    std::cout << "   - Constants:   1000LL, 20, 5000LL, 'A', true, \"Rs \"\n";
    std::cout << "   - Operators:   +, *, /, ==, <<, ::, =\n";
    std::cout << "   - Punctuators: ;, {, }, (, ), <, >\n\n";

    // 2. Safety comparison: enum class vs unscoped enum
    std::cout << "2. SCOPED ENUM (enum class) VS UNSCOPED ENUM:\n";
    Zone myZone = Zone::Middle;
    // int x = myZone; // COMPILE ERROR: Cannot implicitly convert 'Zone' to 'int'
    int explicitVal = static_cast<int>(myZone);
    std::cout << "   Scoped Zone::Middle explicitly cast to int: " << explicitVal << " (Type-safe!)\n";

    UnscopedZone uZone = U_Middle;
    int unsafeVal = uZone; // Silently converts to int
    std::cout << "   Unscoped U_Middle implicitly converted:     " << unsafeVal << " (Potential for bug!)\n\n";

    // 3. Demonstration of #define macro vulnerability vs constexpr
    int sample = 1000;
    // UNSAFE_FARE_CALC(sample + 50) expands to: sample + 50 + sample + 50 * 20 / 100 (WRONG PRECEDENCE!)
    int macroResult = UNSAFE_FARE_CALC(sample + 50);
    long long safeResult = calculatePeakFare(sample + 50);
    std::cout << "3. #define HAZARD VS constexpr FUNCTION:\n";
    std::cout << "   Value passed: 1050 paise\n";
    std::cout << "   UNSAFE_FARE_CALC(1000 + 50): " << macroResult << " (Buggy due to macro text expansion!)\n";
    std::cout << "   calculatePeakFare(1000 + 50): " << safeResult << " (Mathematically exact 1260 paise)\n\n";

    std::cout << "====================================================\n";
    std::cout << "  TASK 2 COMPLETED SUCCESSFULLY                     \n";
    std::cout << "====================================================\n";
    return 0;
}
