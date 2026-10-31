#include <iomanip>
#include <iostream>
#include <string>

class Cart {
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0; // percent

public:
    // TODO 1: add(): store the item and price if there is space, and return *this so
    //   that calls can be chained
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    // TODO 2: setDiscount(): use this-> to tell the member apart from the parameter
    //   named discount, and return *this
    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const
    {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }
};

int main()
{
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);
    Cart* p = &c; // pointer to an object
    p->add("Oil", 180);
    std::cout << std::fixed << std::setprecision(2) << p->total() << '\n';
    return 0;
}
