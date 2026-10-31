#include <iostream>

// The flawed developer design with overlapping default arguments:
double charge(double weight)
{
    return weight * 50.0;
}

double charge(double weight, bool express)
{
    return weight * 50.0 + (express ? 100.0 : 0.0);
}

double charge(double weight, bool express = false, bool insured = false)
{
    double cost = weight * 50.0;
    if (express) cost += 100.0;
    if (insured) cost += 50.0;
    return cost;
}

int main()
{
    // Calling charge with 1 argument:
    // Ambiguity: Compiler cannot choose between:
    // 1) charge(double)
    // 2) charge(double, bool = false, bool = false)
    std::cout << charge(5.0) << '\n';

    // Calling charge with 2 arguments:
    // Ambiguity: Compiler cannot choose between:
    // 1) charge(double, bool)
    // 2) charge(double, bool, bool = false)
    std::cout << charge(5.0, true) << '\n';

    return 0;
}
