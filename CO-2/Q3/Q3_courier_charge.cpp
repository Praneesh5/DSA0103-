#include <iostream>
#include <iomanip>
#include <string>

// Struct Parcel representing the alternative object model
struct Parcel {
    double weightKg;
    bool express;
    bool insured;
    std::string destination;

    Parcel(double w = 1.0, bool exp = false, bool ins = false, const std::string& dest = "Domestic")
        : weightKg(w), express(exp), insured(ins), destination(dest) {}
};

// =========================================================================
// REDESIGNED OVERLOAD SET (Non-Ambiguous, Clear Typing, Right-Aligned Defaults)
// =========================================================================

// Overload 1: Weight in Grams (int)
// Base rate: 0.05 per gram. Express adds 100.0, Insurance adds 50.0.
double charge(int weightGrams, bool express = false, bool insured = false)
{
    std::cout << "[Invoked: charge(int=" << weightGrams 
              << "g, express=" << std::boolalpha << express 
              << ", insured=" << insured << ")] ";
    double total = weightGrams * 0.05;
    if (express) total += 100.0;
    if (insured) total += 50.0;
    return total;
}

// Overload 2: Weight in Kilograms (double)
// Base rate: 50.0 per kg. Express adds 100.0, Insurance adds 50.0.
double charge(double weightKg, bool express = false, bool insured = false)
{
    std::cout << "[Invoked: charge(double=" << weightKg 
              << "kg, express=" << std::boolalpha << express 
              << ", insured=" << insured << ")] ";
    double total = weightKg * 50.0;
    if (express) total += 100.0;
    if (insured) total += 50.0;
    return total;
}

// Overload 3: Parcel Object (Custom Entity)
double charge(const Parcel& p)
{
    std::cout << "[Invoked: charge(const Parcel& [weight=" << p.weightKg 
              << "kg, dest=" << p.destination << "])] ";
    double total = p.weightKg * 50.0;
    if (p.express) total += 100.0;
    if (p.insured) total += 50.0;
    if (p.destination == "International") total += 250.0;
    return total;
}

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Q3: COURIER CHARGE CALCULATOR (REDESIGNED)\n";
    std::cout << "=====================================================\n\n";

    std::cout << "Testing 6 Calls - Predictions vs Actual Selections:\n\n";

    // Call 1: charge(500)
    std::cout << "1. Call: charge(500)\n";
    std::cout << "   Predicted Selection : charge(int, bool, bool) -> Exact int match, defaults applied\n";
    std::cout << "   Actual Execution    : ";
    double res1 = charge(500);
    std::cout << "-> Cost: " << res1 << "\n\n";

    // Call 2: charge(2)
    std::cout << "2. Call: charge(2)\n";
    std::cout << "   Predicted Selection : charge(int, bool, bool) -> Exact int match (2g), defaults applied\n";
    std::cout << "   Actual Execution    : ";
    double res2 = charge(2);
    std::cout << "-> Cost: " << res2 << "\n\n";

    // Call 3: charge(2.0f)
    std::cout << "3. Call: charge(2.0f)\n";
    std::cout << "   Predicted Selection : charge(double, bool, bool) -> float promotes/converts to double\n";
    std::cout << "   Actual Execution    : ";
    double res3 = charge(2.0f);
    std::cout << "-> Cost: " << res3 << "\n\n";

    // Call 4: charge(2.5, true)
    std::cout << "4. Call: charge(2.5, true)\n";
    std::cout << "   Predicted Selection : charge(double, bool, bool) -> double match, express=true, default insured=false\n";
    std::cout << "   Actual Execution    : ";
    double res4 = charge(2.5, true);
    std::cout << "-> Cost: " << res4 << "\n\n";

    // Call 5: charge(1500, false, true)
    std::cout << "5. Call: charge(1500, false, true)\n";
    std::cout << "   Predicted Selection : charge(int, bool, bool) -> int match, all 3 arguments explicitly given\n";
    std::cout << "   Actual Execution    : ";
    double res5 = charge(1500, false, true);
    std::cout << "-> Cost: " << res5 << "\n\n";

    // Call 6: charge(Parcel(4.0, true, true, "International"))
    std::cout << "6. Call: charge(Parcel(4.0, true, true, \"International\"))\n";
    std::cout << "   Predicted Selection : charge(const Parcel&) -> Exact user-defined type match\n";
    std::cout << "   Actual Execution    : ";
    Parcel p1(4.0, true, true, "International");
    double res6 = charge(p1);
    std::cout << "-> Cost: " << res6 << '\n';

    return 0;
}
