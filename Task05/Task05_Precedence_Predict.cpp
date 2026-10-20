/**
 * @file Task05_Precedence_Predict.cpp
 * @brief Task 5: Predict the Output and Analyze Operator Precedence and Associativity
 * @course DSA01 - Assignment 1 (CO1: Bloom's Level K5 - Evaluate)
 */

#include <iostream>

int main()
{
    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';

    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';

    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';

    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';

    return 0;
}
