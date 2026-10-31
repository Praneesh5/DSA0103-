#include <iomanip>
#include <iostream>

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}

    // TODO 1: Overload + as a member function that returns a new Money object
    Money operator+(const Money& other) const {
        Money res;
        res.paise = this->paise + other.paise;
        return res;
    }

    // TODO 2: Overload == as a member, and << as a friend that prints "Rs 200.00"
    bool operator==(const Money& other) const {
        return this->paise == other.paise;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long abs_paise = m.paise < 0 ? -m.paise : m.paise;
        long long r = abs_paise / 100;
        long long p = abs_paise % 100;
        if (m.paise < 0) os << "-";
        os << "Rs " << r << '.' << std::setfill('0') << std::setw(2) << p;
        return os;
    }
};

int main()
{
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';

    // Additional Carry Test: Rs 0.75 + Rs 0.50
    Money p1(0, 75), p2(0, 50);
    Money sum = p1 + p2;
    std::cout << "Carry test (0.75 + 0.50): " << sum << "\n";

    return 0;
}
